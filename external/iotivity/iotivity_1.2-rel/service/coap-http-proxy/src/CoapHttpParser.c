/* ****************************************************************
 *
 * Copyright 2016 Samsung Electronics All Rights Reserved.
 *
 *
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 ******************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#include "CoapHttpParser.h"
#include "oic_malloc.h"
#include "oic_string.h"
#include "uarraylist.h"
#include "logger.h"

#include <string.h>
#include <curl/curl.h>
#ifdef HAVE_PTHREAD_H
#include <pthread.h>
#endif
#if !defined(_MSC_VER)
#include <unistd.h>
#endif //!defined(_MSC_VER)
#include <sys/types.h>
#include <fcntl.h>
#if !defined(_WIN32)
#include <sys/select.h>
#endif //!defined(_WIN32)
#include <errno.h>

#define TAG "CHP_PARSER"

#define DEFAULT_USER_AGENT "IoTivity"
#define MAX_PAYLOAD_SIZE (1048576U) // 1 MB

typedef struct
{
    void* context;
    CHPResponseCallback cb;
    HttpResponse_t resp;
    /* libcurl will not cache request payload when creating a easy handle hence we need to cache */
    void* payload;
    size_t payloadLength;
    /* To track multiple read_callbacks from curl */
    size_t readOffset;
    /* To track multiple write_callbacks from curl */
    size_t writeOffset;
    /* libcurl related */
    CURL* easyHandle;
    /* libcurl does not copy header options passed to a request */
    struct curl_slist *list;
} CHPContext_t;

/* A curl mutihandle is not threadsafe so we require mutexes to add new easy
 * handles to multihandle.
 */
static CURLM *g_multiHandle;
static int g_activeConnections;

/*  Mutex code is taken from CA.
 *  General utility functions shall be placed in common location
 *  so that all modules can use them.
 */
static pthread_mutex_t g_multiHandleMutex;

/* Fds used to signal threads to stop */
static int g_shutdownFds[2];

static bool g_terminateParser;

/*
 * Fds used to signal fdset to be refreshed.
 * When a new easy_handle is added to multi_handle,
 * existing fd_set has to be updated.
 */
static int g_refreshFds[2];

/*
 * Thread handle for curl multi_handle processing.
 */
static pthread_t g_multiHandleThread;

static void CHPParserLockMutex();
static void CHPParserUnlockMutex();

static void CHPParserResetHeaderOptions(u_arraylist_t** headerOptions)
{
    VERIFY_NON_NULL_VOID(headerOptions, TAG, "headerOptions is NULL");

    HttpHeaderOption_t *option = NULL;
    while (NULL != (option = u_arraylist_remove(*headerOptions, 0)))
    {
        OICFree(option);
    }
    u_arraylist_free(headerOptions);
}

static void CHPFreeContext(CHPContext_t *ctxt)
{
    VERIFY_NON_NULL_VOID(ctxt, TAG, "ctxt is NULL");
    if(ctxt->list)
    {
        curl_slist_free_all(ctxt->list);
    }

    if(ctxt->easyHandle)
    {
        curl_easy_cleanup(ctxt->easyHandle);
    }

    CHPParserResetHeaderOptions(&(ctxt->resp.headerOptions));
    OICFree(ctxt->resp.payload);
    OICFree(ctxt->payload);
    OICFree(ctxt);
}

