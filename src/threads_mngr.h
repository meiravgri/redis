/*
 * Copyright (c) 2009-2012, Salvatore Sanfilippo <antirez at gmail dot com>
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 *   * Redistributions of source code must retain the above copyright notice,
 *     this list of conditions and the following disclaimer.
 *   * Redistributions in binary form must reproduce the above copyright
 *     notice, this list of conditions and the following disclaimer in the
 *     documentation and/or other materials provided with the distribution.
 *   * Neither the name of Redis nor the names of its contributors may be used
 *     to endorse or promote products derived from this software without
 *     specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 */

#pragma once
// TODO: if linux!!!
#include "fmacros.h"

#include <sys/types.h>
#include <unistd.h>
#define THREADS_SIGNAL SIGUSR2

/* Callback signature */
typedef void*(*run_on_thread_cb)(void);

/* Register the process to THREADS_SIGNAL */
void ThreadsManager_init(void);

/** @brief Invoke callback by each thread in tids.
 *
 * @param tids  An array of threads that need to invoke callback.
 * @param tids_len The number of threads in @param tids.
 * @param callback A callback to be invoked by each thread in @param tids.
 *
 * NOTES:
 * It is assumed that all the threads don't block or ignore THREADS_SIGNAL.
 *
 * It is safe to include the calling thread in @param tids. However, be aware that subsequent tids will
 * not be signaled until the calling thread returns from the callback invocation.
 * Hence, it is recommended to place the calling thread last in @param tids.
 *
 * The function returns only when @param tids_len threads have returned from @param callback.
 *
 * @return NULL If ThreadsManager_runOnThreads is already in the middle of execution.
 * Otherwise, it returns an array of the threads return value from @param callback.
 * NOTES:
 * The indices of the outputs in the output array are NOT associated with the threads indices in @param tids.
 *
 * The returned array length will be @param tids_len, but it might include NULLs if the
 * invocation of @param callback was unsuccessful.
 *
 * The output array should be freed by the caller by calling zfree().
**/

void **ThreadsManager_runOnThreads(pid_t *tids, size_t tids_len, run_on_thread_cb callback);
