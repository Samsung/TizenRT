/**
 * \file    ssAPI.c
 * \brief   Secure storage API
 *
 * - Copyright : Samsung Electronics CO.LTD.,
 *
 * \internal
 * Author : Chul Lee
 * Dept : Security/Cloud Computing Lab/Convergence Solution Team/SW Center
 * Creation date : 2012/08/27
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "ssMutex.h"
#include "ssAes.h"
#include "ssSha2.h"
#include "ssAPI.h"
#include "ssDevice.h"
#include "ssDefines.h"
#include "wbaes_cmac.h"

#include "thingsmalloc.h"
#include "thingslogger.h"


#define TAG "SECURE_STORAGE"

static SecureStorageCtx ssCtx;

static ss_mutex_t* ssMutex = NULL;

static void ssLogBuffer(const char* msg, unsigned char* buf, size_t bufSize)
{
    if(msg && buf && bufSize > 0)
    {
        char* octetStr = DAMalloc(bufSize * 3 + 1);
        char* temp = octetStr;
        for(int i = 0; i < bufSize; i++)
        {
            if((i + 1) % 20 == 0 || (i + 1) == bufSize) sprintf(temp, "%02X\n", buf[i]);
            else sprintf(temp, "%02X ", buf[i]);
            temp +=  3;
        }
        THINGS_LOG_D(THINGS_DEBUG, TAG, "\n[%s]\n%s", msg, octetStr);
        DAFree(octetStr);
    }
}

/**
 * @brief    encrypt with key which is saved in seucre storage
 *
 * @param    ctx                [in] context of secure sotrage
 * @param    pInput            [in] plain data
 * @param    InputLength    [in] length of plain data
 * @param    pOutput            [out] cipher data
 * @param    pOutputLength    [out] length of cipher data
 *
 * @return    SS_SUCCESS        if no error is occured.
 */
static int ssEncrypt(SecureStorageCtx* ctx, unsigned char* pInput, unsigned int InputLength, unsigned char* pOutput, unsigned int* pOutputLength)
{
    uint32_t paddinglen = 0;

    if(NULL == ctx || NULL == pInput || 0 == InputLength || NULL == pOutput || NULL == pOutputLength)
    {
        THINGS_LOG_D_ERROR(THINGS_ERROR, TAG, "Invalid param");
        return SS_INVALID_ARGUMENT;
    }

    ssLogBuffer("MASTER SECRECT for Secure Storage", ctx->ssMasterKey, ctx->ssMasterKeyLen);
    ssLogBuffer("INITIAL VECTOR for Secure Storage", ctx->ssIV, ctx->ssIVLen);

    //padding
    paddinglen = SS_AES_BLK_SIZE - (InputLength % SS_AES_BLK_SIZE);
    memset(pOutput + InputLength, paddinglen, paddinglen);

    //encrypt with AES CBC
    SS_AES128_CBC_encrypt_buffer(pOutput, pInput, InputLength + paddinglen, ctx->ssMasterKey, ctx->ssIV);
    *pOutputLength = InputLength + paddinglen;

    return SS_SUCCESS;
}

/**
 * @brief    decrypt with key which is saved in secure storage
 *
 * @param    ctx                [in] context of secure storage
 * @param    pInput            [in] cipher data
 * @param    pInputLength    [in] length of cipher data
 * @param    pOutput            [out] plain data
 * @param    pOutputLength    [out] length of plain data
 *
 * @return    SS_SUCCESS        if no error is occured.
 */
static int ssDecrypt(SecureStorageCtx* ctx, unsigned char* pInput, unsigned int InputLength, unsigned char* pOutput, unsigned int* pOutputLength)
{
    uint32_t depaddinglen = 0;
    if(NULL == ctx || NULL == pInput || 0 == InputLength || NULL == pOutput || NULL == pOutputLength)
    {
        THINGS_LOG_D_ERROR(THINGS_ERROR, TAG, "Invalid param");
        return SS_INVALID_ARGUMENT;
    }

    *pOutputLength = 0;

    //decrypt with AES CBC
    SS_AES128_CBC_decrypt_buffer(pOutput, pInput, InputLength, ctx->ssMasterKey, ctx->ssIV);
    //de-padding
    depaddinglen = pOutput[InputLength -1];
    *pOutputLength = InputLength - depaddinglen;

    return SS_SUCCESS;
}

