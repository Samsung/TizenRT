/******************************************************************
 *
 * Copyright 2014 Samsung Electronics All Rights Reserved.
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

#include "caadapterutils.h"

#include <string.h>
#include <ctype.h>
#include "oic_string.h"
#include "oic_malloc.h"
#include <errno.h>

#ifndef WITH_ARDUINO
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#endif

#ifdef __ANDROID__
#include <jni.h>
#endif

#define CA_ADAPTER_UTILS_TAG "OIC_CA_ADAP_UTILS"

#ifdef __ANDROID__
/**
 * @var g_jvm
 * @brief pointer to store JavaVM
 */
static JavaVM *g_jvm = NULL;

/**
 * @var gContext
 * @brief pointer to store context for android callback interface
 */
static jobject g_Context = NULL;
static jobject g_Activity = NULL;
#endif

#ifdef WITH_ARDUINO
CAResult_t CAParseIPv4AddressInternal(const char *ipAddrStr, uint8_t *ipAddr,
                                      size_t ipAddrLen, uint16_t *port)
{
  if (!ipAddr || !isdigit(ipAddrStr[0]) || !port)
  {
    OIC_LOG(ERROR, CA_ADAPTER_UTILS_TAG, "invalid param");
    return CA_STATUS_INVALID_PARAM;
  }

  size_t index = 0;
  uint8_t dotCount = 0;

  ipAddr[index] = 0;
  *port = 0;
  while (*ipAddrStr)
  {
    if (isdigit(*ipAddrStr))
    {
      if (index >= ipAddrLen)
      {
        OIC_LOG(ERROR, CA_ADAPTER_UTILS_TAG, "invalid param");
        return CA_STATUS_INVALID_PARAM;
      }
      ipAddr[index] *= 10;
      ipAddr[index] += *ipAddrStr - '0';
    }
    else if (*ipAddrStr == '.')
    {
      index++;
      dotCount++;
      ipAddr[index] = 0;
    }
    else
    {
      break;
    }
    ipAddrStr++;
  }

  if (*ipAddrStr == ':')
  {
    ipAddrStr++;
    while (*ipAddrStr)
    {
      if (isdigit(*ipAddrStr))
      {
        *port *= 10;
        *port += *ipAddrStr - '0';
      }
      else
      {
        break;
      }
      ipAddrStr++;
    }
  }

  if (dotCount == 3)
  {
    return CA_STATUS_OK;
  }
  return CA_STATUS_FAILED;
}

#else // not with_arduino
/*
 * These two conversion functions return void because errors can't happen
 * (because of NI_NUMERIC), and there's nothing to do if they do happen.
 */
void CAConvertAddrToName(const struct sockaddr_storage *sockAddr, socklen_t sockAddrLen,
                         char *host, uint16_t *port)
{
  VERIFY_NON_NULL_VOID(sockAddr, CA_ADAPTER_UTILS_TAG, "sockAddr is null");
  VERIFY_NON_NULL_VOID(host, CA_ADAPTER_UTILS_TAG, "host is null");
  VERIFY_NON_NULL_VOID(port, CA_ADAPTER_UTILS_TAG, "port is null");

  int r = getnameinfo((struct sockaddr *)sockAddr,
                      sockAddrLen,
                      host, MAX_ADDR_STR_SIZE_CA,
                      NULL, 0,
                      NI_NUMERICHOST | NI_NUMERICSERV);
  if (r)
  {
    if (EAI_SYSTEM == r)
    {
      OIC_LOG_V(ERROR, CA_ADAPTER_UTILS_TAG,
                "getnameinfo failed: errno %s", strerror(errno));
    }
    else
    {
      OIC_LOG_V(ERROR, CA_ADAPTER_UTILS_TAG,
                "getnameinfo failed: %s", gai_strerror(r));
    }
    return;
  }
  *port = ntohs(((struct sockaddr_in *)sockAddr)->sin_port); // IPv4 and IPv6
}

void CAConvertNameToAddr(const char *host, uint16_t port, struct sockaddr_storage *sockaddr)
{
  VERIFY_NON_NULL_VOID(host, CA_ADAPTER_UTILS_TAG, "host is null");
  VERIFY_NON_NULL_VOID(sockaddr, CA_ADAPTER_UTILS_TAG, "sockaddr is null");

  struct addrinfo *addrs;
  struct addrinfo hints = { .ai_family = AF_UNSPEC,
    .ai_socktype = SOCK_DGRAM,
     .ai_flags = AI_NUMERICHOST
  };

  int r = getaddrinfo(host, NULL, &hints, &addrs);
  if (r)
  {
    if (EAI_SYSTEM == r)
    {
      OIC_LOG_V(ERROR, CA_ADAPTER_UTILS_TAG,
                "getaddrinfo failed: errno %s", strerror(errno));
    }
    else
    {
      OIC_LOG_V(ERROR, CA_ADAPTER_UTILS_TAG,
                "getaddrinfo failed: %s", gai_strerror(r));
    }
    return;
  }
  // assumption: in this case, getaddrinfo will only return one addrinfo
  // or first is the one we want.
  if (addrs[0].ai_family == AF_INET6)
  {
    memcpy(sockaddr, addrs[0].ai_addr, sizeof(struct sockaddr_in6));
    ((struct sockaddr_in6 *)sockaddr)->sin6_port = htons(port);
  }
  else
  {
    memcpy(sockaddr, addrs[0].ai_addr, sizeof(struct sockaddr_in));
    ((struct sockaddr_in *)sockaddr)->sin_port = htons(port);
  }
  freeaddrinfo(addrs);
}
#endif // WITH_ARDUINO

#ifdef __ANDROID__
void CANativeJNISetContext(JNIEnv *env, jobject context)
{
  OIC_LOG_V(DEBUG, CA_ADAPTER_UTILS_TAG, "CANativeJNISetContext");

  if (!context)
  {
    OIC_LOG(ERROR, CA_ADAPTER_UTILS_TAG, "context is null");
    return;
  }

  if (!g_Context)
  {
    g_Context = (*env)->NewGlobalRef(env, context);
  }
  else
  {
    OIC_LOG(INFO, CA_ADAPTER_UTILS_TAG, "context is already set");
  }
}

void CANativeJNISetJavaVM(JavaVM *jvm)
{
  OIC_LOG_V(DEBUG, CA_ADAPTER_UTILS_TAG, "CANativeJNISetJavaVM");
  g_jvm = jvm;
}

jobject CANativeJNIGetContext()
{
  return g_Context;
}

JavaVM *CANativeJNIGetJavaVM()
{
  return g_jvm;
}

void CANativeSetActivity(JNIEnv *env, jobject activity)
{
  OIC_LOG_V(DEBUG, CA_ADAPTER_UTILS_TAG, "CANativeSetActivity");

  if (!activity)
  {
    OIC_LOG(ERROR, CA_ADAPTER_UTILS_TAG, "activity is null");
    return;
  }

  if (!g_Activity)
  {
    g_Activity = (*env)->NewGlobalRef(env, activity);
  }
  else
  {
    OIC_LOG(INFO, CA_ADAPTER_UTILS_TAG, "activity is already set");
  }
}

jobject *CANativeGetActivity()
{
  return g_Activity;
}

void CADeleteGlobalReferences(JNIEnv *env)
{
  if (g_Context)
  {
    (*env)->DeleteGlobalRef(env, g_Context);
    g_Context = NULL;
  }

  if (g_Activity)
  {
    (*env)->DeleteGlobalRef(env, g_Activity);
    g_Activity = NULL;
  }
}
#endif
