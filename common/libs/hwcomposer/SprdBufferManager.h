/*
 * Copyright (C) 2010 The Android Open Source Project
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
 */


/******************************************************************************
 **                   Edit    History                                         *
 **---------------------------------------------------------------------------*
 ** DATE          Module              DESCRIPTION                             *
 ** 22/09/2013    Hardware Composer   Responsible for processing some         *
 **                                   Hardware layers. These layers comply    *
 **                                   with display controller specification,  *
 **                                   can be displayed directly, bypass       *
 **                                   SurfaceFligner composition. It will     *
 **                                   improve system performance.             *
 ******************************************************************************
 ** File: SprdBufferManager.h         DESCRIPTION                             *
 **                                   Manage HWC buffer allocation and free.  *
 ******************************************************************************
 ******************************************************************************
 ** Author:         zhongjun.chen@spreadtrum.com                              *
 *****************************************************************************/

#ifndef _SPRD_BUFFER_MANAGER_H_
#define _SPRD_BUFFER_MANAGER_H_

#include <sys/ioctl.h>
#include <sys/mman.h>
#include <pthread.h>
#include <cutils/log.h>
#include <cutils/atomic.h>
#include <utils/RefBase.h>
#include <utils/List.h>

#include <binder/MemoryHeapIon.h>
#include "ion_sprd.h"
#include "gralloc_priv.h"

using namespace android;

/*
 *  ION buffer information, including contiguous physcial address,
 *  virtual address and so on.
 * */
struct bufferInfo {
    unsigned int width;
    unsigned int height;
    int format;
    unsigned int bufSize;
    int phyAddr;
    void *virAddr;

    sp<MemoryHeapIon> IonHeap;
};

/*
 *  A mutex lock object, for protecting ION buffer alloc and free.
 * */
class MutexLock
{
public:
    MutexLock();
    ~MutexLock();

    void Lock();
    void UnLock();

private:
    mutable pthread_mutex_t mMutex;
};

#define SPRD_ION_DEV "/dev/ion"

#define PRIVATE_HALF_BUFFER 0x8000

/*
 *  Manage ION buffer alloc and free
 * */
class SprdBufferManager
{
public:
    SprdBufferManager();

    ~SprdBufferManager();

    void *alloc(unsigned int width, unsigned int height, int format);
    void free(void *bufHandle);

private:
    MutexLock mLock;
    typedef List<struct bufferInfo *> BufferList;
    BufferList mList;

    void Lock();
    void UnLock();

    inline unsigned int RoundUpToPageSize(unsigned int x)
    {
        return ((x + (PAGE_SIZE - 1)) & ~(PAGE_SIZE - 1));
    }

    unsigned int CalculateBufferSize(unsigned int width, unsigned int height, int format);

};


#endif