/**
 * @brief   API to init secure storage context
 *
 * @param   pBuf            [in] Secure storage buffer
 * @param   nBufLen         [in] SECURE_STORAGE_FORMAT or Byte-length of secure storage buffer
 * @param   nMaxBufLen      [in] max length of buffer
 *
 * @return  SS_SUCCESS                  if no error is occured.
            SS_INVALID_ARGUMENT
            SS_MEMORY_OVERFLOW
            SS_DATA_VALIDATION_FAIL
            SS_MEMORY_ALLOCATION_FAIL
 */
static int ssInitSecureStorageContext(SecureStorageCtx* ctx)
{
    unsigned char devId[SS_DEVICE_ID_MAX_SIZE] = {0};
    unsigned char hashedIV[SS_DEVICE_ID_MAX_SIZE] = {0};
    unsigned int devIdLen = 16;
    ss_sha256_ctx sha_ctx1;
    ss_sha256_ctx sha_ctx2;

    memset(ctx, 0, sizeof(SecureStorageCtx));
    ctx->ssBuffer = (unsigned char*)DAMalloc(SS_BUFFER_BLOCK_SIZE);
    if(!ctx->ssBuffer)
    {
        THINGS_LOG_D_ERROR(THINGS_ERROR, TAG, "Failed to memory allocation");
        return SS_MEMORY_ALLOCATION_FAIL;
    }

    int ssRet = ssGetDeviceID(devId, SS_DEVICE_ID_MAX_SIZE, &devIdLen);
    if(SS_SUCCESS != ssRet)
    {
        THINGS_LOG_D_ERROR(THINGS_ERROR, TAG, "Can not read device ID for secure storage. Please make sure the callback function is registered.");
        THINGS_LOG_D_ERROR(THINGS_ERROR, TAG, "Please register callback using ssSetGetDeviceUniqueIDCallback().");
        return ssRet;
    }
    ssLogBuffer("Device ID", devId, devIdLen);

    //Master key generation : SHA256 with Device Unique Value
    ss_sha256_init(&sha_ctx1);
    ss_sha256_update(&sha_ctx1, devId, devIdLen);
    ss_sha256_final(&sha_ctx1, devId);
    WBAES_CMAC(devId, devIdLen, ctx->ssMasterKey, &(ctx->ssMasterKeyLen));

    ss_sha256_init(&sha_ctx2);
    ss_sha256_update(&sha_ctx2, devId, devIdLen);
    ss_sha256_final(&sha_ctx2, hashedIV);
    memcpy(ctx->ssIV, hashedIV, SS_CRYPTO_IV_SIZE);
    ctx->ssIVLen = SS_CRYPTO_IV_SIZE;

    ctx->isOpend = 0;

    return SS_SUCCESS;
}

/**
 * @brief   finalize process for secure storage context
 *
 * @param   pBuf            [out]data in secure storage
 * @param   pBufLen         [out]length of data
 *
 * @return  SS_SUCCESS      if no error is occured.
 */
static int ssFinalSecureStorageContext(SecureStorageCtx* ctx)
{
    if (ctx == NULL)
    {
        return SS_INVALID_ARGUMENT;
    }

    memset(ctx->ssMasterKey, 0x00, ctx->ssMasterKeyLen);
    ctx->ssMasterKeyLen = 0;
    memset(ctx->ssIV, 0x00, ctx->ssIVLen);
    ctx->ssIVLen = 0;
    memset(ctx->ssBuffer, 0x00, ctx->ssBufferSize);
    DAFree(ctx->ssBuffer);
    ctx->isOpend = 0;
    ctx->curPos = 0;
    ctx->ssBufferSize = 0;

    return SS_SUCCESS;
}

int InitSecureStorage()
{
    THINGS_LOG_D(THINGS_DEBUG, TAG, "In %s", __func__);

    if (NULL == ssMutex)
    {
        ssMutex = ssMutexNew();
        if (NULL == ssMutex)
        {
            THINGS_LOG_D_ERROR(THINGS_ERROR, TAG, "ssMutexNew error!");
            return SS_ERROR;
        }
    }

    return SS_SUCCESS;
    THINGS_LOG_D(THINGS_DEBUG, TAG, "Out %s", __func__);
}