static void *CHPParserExecuteMultiHandle(void* data)
{
    OIC_LOG_V(DEBUG, TAG, "%s IN", __func__);
    UNUSED(data);
    /*
     * These fd sets will be fetched from curl multi handle and monitored to execute
     * curl_multi_perform()
     */
    fd_set fdread;
    fd_set fdwrite;
    fd_set fdexcep;
    int maxfd;
    struct timeval timeout;
    struct timeval *tv;
    int activeCon;
    int activeEasyHandle;
    bool goForSelect;
    int retValue;

    /* When active connections exist, curl_multi_perform() shall be called within
     * curlMultiTimeout seconds regardless of whether select returned successfull or not.
     */
    long curlMultiTimeout;
    while (!g_terminateParser)
    {
        // Required everytime before calling curl_multi_fdset()
        FD_ZERO(&fdread);
        FD_ZERO(&fdwrite);
        FD_ZERO(&fdexcep);
        maxfd = -1;
        goForSelect = true;

        // Get currently active transfer fds from curl
        CHPParserLockMutex();
        curl_multi_fdset(g_multiHandle, &fdread, &fdwrite, &fdexcep, &maxfd);
        curl_multi_timeout(g_multiHandle, &curlMultiTimeout);
        activeCon = g_activeConnections;
        CHPParserUnlockMutex();

        // A 0 represent curl_multi_perform() shall be called.
        if(curlMultiTimeout < 0)
        {
            curlMultiTimeout = 1000;
        }

        if(maxfd == -1)
        {
            /* Nothing to monitor from libcurl.
             * This mean that no active sockets exist either because
             * there are no transfer taking place or sockets are not in a
             * state that could be monitored (connecting, retry etc.)
             */
            if(!activeCon)
            {
                // wait until something received on shutdown or refresh fd
                // with no timeout.
                curlMultiTimeout = -1;
            }
            else
            {
                // libcurl recommend doing this.
                usleep(100000);
                // dont select() and directly call curl_multi_perform()
                goForSelect = false;
            }
        }

        if(goForSelect)
        {
            FD_SET(g_shutdownFds[0], &fdread);
            if (maxfd < g_shutdownFds[0])
            {
                maxfd = g_shutdownFds[0];
            }

            FD_SET(g_refreshFds[0], &fdread);
            if (maxfd < g_refreshFds[0])
            {
                maxfd = g_refreshFds[0];
            }

            if(curlMultiTimeout == -1)
            {
                tv = NULL;
            }
            else
            {
                timeout.tv_sec = curlMultiTimeout / 1000;
                timeout.tv_usec = 0;
                tv = &timeout;
            }

            // Select here
            retValue = select(maxfd + 1, &fdread, &fdwrite, &fdexcep, tv);
            if(retValue == -1)
            {
                OIC_LOG_V(ERROR, TAG, "Error in select. %s", strerror(errno));
                continue;
            }

            // Some sockets are available for operations, check if shutdown or refresh fds are
            // among them. In any case, go ahead and call curl_multi_perform()
            if(retValue)
            {
                if (FD_ISSET(g_shutdownFds[0], &fdread))
                {
                    OIC_LOG(ERROR, TAG, "Shutdown requested. multi_handle returning");
                    break;
                }
                else if(FD_ISSET(g_refreshFds[0], &fdread))
                {
                    char buf[20] = {0};
                    ssize_t len = read(g_refreshFds[0], buf, sizeof(buf));
                    UNUSED(len);
                    // new easy handles added, call multi_perform and refresh fds.
                    OIC_LOG(ERROR, TAG, "New easy handle added");
                }
            }
        }

        CURLMcode ret;
        CHPParserLockMutex();
        do
        {
            ret = curl_multi_perform(g_multiHandle, &activeEasyHandle);
            struct CURLMsg *cmsg;
            int cmsgq;
            do
            {
                cmsgq = 0;
                cmsg = curl_multi_info_read(g_multiHandle, &cmsgq);
                if(cmsg && (cmsg->msg == CURLMSG_DONE))
                {
                    CURL *easyHandle = cmsg->easy_handle;
                    g_activeConnections--;
                    curl_multi_remove_handle(g_multiHandle, easyHandle);

                    CHPContext_t *ptr;
                    char *uri = NULL;
                    char *contentType = NULL;
                    long responseCode;

                    curl_easy_getinfo(easyHandle, CURLINFO_PRIVATE, &ptr);
                    curl_easy_getinfo(easyHandle, CURLINFO_EFFECTIVE_URL, &uri);
                    curl_easy_getinfo(easyHandle, CURLINFO_RESPONSE_CODE, &responseCode);
                    curl_easy_getinfo(easyHandle, CURLINFO_CONTENT_TYPE, &contentType);

                    ptr->resp.status = responseCode;
                    OICStrcpy(ptr->resp.dataFormat, sizeof(ptr->resp.dataFormat), contentType);
                    OIC_LOG_V(DEBUG, TAG, "Transfer completed %d uri: %s, %s", g_activeConnections,
                                                                           uri, contentType);
                    ptr->cb(&(ptr->resp), ptr->context);
                    CHPFreeContext(ptr);
                }
            } while(cmsg && !g_terminateParser);
        }while (ret == CURLM_CALL_MULTI_PERFORM && !g_terminateParser);
        CHPParserUnlockMutex();
    }

    if (g_terminateParser)
    {
        OIC_LOG_V(DEBUG, TAG, "Shutdown request received.");
        // g_shutdownFds[1] will be already closed.
        close(g_shutdownFds[0]);
        close(g_refreshFds[0]);
        close(g_refreshFds[1]);
        g_shutdownFds[0] = -1;
        g_shutdownFds[1] = -1;
        g_refreshFds[0] = -1;
        g_refreshFds[1] = -1;
    }

    OIC_LOG_V(DEBUG, TAG, "%s IN", __func__);
    return NULL;
}

