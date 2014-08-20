#
# Copyright (C) 2011 The Android Open-Source Project
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#      http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.
#

-include device/sprd/scx35/BoardConfigCommon.mk
-include device/sprd/scx35/emmc/BoardConfigEmmc.mk

# board configs
TARGET_BOOTLOADER_BOARD_NAME := sp8730sea
UBOOT_DEFCONFIG := sp8730sea
ifeq ($(strip $(BOARD_KERNEL_SEPARATED_DT)),true)
KERNEL_DEFCONFIG := sp8730sea-dt_defconfig
else
KERNEL_DEFCONFIG := sp8730sea-native_defconfig
endif

TARGET_GPU_DFS_MAX_FREQ := 384000
TARGET_GPU_DFS_MIN_FREQ := 104000

# select camera 2M,3M,5M,8M
CAMERA_SUPPORT_SIZE := 8M
FRONT_CAMERA_SUPPORT_SIZE := 3M
TARGET_BOARD_NO_FRONT_SENSOR := false
TARGET_BOARD_CAMERA_FLASH_CTRL := false

#face detect
TARGET_BOARD_CAMERA_FACE_DETECT := false

#snesor interface
TARGET_BOARD_BACK_CAMERA_INTERFACE := mipi
TARGET_BOARD_FRONT_CAMERA_INTERFACE := ccir

#select camera zsl cap mode
TARGET_BOARD_CAMERA_CAPTURE_MODE := false

#rotation capture
TARGET_BOARD_CAMERA_ROTATION_CAPTURE := true

#select continuous auto focus
TARGET_BOARD_CAMERA_CAF := true

#image angle in different project
TARGET_BOARD_CAMERA_ADAPTER_IMAGE := 0

#pre_allocate capture memory
TARGET_BOARD_CAMERA_PRE_ALLOC_CAPTURE_MEM := true

#select mipi d-phy mode(none, phya, phyb, phyab)
TARGET_BOARD_FRONT_CAMERA_MIPI := none
TARGET_BOARD_BACK_CAMERA_MIPI := phyab

#select ccir pclk src(source0, source1)
TARGET_BOARD_FRONT_CAMERA_CCIR_PCLK := source0
TARGET_BOARD_BACK_CAMERA_CCIR_PCLK := source0

# select WCN
BOARD_HAVE_BLUETOOTH := true
ifeq ($(strip $(USE_SPRD_WCN)),true)
BOARD_HAVE_BLUETOOTH_SPRD := true
BOARD_HAVE_FM_TROUT := true
BOARD_USE_SPRD_FMAPP := true
endif

#2351 GPS
BOARD_USE_SPRD_4IN1_GPS := true

# WIFI configs
BOARD_WPA_SUPPLICANT_DRIVER := NL80211
WPA_SUPPLICANT_VERSION      := VER_2_1_DEVEL
BOARD_WPA_SUPPLICANT_PRIVATE_LIB := lib_driver_cmd_sprdwl
BOARD_HOSTAPD_DRIVER        := NL80211
BOARD_HOSTAPD_PRIVATE_LIB   := lib_driver_cmd_sprdwl
BOARD_WLAN_DEVICE           := sprdwl
WIFI_DRIVER_FW_PATH_PARAM   := "/data/misc/wifi/fwpath"
WIFI_DRIVER_FW_PATH_STA     := "sta_mode"
WIFI_DRIVER_FW_PATH_P2P     := "p2p_mode"
WIFI_DRIVER_FW_PATH_AP      := "ap_mode"
WIFI_DRIVER_MODULE_PATH     := "/system/lib/modules/sprdwl.ko"
WIFI_DRIVER_MODULE_NAME     := "sprdwl"

# select sensor
#USE_INVENSENSE_LIB := true
USE_SPRD_SENSOR_LIB := true
BOARD_HAVE_ACC := Lis3dh
BOARD_ACC_INSTALL := 6
BOARD_HAVE_ORI := NULL
BOARD_ORI_INSTALL := NULL
BOARD_HAVE_PLS := LTR558ALS

# ext4 partition layout
TARGET_USERIMAGES_USE_EXT4 := true
BOARD_SYSTEMIMAGE_PARTITION_SIZE := 460000000
ifeq ($(STORAGE_INTERNAL), physical)
BOARD_USERDATAIMAGE_PARTITION_SIZE := 1500000000
else
BOARD_USERDATAIMAGE_PARTITION_SIZE := 3130000000
endif
BOARD_CACHEIMAGE_PARTITION_SIZE := 150000000
BOARD_PRODNVIMAGE_PARTITION_SIZE := 5242880
BOARD_FLASH_BLOCK_SIZE := 4096
BOARD_CACHEIMAGE_FILE_SYSTEM_TYPE := ext4
BOARD_PRODNVIMAGE_FILE_SYSTEM_TYPE := ext4

TARGET_SYSTEMIMAGES_SPARSE_EXT_DISABLED := true
TARGET_USERIMAGES_SPARSE_EXT_DISABLED := false
TARGET_CACHEIMAGES_SPARSE_EXT_DISABLED := false
TARGET_PRODNVIMAGES_SPARSE_EXT_DISABLED := true

DEVICE_GSP_NOT_SCALING_UP_TWICE := true

