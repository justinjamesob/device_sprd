ifneq ($(TARGET_SIMULATOR),true)
LOCAL_PATH:= $(call my-dir)
include $(CLEAR_VARS)

LOCAL_MODULE_PATH := $(TARGET_ROOT_OUT_BIN)
LOCAL_UNSTRIPPED_PATH := $(TARGET_ROOT_OUT_BIN_UNSTRIPPED)

LOCAL_C_INCLUDES    +=  $(LOCAL_PATH) \
			$(LOCAL_PATH)/$(BOARD_PRODUCT_NAME)/ \
			$(LOCAL_PATH)/common \
			device/sprd/common/apps/engineeringmodel/engcs
LOCAL_SRC_FILES:= \
        apnv_server.c \
	apnv_api.c

LOCAL_SHARED_LIBRARIES := \
    libhardware_legacy \
    libc \
    libutils \
    libengclient

LOCAL_MODULE := apnvd
LOCAL_MODULE_TAGS := optional
CAL_MODULE_PATH:=$(TARGET_ROOT_OUT)
include $(BUILD_EXECUTABLE)


#socket
CAL_PATH:= $(call my-dir)
include $(CLEAR_VARS)

LOCAL_C_INCLUDES    +=  $(LOCAL_PATH) \
			$(LOCAL_PATH)/$(BOARD_PRODUCT_NAME)/ \
			$(LOCAL_PATH)/common \
			device/sprd/common/apps/engineeringmodel/engcs
LOCAL_SRC_FILES:= apnv_socket.c
LOCAL_SHARED_LIBRARIES := \
    libhardware_legacy \
    libc \
    libutils \
    libengclient
LOCAL_MODULE := libapnvsocket
LOCAL_MODULE_TAGS := optional
LOCAL_COPY_HEADERS_TO := libapnvsocket
LOCAL_COPY_HEADERS := apnv_socket.h

include $(BUILD_STATIC_LIBRARY)
endif