OCStackResult CHPParserInitializePipe(int fds[2])
{
    OIC_LOG_V(DEBUG, TAG, "%s IN", __func__);
    int ret = -1;
    fds[0] = -1;
    fds[1] = -1;
#if defined(HAVE_PIPE2)
    ret = pipe2(fds, O_CLOEXEC);
#else
    ret = pipe(fds);
    if (-1 != ret)
    {
        ret = fcntl(fds[0], F_GETFD);
        if (-1 != ret)
        {
            ret = fcntl(fds[0], F_SETFD, ret|FD_CLOEXEC);
        }
        if (-1 != ret)
        {
            ret = fcntl(fds[1], F_GETFD);
        }
        if (-1 != ret)
        {
            ret = fcntl(fds[1], F_SETFD, ret|FD_CLOEXEC);
        }
        if (-1 == ret)
        {
            close(fds[1]);
            close(fds[0]);
            fds[0] = -1;
            fds[1] = -1;
        }
    }
#endif
    if (-1 == ret)
    {
        OIC_LOG_V(ERROR, TAG, "FD initialization failed: %s", strerror(errno));
        return OC_STACK_ERROR;
    }
    OIC_LOG_V(DEBUG, TAG, "%s OUT", __func__);
    return OC_STACK_OK;
}

static OCStackResult CHPParserInitializeMutex()
{
    // create the mutex with the attributes set
    int ret = pthread_mutex_init(&g_multiHandleMutex, PTHREAD_MUTEX_DEFAULT);
    if (0 != ret)
    {
        OIC_LOG_V(ERROR, TAG, "%s Failed to initialize mutex !", __func__);
        return OC_STACK_ERROR;
    }
    return OC_STACK_OK;
}

static OCStackResult CHPParserTerminateMutex()
{
    int ret = pthread_mutex_destroy(&g_multiHandleMutex);
    if (0 != ret)
    {
        OIC_LOG_V(ERROR, TAG, "%s Failed to free mutex !", __func__);
        return OC_STACK_ERROR;
    }
    return OC_STACK_OK;
}

static void CHPParserLockMutex()
{
    int ret = pthread_mutex_lock(&g_multiHandleMutex);
    if(ret != 0)
    {
        OIC_LOG_V(ERROR, TAG, "Pthread Mutex lock failed: %d", ret);
    }
}

static void CHPParserUnlockMutex()
{
    int ret = pthread_mutex_unlock(&g_multiHandleMutex);
    if(ret != 0)
    {
        OIC_LOG_V(ERROR, TAG, "Pthread Mutex unlock failed: %d", ret);
    }
}

static OCStackResult CHPParserInitializeMultiHandle()
{
    CHPParserLockMutex();
    if(g_multiHandle)
    {
        OIC_LOG(ERROR, TAG, "Multi handle already initialized.");
        CHPParserUnlockMutex();
        return OC_STACK_OK;
    }

    g_multiHandle = curl_multi_init();
    if(!g_multiHandle)
    {
        OIC_LOG(ERROR, TAG, "Failed to create multi handle.");
        CHPParserUnlockMutex();
        return OC_STACK_ERROR;
    }

    CHPParserUnlockMutex();
    return OC_STACK_OK;
}

static OCStackResult CHPParserTerminateMultiHandle()
{
    CHPParserLockMutex();
    if(!g_multiHandle)
    {
        OIC_LOG(ERROR, TAG, "Multi handle not initialized.");
        CHPParserUnlockMutex();
        return OC_STACK_OK;
    }

    curl_multi_cleanup(g_multiHandle);
    g_multiHandle = NULL;
    CHPParserUnlockMutex();
    return OC_STACK_OK;
}