int DeinitSecureStorage()
{
    THINGS_LOG_D(THINGS_DEBUG, TAG, "In %s", __func__);

    if (!ssMutexFree(ssMutex))
    {
        THINGS_LOG_D_ERROR(THINGS_ERROR, TAG, "ssMutexFree error!");
        return SS_ERROR;
    }
    ssMutex = NULL;
    return SS_SUCCESS;
    THINGS_LOG_D(THINGS_DEBUG, TAG, "Out %s", __func__);
}

/** Persistent storage open handler.*/
FILE* secure_fopen(const char *path, const char *mode)
{
    int ssRet = SS_SUCCESS;
    FILE* fp = NULL;

    THINGS_LOG_D(THINGS_DEBUG, TAG, "secure_fopen :  %s : %s", path, mode);

    ssMutexLock(ssMutex);
    memset(&ssCtx, 0x00, sizeof(SecureStorageCtx));
    ssRet = ssInitSecureStorageContext(&ssCtx);
    if(SS_SUCCESS != ssRet)
    {
        THINGS_LOG_D_ERROR(THINGS_ERROR, TAG, "Failed to secure storage initialization : %d", ssRet);
        ssMutexUnlock(ssMutex);
        return NULL;
    }
    fp = fopen(path, mode);
    ssMutexUnlock(ssMutex);

    return fp;
}

/** Persistent storage read handler.*/
size_t secure_fread(void *ptr, size_t size, size_t nmemb, FILE *stream)
{
    uint8_t* encSVRDB = NULL;
    unsigned int encSVRDBLen = 0;
    int readLen = 0;
    int ssRet = SS_SUCCESS;
    static unsigned int ssBufSize = SS_BUFFER_BLOCK_SIZE;

    ssMutexLock(ssMutex);

    encSVRDB = (uint8_t*)DAMalloc(ssBufSize + 1);
    if(!encSVRDB)
    {
        THINGS_LOG_ERROR(THINGS_ERROR, TAG, "Failed to memory allocation!!");
        goto exit;
    }

    if(!ssCtx.isOpend)
    {
        unsigned int curBufPos = 0;
        while(1)
        {
            encSVRDBLen = fread(encSVRDB + curBufPos, size, SS_BUFFER_BLOCK_SIZE, stream);
            if(encSVRDBLen + curBufPos == 0)
            {
                THINGS_LOG_ERROR(THINGS_ERROR, TAG, "Failed to open secure storage");
                goto exit;
            }
            if(encSVRDBLen +  curBufPos < 32 + 16)
            {
                THINGS_LOG_ERROR(THINGS_ERROR, TAG, "Secure storage too small");
                goto exit;
            }

            //If secure storage buffer is not enough, try reallocation
            if(encSVRDBLen == ssBufSize - curBufPos)
            {
                uint8_t* tmpSVRDB = (uint8_t*)DAMalloc(curBufPos + encSVRDBLen + 1);
                if(!tmpSVRDB)
                {
                    THINGS_LOG_ERROR(THINGS_ERROR, TAG, "Failed to memory allocation!!");
                    goto exit;
                }
                memcpy(tmpSVRDB, encSVRDB, curBufPos + encSVRDBLen);
                DAFree(encSVRDB);
                encSVRDB = (uint8_t*)DAMalloc((ssBufSize + SS_BUFFER_BLOCK_SIZE));
                if(!encSVRDB)
                {
                    THINGS_LOG_ERROR(THINGS_ERROR, TAG, "Failed to memory allocation!!");
                    DAFree(tmpSVRDB);
                    goto exit;
                }
                memcpy(encSVRDB, tmpSVRDB, curBufPos + encSVRDBLen);
                DAFree(tmpSVRDB);

                //reallocate secure storage buffer of ss context.
                DAFree(ssCtx.ssBuffer);
                ssCtx.ssBuffer = (unsigned char*)DAMalloc((ssBufSize + SS_BUFFER_BLOCK_SIZE));
                if(!ssCtx.ssBuffer)
                {
                    THINGS_LOG_ERROR(THINGS_ERROR, TAG, "Failed to memory allocation!!");
                    goto exit;
                }
                curBufPos += encSVRDBLen;
                ssBufSize = ssBufSize + SS_BUFFER_BLOCK_SIZE;
            }
            else
            {
                break;
            }
        }
        encSVRDBLen += curBufPos;

        ssRet = ssDecrypt(&ssCtx, (unsigned char*)encSVRDB, encSVRDBLen, ssCtx.ssBuffer, &ssCtx.ssBufferSize);
        if(SS_SUCCESS != ssRet)
        {
            THINGS_LOG_D_ERROR(THINGS_ERROR, TAG, "Failed to decrypt SVR DB : %d", ssRet);
            goto exit;
        }

        ssCtx.isOpend = 1;
        ssCtx.curPos = 0;
    }

    if(ssCtx.curPos >= ssCtx.ssBufferSize)
    {
        THINGS_LOG_D(THINGS_DEBUG, TAG, "End of Secure Storage");
        readLen = 0;
    }
    else if(ssCtx.curPos == 0 && (nmemb * size) >= ssCtx.ssBufferSize)
    {
        memcpy((uint8_t*)ptr, ssCtx.ssBuffer, ssCtx.ssBufferSize);
        ssCtx.curPos = ssCtx.ssBufferSize;
        readLen = ssCtx.ssBufferSize;
    }
    else if(ssCtx.curPos != 0 && (nmemb * size) >= (ssCtx.ssBufferSize - ssCtx.curPos))
    {
        memcpy((uint8_t*)ptr, ssCtx.ssBuffer + ssCtx.curPos, ssCtx.ssBufferSize - ssCtx.curPos);
        readLen = ssCtx.ssBufferSize - ssCtx.curPos;
        ssCtx.curPos = ssCtx.ssBufferSize;
    }
    else
    {
        readLen = (nmemb * size);
        memcpy((uint8_t*)ptr, ssCtx.ssBuffer + ssCtx.curPos, readLen);
        ssCtx.curPos += readLen;
    }

exit:
    if(encSVRDB)
    {
        DAFree(encSVRDB);
    }
    ssMutexUnlock(ssMutex);

    return readLen;
}

