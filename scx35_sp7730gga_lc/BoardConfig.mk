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
-include device/sprd/scx35/nand/BoardConfigNand.mk

# nand fstab
TARGET_RECOVERY_FSTAB := device/sprd/scx35/nand/recovery.fstab

# board configs
TARGET_BOOTLOADER_BOARD_NAME := sp7730gga_lc
UBOOT_DEFCONFIG := sp7730gga_lc
ifeq ($(strip $(BOARD_KERNEL_SEPARATED_DT)),true)
KERNEL_DEFCONFIG := sp7730gga_lc-dt_defconfig
else
KERNEL_DEFCONFIG := sp7730gga_lc-native_defconfig
endif

TARGET_GPU_DFS_MAX_FREQ := 384000
TARGET_GPU_DFS_MIN_FREQ := 104000
USE_SPRD_HWCOMPOSER := false
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

#select mipi d-phy mode(none, phya, phyb, phyab)
TARGET_BOARD_FRONT_CAMERA_MIPI := none
TARGET_BOARD_BACK_CAMERA_MIPI := phyab

#select ccir pclk src(source0, source1)
TARGET_BOARD_FRONT_CAMERA_CCIR_PCLK := source0
TARGET_BOARD_BACK_CAMERA_CCIR_PCLK := source0

# select WCN
BOARD_HAVE_BLUETOOTH := true
BOARD_HAVE_BLUETOOTH_SPRD := true
BOARD_HAVE_FM_TROUT := true
BOARD_USE_SPRD_FMAPP := true

#2351 GPS
BOARD_USE_SPRD_4IN1_GPS := true

# WIFI configs
BOARD_WPA_SUPPLICANT_DRIVER := NL80211
WPA_SUPPLICANT_VERSION      := VER_2_1_DEVEL
BOARD_WPA_SUPPLICANT_PRIVATE_LIB := lib_driver_cmd_ittiam
BOARD_HOSTAPD_DRIVER        := NL80211
BOARD_HOSTAPD_PRIVATE_LIB   := lib_driver_cmd_ittiam
BOARD_WLAN_DEVICE           := ittiam
WIFI_DRIVER_FW_PATH_PARAM   := "/data/misc/wifi/fwpath"
WIFI_DRIVER_FW_PATH_STA     := "sta_mode"
WIFI_DRIVER_FW_PATH_P2P     := "p2p_mode"
WIFI_DRIVER_FW_PATH_AP      := "ap_mode"
WIFI_DRIVER_MODULE_PATH     := "/system/lib/modules/ittiam.ko"
WIFI_DRIVER_MODULE_NAME     := "ittiam"

# select sensor
#USE_INVENSENSE_LIB := true
USE_SPRD_SENSOR_LIB := true
BOARD_HAVE_ACC := Lis3dh
BOARD_ACC_INSTALL := 6
BOARD_HAVE_ORI := NULL
BOARD_ORI_INSTALL := NULL
BOARD_HAVE_PLS := LTR558ALS

# UBIFS partition layout
BOARD_FLASH_BLOCK_SIZE := 4096
TARGET_USERIMAGES_USE_UBIFS := true
BOARD_PAGE_SIZE := 4096
BOARD_SECT_SIZE := 4096
BOARD_BLOCK_SIZE := 262144
BOARD_ERASE_SIZE := 253952
BOARD_SYSTEMIMAGE_PARTITION_SIZE := 350000000
ifeq ($(STORAGE_INTERNAL), physical)
BOARD_USERDATAIMAGE_PARTITION_SIZE := 1500000000
else
BOARD_USERDATAIMAGE_PARTITION_SIZE := 300000000
endif
BOARD_CACHEIMAGE_PARTITION_SIZE := 10000000
BOARD_PRODNVIMAGE_PARTITION_SIZE := 10000000
BOARD_CACHEIMAGE_FILE_SYSTEM_TYPE := ubifs
BOARD_PRODNVIMAGE_FILE_SYSTEM_TYPE := ubifs

DEVICE_GSP_NOT_SCALING_UP_TWICE := true