OCStackResult CHPParserInitialize()
{
    OIC_LOG_V(DEBUG, TAG, "%s IN", __func__);

    OCStackResult ret = CHPParserInitializeMutex();
    if(ret != OC_STACK_OK)
    {
        return ret;
    }

    ret = CHPParserInitializeMultiHandle();
    if(ret != OC_STACK_OK)
    {
        OIC_LOG_V(ERROR, TAG, "Failed to intialize multi handle: %d", ret);
        CHPParserTerminate();
        return ret;
    }

    ret = CHPParserInitializePipe(g_shutdownFds);
    if(ret != OC_STACK_OK)
    {
        OIC_LOG_V(ERROR, TAG, "Failed to intialize shutdown fds: %d", ret);
        CHPParserTerminate();
        return ret;
    }

    ret = CHPParserInitializePipe(g_refreshFds);
    if(ret != OC_STACK_OK)
    {
        OIC_LOG_V(ERROR, TAG, "Failed to intialize refresh fds: %d", ret);
        CHPParserTerminate();
        return ret;
    }

    // Launch multi_handle processor thread
    int result = pthread_create(&g_multiHandleThread, NULL, CHPParserExecuteMultiHandle, NULL);
    if(result != 0)
    {
        OIC_LOG_V(ERROR, TAG, "Thread start failed with error %d", result);
        CHPParserTerminate();
        return OC_STACK_ERROR;
    }

    g_terminateParser = false;
    CHPParserLockMutex();
    g_activeConnections = 0;
    CHPParserUnlockMutex();
    OIC_LOG_V(DEBUG, TAG, "%s OUT", __func__);
    return OC_STACK_OK;
}

OCStackResult CHPParserTerminate()
{
    OIC_LOG_V(DEBUG, TAG, "%s IN", __func__);
    g_terminateParser = true;
    if (g_shutdownFds[1] != -1)
    {
        // Signal multi_handle thread to come out
        close(g_shutdownFds[1]);
    }
    pthread_join(g_multiHandleThread, NULL);

    OCStackResult ret = CHPParserTerminateMultiHandle();
    if(ret != OC_STACK_OK)
    {
        OIC_LOG_V(ERROR, TAG, "Multi handle termination failed: %d", ret);
    }

    CHPParserLockMutex();
    g_activeConnections = 0;
    CHPParserUnlockMutex();

    ret = CHPParserTerminateMutex();
    if(ret != OC_STACK_OK)
    {
        OIC_LOG_V(ERROR, TAG, "mutex termination failed: %d", ret);
    }
    OIC_LOG_V(DEBUG, TAG, "%s OUT", __func__);
    return OC_STACK_OK;
}

static size_t CHPEasyHandleWriteCb(char *buffer, size_t size, size_t num, void *context)
{
    size_t dataToWrite = size * num;
    if(!dataToWrite)
    {
        // Empty payload received. Ignore.
        return 0;
    }

    if(!context || !buffer || g_terminateParser)
    {
        OIC_LOG_V(ERROR, TAG, "%s invalid arguments or terminating", __func__);
        return 0;
    }

    CHPContext_t* ctx = context;
    HttpResponse_t *resp = &(ctx->resp);

    if(ctx->writeOffset + dataToWrite > MAX_PAYLOAD_SIZE)
    {
        OIC_LOG_V(ERROR, TAG, "%s Payload limit exceeded", __func__);
        resp->payloadLength = 0;
        ctx->writeOffset = 0;
        OICFree(resp->payload);
        resp->payload = NULL;
        return 0;
    }

    if (!resp->payload)
    {
        resp->payload = OICMalloc(dataToWrite);
        if (!resp->payload)
        {
            OIC_LOG_V(ERROR, TAG, "%s Out of memory!", __func__);
            return 0;
        }
    }
    else
    {
        // Realloc buffer
        void *newPayload = OICRealloc(resp->payload, ctx->writeOffset + dataToWrite);
        if (!newPayload)
        {
            OIC_LOG_V(ERROR, TAG, "Realloc failed! Current: %u Extra: %u", ctx->writeOffset,
                                                                           dataToWrite);
            resp->payloadLength = 0;
            ctx->writeOffset = 0;
            OICFree(resp->payload);
            resp->payload = NULL;
            return 0;
        }
        resp->payload = newPayload;
    }

    memcpy(resp->payload + ctx->writeOffset, buffer, dataToWrite);
    ctx->writeOffset += dataToWrite;
    resp->payloadLength = ctx->writeOffset;

    OIC_LOG_V(DEBUG, TAG, "%s OUT %u : %u", __func__, resp->payloadLength, dataToWrite);
    return dataToWrite;
}

