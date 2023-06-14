/*
 * nghttp2 - HTTP/2 C Library
 *
 * Copyright (c) 2014 Tatsuhiro Tsujikawa
 *
 * Permission is hereby granted, free of charge, to any person obtaining
 * a copy of this software and associated documentation files (the
 * "Software"), to deal in the Software without restriction, including
 * without limitation the rights to use, copy, modify, merge, publish,
 * distribute, sublicense, and/or sell copies of the Software, and to
 * permit persons to whom the Software is furnished to do so, subject to
 * the following conditions:
 *
 * The above copyright notice and this permission notice shall be
 * included in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 * NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE
 * LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION
 * OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION
 * WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 */
#include "nghttp2_mem.h"
#include "osdep_service.h"

static void *default_malloc(size_t size, void *mem_user_data) {
  (void)mem_user_data;

  return malloc(size);
}

static void default_free(void *ptr, void *mem_user_data) {
  (void)mem_user_data;

  free(ptr);
}
extern void *memset(void * dst0, int Val, size_t length);
static void *default_calloc(size_t nmemb, size_t size, void *mem_user_data) {
  (void)mem_user_data;

  void *ptr;
  ptr = malloc(nmemb * size);
  if (ptr == NULL) {
  	return NULL;
  } else {
    memset(ptr, 0, nmemb * size);
	return ptr;
  }
}

static void *default_realloc(void *ptr, size_t size, void *mem_user_data) {
  (void)mem_user_data;

  return realloc(ptr, size);
}

static nghttp2_mem mem_default = {NULL, default_malloc, default_free,
                                  default_calloc, default_realloc};

nghttp2_mem *nghttp2_mem_default(void) { return &mem_default; }

void *nghttp2_mem_malloc(nghttp2_mem *mem, size_t size) {
  return mem->_malloc(size, mem->mem_user_data);
}

void nghttp2_mem_free(nghttp2_mem *mem, void *ptr) {
  mem->_free(ptr, mem->mem_user_data);
}

void nghttp2_mem_free2(nghttp2_free free_func, void *ptr, void *mem_user_data) {
  free_func(ptr, mem_user_data);
}

void *nghttp2_mem_calloc(nghttp2_mem *mem, size_t nmemb, size_t size) {
  return mem->_calloc(nmemb, size, mem->mem_user_data);
}

void *nghttp2_mem_realloc(nghttp2_mem *mem, void *ptr, size_t size) {
  return mem->_realloc(ptr, size, mem->mem_user_data);
}