/** Persistent storage write handler.*/
size_t secure_fwrite(const void *ptr, size_t size, size_t nmemb, FILE *stream)
{
    //encrypt ptr using secure storage
    uint8_t* encSVRDB = NULL;
    unsigned int encSVRDBLen = 0;
    int writtenLen= 0;
    int ssRet = SS_SUCCESS;

    ssMutexLock(ssMutex);
    encSVRDB = (uint8_t*)DAMalloc(CBC_MAC_SIZE + (nmemb * size) + 16); //+16 to prevent memory overflow in case of padding
    if(!encSVRDB)
    {
        THINGS_LOG_ERROR(THINGS_ERROR, TAG, "Failed to memory allocation!!");
        goto exit;
    }
    memset(encSVRDB, 0x00, CBC_MAC_SIZE + (nmemb * size) + 16);

    ssRet = ssEncrypt(&ssCtx, (unsigned char*)ptr, nmemb * size, encSVRDB, &encSVRDBLen);
    if(SS_SUCCESS != ssRet)
    {
        THINGS_LOG_D_ERROR(THINGS_ERROR, TAG, "Failed to encrypt SVR DB : %d", ssRet);
        goto exit;
    }

    writtenLen = fwrite(encSVRDB, 1, encSVRDBLen, stream);
    if(writtenLen < nmemb * size)
    {
        THINGS_LOG_ERROR(THINGS_ERROR, TAG, "Failed to write encrypted SVR DB");
        goto exit;
    }

    writtenLen = (nmemb * size);

exit:
    if(encSVRDB)
    {
        DAFree(encSVRDB);
    }
    ssMutexUnlock(ssMutex);

    return writtenLen;
}

/** Persistent storage close handler.*/
int secure_fclose(FILE *fp)
{
    int ret = 0;

    ssMutexLock(ssMutex);
    ssFinalSecureStorageContext(&ssCtx);
    ret = fclose(fp);
    ssMutexUnlock(ssMutex);

    THINGS_LOG_D(THINGS_DEBUG, TAG, "secure_fclose : %d", ret);

    return ret;
}