static size_t CHPEasyHandleReadCb(char *buffer, size_t size, size_t num, void *context)
{
    if(!context || !buffer || g_terminateParser)
    {
        OIC_LOG_V(ERROR, TAG, "%s invalid arguments or terminating", __func__);
        return CURL_READFUNC_ABORT;
    }

    CHPContext_t *ctx = context;
    size_t remainingLen = ctx->payloadLength - ctx->readOffset;
    size_t toTransfer = size * num > remainingLen ? remainingLen : size * num;
    memcpy(buffer, ctx->payload + ctx->readOffset, toTransfer);
    ctx->readOffset += toTransfer;
    return toTransfer;
}

static size_t CHPEasyHandleHeaderCb(char *buffer, size_t size, size_t num, void *context)
{
    size_t dataToWrite = size * num;
    if(!buffer || !dataToWrite || !context || g_terminateParser)
    {
        OIC_LOG_V(ERROR, TAG, "%s invalid arguments or terminating", __func__);
        return 0;
    }

    /* curl will call this function for each line in response header including status line
     * and for each http response that it might have received from http server for ex: redirect,
     * proxy handshakes etc. All these intermediary responses are not useful for us but there
     * isn't any mechanism to track which one is going to be final.
     * Hence here we process each response and assume that the relevant one will be the final
     * response.
     */

    /* Start of a response is tracked by presence of status line starting with "HTTP/"
     * This also acts as a reset for everything else (payload, header options) as we are processing
     * a new response.
     */

    CHPContext_t *ctx = context;
    HttpResponse_t *resp = &(ctx->resp);
    if (dataToWrite > 5)
    {
        if (strncmp("HTTP/", buffer, 5) == 0)
        {
            OIC_LOG(ERROR, TAG, "New header received");
            resp->payloadLength = 0;
            ctx->writeOffset = 0;
            OICFree(resp->payload);
            resp->payload = NULL;
            CHPParserResetHeaderOptions(&(resp->headerOptions));
            // This is a status line. We are only interested in header options.
            return dataToWrite;
        }
    }


    // A header line can have CR LF NULL and spaces at end. Make endOfHeader point to last
    // character in header value
    char* endOfHeader = buffer + dataToWrite;
    while ((endOfHeader > buffer) && (*endOfHeader == '\r' || *endOfHeader == '\n'
                || *endOfHeader == ' ' || *endOfHeader == '\0'))
    {
        endOfHeader--;
    }

    /* curl might not send the buffer NULL terminated and copying each header is too much overhead
     * hence use mem family of function to search */
    char* ptr = (char*) memchr(buffer, ':', dataToWrite);
    // There is a colon and its not the first character
    if(ptr && ptr != buffer && ptr <= endOfHeader)
    {
        size_t headerFieldLen = ptr - buffer;
        size_t headerValueLen;
        char* headerValuePtr;

        /* Skip any white spaces */
        ptr++;
        while(ptr <= endOfHeader && *ptr == ' ')
        {
            ptr++;
        }

        if(ptr > endOfHeader)
        {
            headerValueLen = 0;
            headerValuePtr = NULL;
        }
        else
        {
            // endOfHeader is pointing to last header value character hence +1
            headerValueLen = endOfHeader - ptr + 1;
            headerValuePtr = ptr;
        }

        if (!(resp->headerOptions))
        {
            // First header callback, assign storage for header options
            resp->headerOptions = u_arraylist_create();
            if (!(resp->headerOptions))
            {
                OIC_LOG(ERROR, TAG, "Memory failed!");
                return 0;
            }
        }

        HttpHeaderOption_t *option = OICCalloc(1, sizeof(HttpHeaderOption_t));
        if (!option)
        {
            OIC_LOG(ERROR, TAG, "Memory failed!");
            return 0;
        }

        headerFieldLen = headerFieldLen > (sizeof(option->optionName) - 1) ?
                             (sizeof(option->optionName) - 1): headerFieldLen;
        memcpy(option->optionName, buffer, headerFieldLen);
        option->optionName[headerFieldLen] = '\0';

        if(headerValueLen)
        {
            headerValueLen = headerValueLen > (sizeof(option->optionData) - 1) ?
                              (sizeof(option->optionData) - 1): headerValueLen;
            memcpy(option->optionData, headerValuePtr, headerValueLen);
            option->optionData[headerValueLen] = '\0';
        }

        OIC_LOG_V(DEBUG, TAG, "%s:: %s: %s", __func__, option->optionName, option->optionData);
        // Add to header option list
        if(!u_arraylist_add(resp->headerOptions, option))
        {
            OIC_LOG(ERROR, TAG, "u_arraylist_add failed!");
            OICFree(option);
            return 0;
        }
    }

    // ignore else as this might be CRLF header lines.
    return dataToWrite;
}

