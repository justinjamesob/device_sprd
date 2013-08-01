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

TARGET_BOARD_PLATFORM := shark0
TARGET_BOOTLOADER_BOARD_NAME := shark0ea

TARGET_VLX_ENABLE ?= true

# Enable the optimized DEX
ifneq ($(filter user, $(TARGET_BUILD_VARIANT)),)
WITH_DEXPREOPT=true
else
WITH_DEXPREOPT=
endif

ifneq ($(filter %sdk, $(TARGET_PRODUCT)),)
# for emulator
TARGET_NO_BOOTLOADER := true
TARGET_NO_KERNEL := true
TARGET_PROVIDES_INIT_RC := false
# Build OpenGLES emulation guest and host libraries
BUILD_EMULATOR_OPENGL := true

# Build and enable the OpenGL ES View renderer. When running on the emulator,
# the GLES renderer disables itself if host GL acceleration isn't available.
USE_OPENGL_RENDERER := true

BOARD_USE_VETH := true
# for emulator end
else
# for real hardware
# config u-boot
TARGET_NO_BOOTLOADER := false
ifeq ($(TARGET_VLX_ENABLE), true)
UBOOT_DEFCONFIG := shark0ea
else
UBOOT_DEFCONFIG := shark0ea_native
endif

# config kernel
TARGET_NO_KERNEL := false
ifeq ($(TARGET_VLX_ENABLE), true)
KERNEL_DEFCONFIG := shark0ea-vlx-mxd_4p1_defconfig
else
KERNEL_DEFCONFIG := shark0ea-native_defconfig
endif
USES_UNCOMPRESSED_KERNEL := true
BOARD_KERNEL_BASE := 0x00000000
BOARD_KERNEL_CMDLINE := console=ttyS1,115200n8

# use default init.rc
TARGET_PROVIDES_INIT_RC := true

BOARD_HAVE_BLUETOOTH := true
BOARD_HAVE_BLUETOOTH_BCM := true
BOARD_HAVE_FM_BCM := true
BOARD_USE_SPRD_FMAPP := true

# board specific modules
BOARD_USES_TINYALSA_AUDIO := true
BOARD_USE_VETH := true
BOARD_SPRD_RIL := true
BOARD_SAMSUNG_RIL := false
TARGET_RECOVERY_UI_LIB := librecovery_ui_sp8810
TARGET_RECOVERY_PIXEL_FORMAT := "RGBA_8888"
USE_SPRD_HWCOMPOSER  := true

# ext4 partition layout
TARGET_USERIMAGES_USE_EXT4 := true
BOARD_SYSTEMIMAGE_PARTITION_SIZE := 300000000
BOARD_USERDATAIMAGE_PARTITION_SIZE := 2000000000
BOARD_CACHEIMAGE_PARTITION_SIZE := 150000000
BOARD_FLASH_BLOCK_SIZE := 4096
BOARD_CACHEIMAGE_FILE_SYSTEM_TYPE := ext4
USE_OPENGL_RENDERER := true


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


####################!!!! DONT'T FOLLOW THIS WAY TO CUSTOMIZE  !!!###############
#We confirm(2012-12-26):we will only use maxscend
#BOARD_CMMB_HW ?= siano

#ifeq ($(BOARD_CMMB_HW), mxd)
#KERNEL_DEFCONFIG := shark0ea-vlx-mxd_defconfig
CMMB_MXDID       := 0266
#endif
################################################################################
# for real hardware end
endif
