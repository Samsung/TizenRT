/****************************************************************************
 *
 * Copyright 2018 Samsung Electronics All Rights Reserved.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing,
 * software distributed under the License is distributed on an
 * "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND,
 * either express or implied. See the License for the specific
 * language governing permissions and limitations under the License.
 *
 ****************************************************************************/
#ifndef SUPPORT_ASSERT_CHECKPOINT_H
#define SUPPORT_ASSERT_CHECKPOINT_H

#include <csignal>
#include <iostream>
#include <cstdlib>

struct Checkpoint {
  const char* file;
  const char* func;
  int line;
  const char* msg;

  Checkpoint() : file(nullptr), func(nullptr), line(-1), msg(nullptr) {}
  Checkpoint(const char* xfile, const char* xfunc, int xline, const char* xmsg)
      : file(xfile), func(xfunc), line(xline), msg(xmsg)
  {}

  template <class Stream>
  void print(Stream& s) const {
      if (!file) {
          s << "NO CHECKPOINT\n";
          return;
      }
      s << file << ":" << line << " " << func << ": Checkpoint";
      if (msg)
        s << " '" << msg << "'";
      s << std::endl;
  }
};

inline Checkpoint& globalCheckpoint() {
    static Checkpoint C;
    return C;
}

inline void clearCheckpoint() {
    globalCheckpoint() = Checkpoint();
}

#if defined(__GNUC__)
#define CHECKPOINT_FUNCTION_NAME __PRETTY_FUNCTION__
#else
#define CHECKPOINT_FUNCTION_NAME __func__
#endif

#define CHECKPOINT(msg) globalCheckpoint() = Checkpoint(__FILE__, CHECKPOINT_FUNCTION_NAME, __LINE__, msg);

inline void checkpointSignalHandler(int signal) {
    if (signal == SIGABRT) {
        globalCheckpoint().print(std::cerr);
    } else {
        std::cerr << "Unexpected signal " << signal << " received\n";
    }
    std::_Exit(EXIT_FAILURE);
}

inline bool initCheckpointHandler() {
    typedef void(*HandlerT)(int);
    static bool isInit = false;
    if (isInit) return true;
    HandlerT prev_h = std::signal(SIGABRT, checkpointSignalHandler);
    if (prev_h == SIG_ERR) {
        std::cerr << "Setup failed.\n";
        std::_Exit(EXIT_FAILURE);
    }
    isInit = true;
    return false;
}

static bool initDummy = initCheckpointHandler();

#endif