static OCStackResult CHPInitializeEasyHandle(CURL** easyHandle, HttpRequest_t *req,
                                             CHPContext_t* handleContext)
{
    OIC_LOG_V(DEBUG, TAG, "%s IN", __func__);
    VERIFY_NON_NULL_RET(req, TAG, "req", OC_STACK_INVALID_PARAM);
    VERIFY_NON_NULL_RET(easyHandle, TAG, "easyHandle", OC_STACK_INVALID_PARAM);
    VERIFY_NON_NULL_RET(handleContext, TAG, "handleContext", OC_STACK_INVALID_PARAM);

    CURL *e = curl_easy_init();
    if(!e)
    {
        OIC_LOG(ERROR, TAG, "easy init failed!");
        return OC_STACK_ERROR;
    }

    /* Set http resource uri */
    curl_easy_setopt(e, CURLOPT_URL, req->resourceUri);
    /* Default protocol when scheme is not available in uri */
    // curl version 7.22 don't support this option.
    //curl_easy_setopt(e, CURLOPT_DEFAULT_PROTOCOL, "http");
    /* Set handle context */
    curl_easy_setopt(e, CURLOPT_PRIVATE, handleContext);
    curl_easy_setopt(e, CURLOPT_WRITEFUNCTION, CHPEasyHandleWriteCb);
    curl_easy_setopt(e, CURLOPT_WRITEDATA, handleContext);
    curl_easy_setopt(e, CURLOPT_READFUNCTION, CHPEasyHandleReadCb);
    curl_easy_setopt(e, CURLOPT_READDATA, handleContext);
    curl_easy_setopt(e, CURLOPT_HEADERFUNCTION, CHPEasyHandleHeaderCb);
    curl_easy_setopt(e, CURLOPT_HEADERDATA, handleContext);

    /* Allow access to only http server's */
    curl_easy_setopt(e, CURLOPT_PROTOCOLS,
                     CURLPROTO_HTTP | CURLPROTO_HTTPS);
    /* complete connection within 15 seconds */
    curl_easy_setopt(e, CURLOPT_CONNECTTIMEOUT, 15L);
    /* Abort transaction if getting less than 1kbps for 60 seconds */
    curl_easy_setopt(e, CURLOPT_LOW_SPEED_LIMIT, 1024L);
    curl_easy_setopt(e, CURLOPT_LOW_SPEED_TIME, 60L);
    curl_easy_setopt(e, CURLOPT_USERAGENT, DEFAULT_USER_AGENT);
    /* Close connection once done with transaction */
    curl_easy_setopt(e, CURLOPT_FORBID_REUSE, 1L);
    /* Allow redirect */
    curl_easy_setopt(e, CURLOPT_FOLLOWLOCATION, 1L);
    /* Only redirect to http servers */
    curl_easy_setopt(e, CURLOPT_REDIR_PROTOCOLS,
                     CURLPROTO_HTTP | CURLPROTO_HTTPS);
    /* Limit maximum redirects */
    curl_easy_setopt(e, CURLOPT_MAXREDIRS, 10L);

    handleContext->writeOffset = 0;
    handleContext->readOffset = 0;
    switch(req->method)
    {
        case CHP_GET:
            OIC_LOG(DEBUG, TAG, "Sending GET request");
            curl_easy_setopt(e, CURLOPT_HTTPGET, 1);
            break;
        case CHP_POST:
            OIC_LOG(DEBUG, TAG, "Sending POST request");
            curl_easy_setopt(e, CURLOPT_POST, 1);
            curl_easy_setopt(e, CURLOPT_POSTFIELDS, NULL);
            curl_easy_setopt(e, CURLOPT_POSTFIELDSIZE, req->payloadLength);
            handleContext->payloadLength = req->payloadLength;
            handleContext->payload = req->payload;
            req->payloadCached = true;
            break;
        case CHP_PUT:
            OIC_LOG(DEBUG, TAG, "Sending PUT request");
            curl_easy_setopt(e, CURLOPT_UPLOAD, 1);
            curl_easy_setopt(e, CURLOPT_INFILESIZE, req->payloadLength);
            handleContext->payloadLength = req->payloadLength;
            handleContext->payload = req->payload;
            req->payloadCached = true;
            break;;
        case CHP_DELETE:
            OIC_LOG(DEBUG, TAG, "Sending DELETE request");
            /* libcurl don't have direct option for sending DELETE */
            curl_easy_setopt(e, CURLOPT_CUSTOMREQUEST, "DELETE");
            break;
        default:
            return OC_STACK_INVALID_METHOD;
    }

    // Add header options from request
    struct curl_slist *list = NULL;
    char buffer[CHP_MAX_HF_NAME_LENGTH + CHP_MAX_HF_DATA_LENGTH + 2]; // extra 2 bytes for ": "

    if (req->headerOptions)
    {
        HttpHeaderOption_t *option = NULL;
        int headerCount = u_arraylist_length(req->headerOptions);
        for(int i = 0; i < headerCount; i++)
        {
            option = u_arraylist_get(req->headerOptions, i);
            if(option)
            {
                OIC_LOG_V(DEBUG, TAG, "Adding header option: %s", buffer);
                snprintf(buffer, sizeof(buffer), "%s: %s", option->optionName, option->optionData);
                list = curl_slist_append(list, buffer);
            }
        }
    }

    /* Add content-type and accept header */
    snprintf(buffer, sizeof(buffer), "Accept: %s", req->acceptFormat);
    list = curl_slist_append(list, buffer);
    snprintf(buffer, sizeof(buffer), "Content-Type: %s", req->payloadFormat);
    curl_easy_setopt(e, CURLOPT_HTTPHEADER, list);

    *easyHandle = e;
    OIC_LOG_V(DEBUG, TAG, "%s OUT", __func__);
    return OC_STACK_OK;
}

