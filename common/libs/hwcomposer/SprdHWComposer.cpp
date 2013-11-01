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
 ** File: SprdHWComposer.h            DESCRIPTION                             *
 **                                   comunicate with SurfaceFlinger and      *
 **                                   other class objects of HWComposer       *
 ******************************************************************************
 ******************************************************************************
 ** Author:         zhongjun.chen@spreadtrum.com                              *
 *****************************************************************************/

#include "SprdHWComposer.h"


using namespace android;

bool SprdHWComposer:: Init()
{
    FrameBufferInfo *fbInfo = getFrameBufferInfo();
    if (fbInfo == NULL) {
        ALOGE("Can NOT get FrameBuffer info");
        return false;
    }

    mBufferManager = new SprdBufferManager();
    if (mBufferManager == NULL) {
        ALOGE("Can NOT create SprdBufferManager object");
        return false;
    }

    mLayerList = new SprdHWLayerList(fbInfo->fb_width, fbInfo->fb_height);
    if (mLayerList == NULL) {
        ALOGE("Can NOT create SprdHWLayerList object");
        return false;
    }

    mOverlayPlane = new SprdOverlayPlane(fbInfo, getBufferManager());
    if (mOverlayPlane == NULL) {
        ALOGE("Cant NOT create SprdOverlayPlane object");
        return false;
    }

    mPrimaryPlane = new SprdPrimaryPlane(fbInfo, getBufferManager());
    if (mPrimaryPlane == NULL) {
        ALOGE("Can NOT create SprdPrimaryPlane object");
        return false;
    }

#ifdef OVERLAY_COMPOSER_GPU
    mOverlayComposer = new OverlayComposer(mPrimaryPlane);
    if (mOverlayComposer == NULL)
    {
        ALOGE("Cannot get OverlayComposer object");
        return false;
    }
#endif

    mVsyncEvent = new SprdVsyncEvent();
    if (mVsyncEvent == NULL) {
        ALOGE("Can NOT create SprdVsyncEvent object");
        return false;
    }

    mUtil = new SprdUtil(mBufferManager, fbInfo);
    if (mUtil == NULL) {
        ALOGE("Can NOT create SprdUtil object");
        return false;
    }

    mInitFlag = 1;

    return true;
}

SprdHWComposer:: ~SprdHWComposer()
{
    eventControl(0);

    if (mVsyncEvent != NULL)
    {
        mVsyncEvent->requestExitAndWait();
    }

    if (mUtil) {
        delete mUtil;
        mUtil = NULL;
    }

    if (mPrimaryPlane) {
        delete mPrimaryPlane;
        mPrimaryPlane = NULL;
    }

    if (mOverlayPlane) {
        delete mOverlayPlane;
        mOverlayPlane = NULL;
    }

    if (mLayerList) {
        delete mLayerList;
        mLayerList = NULL;
    }

    if (mBufferManager) {
        delete mBufferManager;
        mBufferManager = NULL;
    }

    freeFrameBufferInfo();

    mInitFlag = 0;
}

