LOCAL_PATH:= $(call my-dir)


include $(CLEAR_VARS)

LOCAL_MODULE_TAGS := optional
LOCAL_MODULE := ES_File_Explorer.apk
LOCAL_MODULE_CLASS := APPS
LOCAL_CERTIFICATE := PRESIGNED
LOCAL_MODULE_PATH := $(TARGET_OUT)/preloadapp
LOCAL_SRC_FILES := ES_File_Explorer_V1_6_2_5_Spreadtrum_cmcc_20130306.apk

include $(BUILD_PREBUILT)

include $(CLEAR_VARS)

#LOCAL_NO_STANDARD_LIBRARIES := true
#LOCAL_PACKAGE_NAME := framework-se-res
LOCAL_MODULE :=framework-se-res.apk
LOCAL_CERTIFICATE := platform
LOCAL_MODULE_CLASS := APPS
LOCAL_MODULE_TAGS := optional

# Install this alongside the libraries.
LOCAL_MODULE_PATH := $(TARGET_OUT_JAVA_LIBRARIES)
LOCAL_EXPORT_PACKAGE_RESOURCES := true

# Include resources generated by system RenderScript files.
LOCAL_SRC_FILES := secu_resource.apk
include $(BUILD_PREBUILT)


#include $(CLEAR_VARS)

#LOCAL_MODULE_TAGS := optional
#LOCAL_MODULE := SecondClock.apk
#LOCAL_MODULE_CLASS := APPS
#LOCAL_CERTIFICATE := PRESIGNED
#LOCAL_MODULE_PATH := $(TARGET_OUT)/preloadapp
#LOCAL_SRC_FILES := Stopwatch_serverV1.0.0.7.apk

#include $(BUILD_PREBUILT)

#include $(CLEAR_VARS)

#LOCAL_MODULE_TAGS := optional
#LOCAL_MODULE := WorldCLOCk.apk
#LOCAL_MODULE_CLASS := APPS
#LOCAL_CERTIFICATE := PRESIGNED
#LOCAL_MODULE_PATH := $(TARGET_OUT)/preloadapp
#LOCAL_SRC_FILES := WorldClock1105-Rev1.3.6_withAds.apk

#include $(BUILD_PREBUILT)

#include $(CLEAR_VARS)

#LOCAL_MODULE_TAGS := optional
#LOCAL_MODULE := Operamini.apk
#LOCAL_MODULE_CLASS := APPS
#LOCAL_CERTIFICATE := PRESIGNED
#LOCAL_MODULE_PATH := $(TARGET_OUT)/preloadapp
#LOCAL_SRC_FILES := Operamini.apk

#include $(BUILD_PREBUILT)


#include $(CLEAR_VARS)

#LOCAL_MODULE_TAGS := optional
#LOCAL_MODULE := libom.so
#LOCAL_MODULE_CLASS := SHARED_LIBRARIES
#LOCAL_SRC_FILES := libom.so

#include $(BUILD_PREBUILT)

#include $(CLEAR_VARS)

#LOCAL_MODULE_TAGS := optional
#LOCAL_MODULE := FMPlayer.apk
#LOCAL_MODULE_CLASS := APPS
#LOCAL_CERTIFICATE := PRESIGNED
#LOCAL_SRC_FILES := FMPlayer.apk 

#include $(BUILD_PREBUILT)


#include $(CLEAR_VARS)

#LOCAL_MODULE_TAGS := optional
#LOCAL_MODULE := FaceBook.apk
#LOCAL_MODULE_CLASS := APPS
#LOCAL_CERTIFICATE := PRESIGNED
#LOCAL_SRC_FILES := FBAndroid-1.9.7-preload.apk 

#include $(BUILD_PREBUILT)


include $(CLEAR_VARS)

LOCAL_MODULE_TAGS := optional
LOCAL_MODULE := Permission.apk
LOCAL_MODULE_CLASS := APPS
LOCAL_CERTIFICATE := PRESIGNED
LOCAL_SRC_FILES := Permission.apk 

include $(BUILD_PREBUILT)