OCStackResult CHPPostHttpRequest(HttpRequest_t *req, CHPResponseCallback httpcb,
                                 void *context)
{
    OIC_LOG_V(DEBUG, TAG, "%s IN", __func__);
    VERIFY_NON_NULL_RET(req, TAG, "req", OC_STACK_INVALID_PARAM);
    VERIFY_NON_NULL_RET(httpcb, TAG, "httpcb", OC_STACK_INVALID_PARAM);

    CHPContext_t *ctxt = OICCalloc(1, sizeof(CHPContext_t));
    if (!ctxt)
    {
        OIC_LOG(ERROR, TAG, "Memory failed!");
        return OC_STACK_NO_MEMORY;
    }

    ctxt->cb = httpcb;
    ctxt->context = context;
    OCStackResult ret = CHPInitializeEasyHandle(&ctxt->easyHandle, req, ctxt);
    if(ret != OC_STACK_OK)
    {
        OIC_LOG_V(ERROR, TAG, "Failed to initialize easy handle [%d]", ret);
        OICFree(ctxt);
        return ret;
    }

    // Add easy_handle to multi_handle
    CHPParserLockMutex();
    curl_multi_add_handle(g_multiHandle, ctxt->easyHandle);
    g_activeConnections++;
    CHPParserUnlockMutex();
    // Notify refreshfd
    ssize_t len = 0;
    do
    {
        len = write(g_refreshFds[1], "w", 1);
    } while ((len == -1) && (errno == EINTR));

    if ((len == -1) && (errno != EINTR) && (errno != EPIPE))
    {
        OIC_LOG_V(DEBUG, TAG, "refresh failed: %s", strerror(errno));
    }

    OIC_LOG_V(DEBUG, TAG, "%s OUT", __func__);
    return OC_STACK_OK;
}