FrameBufferInfo *SprdHWComposer:: getFrameBufferInfo()
{
    char const * const deviceTemplate[] = {
        "/dev/graphics/fb%u",
        "/dev/fb%u",
        NULL
    };

    int fd = -1;
    int i = 0;
    char name[64];
    void *vaddr;

    struct fb_fix_screeninfo finfo;
    struct fb_var_screeninfo vinfo;
    size_t fbSize = 0;
    uint32_t bytespp = 0;

    while ((fd == -1) && deviceTemplate[i])
    {
        snprintf(name, 64, deviceTemplate[i], 0);
        fd = open(name, O_RDWR, 0);
        i++;
    }

    if (fd < 0)
    {
        ALOGE("fail to open fb");
        return NULL;
    }

    if (ioctl(fd, FBIOGET_FSCREENINFO, &finfo) == -1)
    {
        ALOGE("fail to get FBIOGET_FSCREENINFO");
        close(fd);
        return NULL;
    }

    if (ioctl(fd, FBIOGET_VSCREENINFO, &vinfo) == -1)
    {
        ALOGE("fail to get FBIOGET_VSCREENINFO");
        return NULL;
    }

    fbSize = roundUpToPageSize(finfo.line_length * vinfo.yres_virtual);
    vaddr = mmap(0, fbSize, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
    if (vaddr == MAP_FAILED)
    {
        ALOGE("Error mapping the framebuffer (%s)", strerror(errno));
        close(fd);
        return NULL;
    }


    /*
     * Store the FrameBuffer info
     * */
    FrameBufferInfo *FBInfo = (FrameBufferInfo *)malloc(sizeof(FrameBufferInfo));
    if (FBInfo == NULL)
    {
        ALOGE("Cannot malloc the FrameBufferInfo, no MEM");
        close(fd);
        return NULL;
    }

    FBInfo->fbfd = fd;
    FBInfo->fb_width = vinfo.xres;
    FBInfo->fb_height = vinfo.yres;
    FBInfo->fb_virt_addr = vaddr;

    switch(vinfo.bits_per_pixel) {
    case 16:
        bytespp = 2;
        FBInfo->format = HAL_PIXEL_FORMAT_RGB_565;
        break;
    case 24:
        bytespp = 3;
        FBInfo->format = HAL_PIXEL_FORMAT_RGB_888;
        break;
    case 32:
        bytespp = 4;
        FBInfo->format = HAL_PIXEL_FORMAT_RGBA_8888;
        break;
    default:
        ALOGE_IF(mDebugFlag, "fail to getFrameBufferInfo not support bits per pixel:%d" , vinfo.bits_per_pixel);
        return NULL;
    }

    if(vinfo.yoffset == vinfo.yres)
    { //flushing the second buffer.
        FBInfo->pFrontAddr = (char*)((unsigned int)FBInfo->fb_virt_addr + vinfo.xres * vinfo.yres * bytespp);
        FBInfo->pBackAddr  = (char *)(FBInfo->fb_virt_addr);
    }
    else if(vinfo.yoffset == 0)
    { //flushing the first buffer.
        FBInfo->pFrontAddr = (char *)(FBInfo->fb_virt_addr);
        FBInfo->pBackAddr = (char*)((unsigned int)FBInfo->fb_virt_addr + vinfo.xres * vinfo.yres * bytespp);
    }
    else
    {
        ALOGE_IF(mDebugFlag, "fail to getFrameBufferInfo");
    }

    mFBInfo = FBInfo;

    return FBInfo;
}

void SprdHWComposer:: freeFrameBufferInfo()
{

    if (mFBInfo)
    {
       close(mFBInfo->fbfd);
       free(mFBInfo);
       mFBInfo = NULL;
    }

}

bool SprdHWComposer:: PostFrameBufferOnline()
{
    if (mForceOverlayFlag)
    {
        mPostFrameBuffer = false;
    }

    return mPostFrameBuffer;
}

int SprdHWComposer:: prepare(hwc_layer_list_t *list)
{
    bool ret = false;
    mForceOverlayFlag = false;

    queryDebugFlag(&mDebugFlag);

    ALOGI_IF(mDebugFlag, "HWC start prepare");

    if (list == NULL)
    {
        ALOGE("The input parameters list is NULl");
        return -1;
    }

    ret = mLayerList->updateGeometry(list);
    if (ret == false)
    {
        ALOGE("(FILE:%s, line:%d, func:%s) updateGeometry failed",
              __FILE__, __LINE__, __func__);
        return -1;
    }

    ret = mLayerList->revistGeometry(&mForceOverlayFlag);
    if (ret == false)
    {
        ALOGE("(FILE:%s, line:%d, func:%s) revistGeometry failed",
              __FILE__, __LINE__, __func__);
        return -1;
    }

    return 0;
}

int SprdHWComposer:: commit(hwc_display_t dpy, hwc_surface_t sur, hwc_layer_list_t* list)
{
    bool ret = false;
    bool DirectDisplayFlag = false;
    mPostFrameBuffer = false;
    SprdHWLayer *OverlayLayer = NULL;
    SprdHWLayer *PrimaryLayer = NULL;

    if (dpy == NULL && sur == NULL && list == NULL)
    {
        /*
         * release our resources, the screen is turning off
         * in our case, there is nothing to do.
         * */
         return 0;
    }

    if (list == NULL)
    {
        ALOGW("commit list is NULL");

        if (NULL == sur)
        {
            return HWC_EGL_ERROR;
        }

        EGLBoolean sucess = eglSwapBuffers((EGLDisplay)dpy, (EGLSurface)sur);

        if (!sucess)
        {
            return HWC_EGL_ERROR;
        }

        return 0;
    }

    ALOGI_IF(mDebugFlag, "HWC start commit");

    for (size_t i = 0; i < list->numHwLayers; i++)
    {
        hwc_layer_t *AndroidLayer = &(list->hwLayers[i]);

        if (AndroidLayer && AndroidLayer->compositionType == HWC_OVERLAY)
        {
            if (mLayerList->getPlaneType(i) == PLANE_OVERLAY)
            {
                OverlayLayer = mLayerList->getSprdLayer(i);

                OverlayLayer->setAndroidLayer(AndroidLayer);

                mOverlayPlane->UpdateOverlayLayer(OverlayLayer);
            }
            else if (mLayerList->getPlaneType(i) == PLANE_PRIMARY)
            {
                PrimaryLayer = mLayerList->getSprdLayer(i);

                PrimaryLayer->setAndroidLayer(AndroidLayer);

                mPrimaryPlane->UpdatePrimaryLayer(PrimaryLayer);

                if ( mForceOverlayFlag == false && OverlayLayer == NULL)
                {
                    mPrimaryPlane->SetDirectDisplay(AndroidLayer);
                }
            }
        }
        else
        {
            mPostFrameBuffer = true;
        }

        AndroidLayer = NULL;
    }

#ifdef OVERLAY_COMPOSER_GPU
    if (mForceOverlayFlag)
    {
        ALOGI_IF(mDebugFlag, "Start OverlayComposer composition misson");
        mOverlayComposer->onComposer(list);
    }
#endif

    if (mForceOverlayFlag == false &&
        (OverlayLayer != NULL || PrimaryLayer != NULL))
    {
        struct bufferInfo *buffer1 = NULL;
        struct bufferInfo *buffer2 = NULL;

        if (OverlayLayer != NULL)
        {
            mOverlayPlane->dequeueBuffer();

            buffer1 = mOverlayPlane->getPlaneBuffer();
        }

        if (PrimaryLayer != NULL)
        {
            mPrimaryPlane->dequeueBuffer();

            buffer2 = mPrimaryPlane->getPlaneBuffer();

            DirectDisplayFlag = mPrimaryPlane->GetDirectDisplay();
        }


        if (DirectDisplayFlag == false)
        {
#ifdef TRANSFORM_USE_DCAM
            mUtil->transformLayer(OverlayLayer, PrimaryLayer, buffer1, buffer2);
#endif

#ifdef PROCESS_VIDEO_USE_GSP
            mUtil->composerLayers(OverlayLayer, PrimaryLayer, buffer1, buffer2);
        }

        if (OverlayLayer != NULL)
        {
            mPrimaryPlane->disable();
        }
#endif

        buffer1 = NULL;
        buffer2 = NULL;

        if (mOverlayPlane->online())
        {
            mOverlayPlane->queueBuffer();
        }

        if (mPrimaryPlane->online())
        {
            mPrimaryPlane->queueBuffer();
        }


        if (OverlayLayer || PrimaryLayer)
        {
            mPrimaryPlane->display(OverlayLayer, PrimaryLayer);
        }
    }


#ifdef OVERLAY_COMPOSER_GPU
    if (mForceOverlayFlag)
    {
        ALOGI_IF(mDebugFlag, "Start OverlayComposer display misson");
        mOverlayComposer->onDisplay();
    }
#endif

    if (PostFrameBufferOnline())
    {
        ALOGI_IF(mDebugFlag, "eglSwapBuffers");
        EGLBoolean sucess = eglSwapBuffers((EGLDisplay)dpy, (EGLSurface)sur);
        if (!sucess)
        {
            return HWC_EGL_ERROR;
        }

        queryDumpFlag(&mDumpFlag);

        if (mDumpFlag)
        {
            dumpFrameBuffer(mFBInfo->pFrontAddr, "FrameBuffer", mFBInfo->fb_width, mFBInfo->fb_height, mFBInfo->format, mDumpFlag);
        }
    }

    OverlayLayer = NULL;
    PrimaryLayer = NULL;

    mForceOverlayFlag = false;
    DirectDisplayFlag = false;

    return 0;
}

void SprdHWComposer:: registerProcs(hwc_procs_t const* procs)
{
    sp<SprdVsyncEvent> VE = getVsyncEventHandle();
    if (VE == NULL)
    {
        ALOGE("getVsyncEventHandle failed");
        return;
    }

    VE->setVsyncEventProcs(const_cast<hwc_procs_t *>(procs));
}

bool SprdHWComposer:: eventControl(int enabled)
{
    sp<SprdVsyncEvent> VE = getVsyncEventHandle();
    if (VE == NULL)
    {
        ALOGE("getVsyncEventHandle failed");
        return false;
    }

    VE->setEnabled(enabled);

    return true;
}





/*  
 *  HWC module info
 * */

static int hwc_eventControl(hwc_composer_device_t* dev, int event, int enabled)
{
    int status = -EINVAL;
    bool ret = false;

    SprdHWComposer *HWC = static_cast<SprdHWComposer*>(dev);
    if (HWC == NULL)
    {
        ALOGE("Can NOT get SprdHWComposer reference");
        return status;
    }

    switch(event) 
    {
        case HWC_EVENT_VSYNC:
            ret = HWC->eventControl(enabled);
            if (!ret)
            {
                ALOGE("Vsync event control failed");
                status = -EPERM;
                return status;
            }
            break;
            
        default:
            ALOGE("unsupported event");
            status = -EPERM;
            return status;
    }

    return 0;
}

static hwc_methods_t hwc_device_methods = {
    eventControl: hwc_eventControl
};

static int hwc_prepare(hwc_composer_device_t *dev, hwc_layer_list_t *list)
{
    int status = -EINVAL;

    SprdHWComposer *HWC = static_cast<SprdHWComposer*>(dev);
    if (HWC == NULL)
    {
        ALOGE("Can NOT get SprdHWComposer reference");
        return status;
    }

    status = HWC->prepare(list);
    
    return status;
}

static int hwc_set(hwc_composer_device_t *dev, hwc_display_t dpy, 
               hwc_surface_t sur, hwc_layer_list_t* list)
{
    int status = -EINVAL;

    SprdHWComposer *HWC = static_cast<SprdHWComposer*>(dev);
    if (HWC == NULL)
    {
        ALOGE("Can NOT get SprdHWComposer reference");
        return status;
    }

    status = HWC->commit(dpy, sur, list);

    return status;
}

static int hwc_device_close(struct hw_device_t *dev)
{
    SprdHWComposer *HWC = (SprdHWComposer*)(dev);
    if (HWC != NULL)
    {
        delete HWC;
        HWC = NULL;
    }

    return 0;
};

static void hwc_registerProcs(hwc_composer_device_t* dev, hwc_procs_t const* procs)
{
    SprdHWComposer *HWC = static_cast<SprdHWComposer*>(dev);
    if (HWC == NULL)
    {
        ALOGE("Can NOT get SprdHWComposer reference");
        return;
    }

    HWC->registerProcs(procs);
}

static int hwc_device_open(const struct hw_module_t* module, const char* name, struct hw_device_t** device)
{
    int status = -EINVAL;

    if (strcmp(name, HWC_HARDWARE_COMPOSER)) 
    {
        ALOGE("The module name is not HWC_HARDWARE_COMPOSER");
        return status;
    }

    SprdHWComposer *HWC = new SprdHWComposer();
    if (HWC == NULL) {
        ALOGE("Can NOT create SprdHWComposer object");
        status = -ENOMEM;
        return status;
    }

    bool ret = HWC->Init();
    if (!ret)
    {
        ALOGE("Init HWComposer failed");
        status = -ENOMEM;
        return status;
    }

    HWC->hwc_composer_device_t::common.tag = HARDWARE_DEVICE_TAG;
    HWC->hwc_composer_device_t::common.version = HWC_DEVICE_API_VERSION_0_3;
    HWC->hwc_composer_device_t::common.module = const_cast<hw_module_t*>(module);
    HWC->hwc_composer_device_t::common.close = hwc_device_close;

    HWC->hwc_composer_device_t::prepare = hwc_prepare;
    HWC->hwc_composer_device_t::set = hwc_set;
    HWC->hwc_composer_device_t::registerProcs = hwc_registerProcs;
    HWC->hwc_composer_device_t::methods = &hwc_device_methods;

    *device = &HWC->hwc_composer_device_t::common;

    status = 0;

    return status;
}

static struct hw_module_methods_t hwc_module_methods = {
    open: hwc_device_open
};

hwc_module_t HAL_MODULE_INFO_SYM = {
    common: {
        tag: HARDWARE_MODULE_TAG,
        version_major: 2,
        version_minor: 0,
        id: HWC_HARDWARE_MODULE_ID,
        name: "SPRD hwcomposer module",
        author: "The Android Open Source Project",
        methods: &hwc_module_methods,
    }
};
