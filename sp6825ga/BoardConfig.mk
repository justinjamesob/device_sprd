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

TARGET_CPU_ABI := armeabi-v7a
TARGET_CPU_ABI2 := armeabi
TARGET_CPU_SMP := true
TARGET_ARCH_VARIANT := armv7-a-neon
ARCH_ARM_HAVE_TLS_REGISTER := true

TARGET_BOARD_PLATFORM := sc8825
TARGET_BOOTLOADER_BOARD_NAME := sp6825ga

TARGET_VLX_ENABLE ?= true

# config u-boot
ifeq ($(TARGET_VLX_ENABLE), true)
UBOOT_DEFCONFIG := sp6825ga
else
UBOOT_DEFCONFIG := sp6825ga_native
endif

# config kernel
TARGET_NO_KERNEL := false
ifeq ($(TARGET_VLX_ENABLE), true)
KERNEL_DEFCONFIG := sp6825ga-vlx_defconfig
else
KERNEL_DEFCONFIG := sp6825ga-native_defconfig
endif
USES_UNCOMPRESSED_KERNEL := true
BOARD_KERNEL_BASE := 0x00000000
BOARD_KERNEL_CMDLINE := console=ttyS1,115200n8 mem=239M

# define for modem_control and nvitemd
BOARD_SP6825GA        := true

# use default init.rc
TARGET_PROVIDES_INIT_RC := true


USE_OPENGL_RENDERER := true
#USE_UI_OVERLAY := true
CAMERA_SUPPORT_SIZE := 3M

BOARD_HAVE_BLUETOOTH := true
BOARD_HAVE_FM_BEKEN := true
BOARD_USE_SPRD_FMAPP := true
# board specific modules
BOARD_USES_TINYALSA_AUDIO := true
BOARD_USES_LINE_CALL := false
BOARD_USE_VETH := true
BOARD_SPRD_RIL := true
BOARD_SAMSUNG_RIL := false
TARGET_RECOVERY_UI_LIB := librecovery_ui_sp8810
TARGET_RECOVERY_PIXEL_FORMAT := "RGBA_8888"





USE_CAMERA_STUB := true
TARGET_BOARD_CAMERA_ROTATION := true

BOARD_USES_GENERIC_AUDIO := false
AUDIO_MUX_PIPE := true
USE_BOOT_AT_DIAG := true
# sensor config
USE_INVENSENSE_LIB := NULL
USE_SPRD_SENSOR_LIB := true
BOARD_HAVE_ACC := Lis3dh
BOARD_ACC_INSTALL := 5
BOARD_HAVE_ORI := Akm
BOARD_ORI_INSTALL := 7
BOARD_HAVE_PLS := LTR558ALS
TARGET_BOARD_BACK_CAMERA_ROTATION := false
TARGET_BOARD_FRONT_CAMERA_ROTATION := true
TARGET_BOARD_BACK_CAMERA_INTERFACE := mipi
TARGET_BOARD_FRONT_CAMERA_INTERFACE := ccir
TARGET_BOARD_CAMERA_SUPPORT_CIF := true

#BOARD_WPA_SUPPLICANT_DRIVER := NL80211
#WPA_SUPPLICANT_VERSION     := VER_0_8_X
#BOARD_WPA_SUPPLICANT_PRIVATE_LIB := lib_driver_cmd_rtl
#BOARD_HOSTAPD_DRIVER        := NL80211
#BOARD_HOSTAPD_PRIVATE_LIB   :=  lib_driver_cmd_rtl
#BOARD_WLAN_DEVICE           := realtek

#WIFI_DRIVER_FW_PATH_PARAM   := ""
#WIFI_DRIVER_FW_PATH_STA     := "/etc/wifi/fw_rtk_sta.bin"
#WIFI_DRIVER_FW_PATH_P2P     := "/etc/wifi/fw_rtk_p2p.bin"
#WIFI_DRIVER_FW_PATH_AP      := "/etc/wifi/fw_rtk_ap.bin"
#WIFI_DRIVER_MODULE_NAME := 8723as
#WIFI_DRIVER_MODULE_PATH := "/system/lib/modules/8723as.ko"

CODEC_ALLOC_FROM_PMEM:=false
BOARD_GPS := ublox

PRODUCT_AAPT_CONFIG += xhdpi hdpi
BOARD_USE_SPRD_FMAPP := true
