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
TARGET_ARCH_VARIANT := armv7-a-neon
ARCH_ARM_HAVE_TLS_REGISTER := true

TARGET_BOARD_PLATFORM := sc8810
TARGET_BOOTLOADER_BOARD_NAME := sp7702

TARGET_VLX_ENABLE ?= false

# config u-boot
TARGET_NO_BOOTLOADER := false
ifeq ($(TARGET_VLX_ENABLE), true)
UBOOT_DEFCONFIG := sp7702
else
UBOOT_DEFCONFIG := sp7702_native
endif

# config kernel
TARGET_NO_KERNEL := false
ifeq ($(TARGET_VLX_ENABLE), true)
KERNEL_DEFCONFIG := sp7702-vlx_defconfig
else
KERNEL_DEFCONFIG := sp7702-native_4p1_defconfig
endif
USES_UNCOMPRESSED_KERNEL := true
BOARD_KERNEL_BASE := 0x00000000
BOARD_KERNEL_CMDLINE := console=ttyS1,115200n8


# define for modem_control and nvitemd
BOARD_7702_STINRAY	:= true

# use default init.rc
TARGET_PROVIDES_INIT_RC := true

BOARD_HAVE_BLUETOOTH := true
BOARD_HAVE_BLUETOOTH_BCM := true
BOARD_HAVE_FM_BCM := true
BOARD_USE_SPRD_FMAPP := true
# board specific modules
BOARD_USES_TINYALSA_AUDIO := true
BOARD_USES_LINE_CALL	:= true
BOARD_USE_VETH := true
BOARD_SPRD_RIL := true
BOARD_SAMSUNG_RIL := false
TARGET_RECOVERY_UI_LIB := librecovery_ui_sp8810
TARGET_RECOVERY_PIXEL_FORMAT := "RGBA_8888"

BOARD_WPA_SUPPLICANT_DRIVER := NL80211
WPA_SUPPLICANT_VERSION     := VER_0_8_X
BOARD_WPA_SUPPLICANT_PRIVATE_LIB := lib_driver_cmd_bcmdhd
BOARD_HOSTAPD_DRIVER        := NL80211
BOARD_HOSTAPD_PRIVATE_LIB   := lib_driver_cmd_bcmdhd
BOARD_WLAN_DEVICE           := bcmdhd

WIFI_DRIVER_FW_PATH_PARAM   := "/sys/module/bcmdhd/parameters/firmware_path"
WIFI_DRIVER_FW_PATH_STA     := "/etc/wifi/fw_bcmdhd.bin"
WIFI_DRIVER_FW_PATH_P2P     := "/etc/wifi/fw_bcmdhd_p2p.bin"
WIFI_DRIVER_FW_PATH_AP      := "/etc/wifi/fw_bcmdhd_apsta.bin"
USE_CAMERA_STUB := true

BOARD_USES_GENERIC_AUDIO := false
USE_BOOT_AT_DIAG := true
