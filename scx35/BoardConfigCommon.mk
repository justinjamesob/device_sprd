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

TARGET_ARCH := arm
TARGET_ARCH_VARIANT := armv7-a-neon
TARGET_CPU_ABI := armeabi-v7a
TARGET_CPU_ABI2 := armeabi
TARGET_CPU_VARIANT := cortex-a7
TARGET_CPU_SMP := true
ARCH_ARM_HAVE_TLS_REGISTER := true

TARGET_BOARD_PLATFORM := sc8830

# Enable dex-preoptimization
WITH_DEXPREOPT := true

# config u-boot
TARGET_NO_BOOTLOADER := false

# config kernel
TARGET_NO_KERNEL := false
USES_UNCOMPRESSED_KERNEL := true
BOARD_KERNEL_BASE := 0x00000000
BOARD_KERNEL_CMDLINE := console=ttyS1,115200n8

# config sepolicy
BOARD_SEPOLICY_DIRS := device/sprd/scx35/sepolicy
BOARD_SEPOLICY_UNION := \
       file.te \
       file_contexts

# recovery configs
TARGET_RECOVERY_FSTAB := device/sprd/scx35/recovery/recovery.fstab
TARGET_RECOVERY_INITRC := device/sprd/scx35/recovery/init.rc
TARGET_RECOVERY_UI_LIB := librecovery_ui_sp8830
TARGET_RECOVERY_PIXEL_FORMAT := "RGBA_8888"

# camera configs
USE_CAMERA_STUB := true
#zsl capture
TARGET_BOARD_CAMERA_CAPTURE_MODE := false
#back camera rotation capture
TARGET_BOARD_BACK_CAMERA_ROTATION := false
#front camera rotation capture
TARGET_BOARD_FRONT_CAMERA_ROTATION := false
#rotation capture
TARGET_BOARD_CAMERA_ROTATION_CAPTURE := false
#video size 1080P,720P,D1
TARGET_BOARD_CAMERA_SUPPORT_VIDEO_SIZE := 1080P
#framebuffer switch to rgb16 in 1080P video recorder, only for shark!
FB_FORMAT_SWITCH := true
TARGET_BOARD_BACK_CAMERA_INTERFACE := mipi
TARGET_BOARD_FRONT_CAMERA_INTERFACE := ccir
TARGET_BOARD_CAMERA_HAL_VERSIONG := HAL1.0

# audio configs
BOARD_USES_GENERIC_AUDIO := true
BOARD_USES_TINYALSA_AUDIO := true
BOARD_USES_ALSA_AUDIO := false
BUILD_WITH_ALSA_UTILS := false

# telephony
BOARD_USE_VETH := true
BOARD_SPRD_RIL := true
USE_BOOT_AT_DIAG := true

# graphics
TARGET_GPU_PP_CORE := 2
USE_SPRD_HWCOMPOSER  := true
USE_OPENGL_RENDERER := true
USE_OVERLAY_COMPOSER_GPU := true

