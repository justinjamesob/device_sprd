#
# Copyright (C) 2007 The Android Open Source Project
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

TARGET_PLATFORM := scx15
PLATDIR := device/sprd/$(TARGET_PLATFORM)
OPERATOR_OVERLAY := vendor/sprd/operator/operator_res/operatorname_overlay

TARGET_BOARD := scx15_sp7715ea
BOARDDIR := device/sprd/$(TARGET_BOARD)

BOARD_KERNEL_PAGESIZE := 2048
BOARD_KERNEL_SEPARATED_DT := true

# include general common configs
$(call inherit-product, $(PLATDIR)/device.mk)
$(call inherit-product, $(PLATDIR)/emmc/emmc_device.mk)
$(call inherit-product, $(PLATDIR)/proprietories.mk)

DEVICE_PACKAGE_OVERLAYS := $(BOARDDIR)/overlay $(PLATDIR)/overlay $(OPERATOR_OVERLAY)

PRODUCT_AAPT_CONFIG := hdpi

# Remove xhdpi

# Set default USB interface
PRODUCT_DEFAULT_PROPERTY_OVERRIDES += \
	persist.sys.usb.config=mass_storage

PRODUCT_PROPERTY_OVERRIDES += \
	keyguard.no_require_sim=true \
	ro.com.android.dataroaming=false \
	ro.msms.phone_count=2 \
	persist.msms.phone_count=2 \
	persist.msmslt=0 \
	ro.modem.w.count=2 \
        persist.sys.modem.diag=,gser \
        sys.usb.gser.count=4 \
        wcdma.sim.slot.cfg=true \
        persist.support.oplpnn=true \
        persist.support.cphsfirst=false

# board-specific modules
PRODUCT_PACKAGES += \
        sensors.$(TARGET_PLATFORM) \
	fm.$(TARGET_PLATFORM)

-include vendor/sprd/open-source/common_packages.mk
-include vendor/sprd/open-source/plus_special_packages.mk

#	libmllite.so \
#	libmplmpu.so \
#	libinvensense_hal
#
# board-specific files
PRODUCT_COPY_FILES += \
	$(BOARDDIR)/init.board.rc:root/init.board.rc \
	$(BOARDDIR)/audio_params/tiny_hw.xml:system/etc/tiny_hw.xml \
	$(BOARDDIR)/audio_params/codec_pga.xml:system/etc/codec_pga.xml \
	$(BOARDDIR)/audio_params/audio_hw.xml:system/etc/audio_hw.xml \
	$(BOARDDIR)/audio_params/audio_para:system/etc/audio_para \
	$(BOARDDIR)/audio_params/audio_policy.conf:system/etc/audio_policy.conf \
	$(BOARDDIR)/focaltech_ts.idc:system/usr/idc/focaltech_ts.idc \
	frameworks/native/data/etc/android.hardware.sensor.light.xml:system/etc/permissions/android.hardware.sensor.light.xml \
	frameworks/native/data/etc/android.hardware.sensor.proximity.xml:system/etc/permissions/android.hardware.sensor.proximity.xml \
	frameworks/native/data/etc/android.hardware.sensor.accelerometer.xml:system/etc/permissions/android.hardware.sensor.accelerometer.xml \
	frameworks/native/data/etc/android.hardware.sensor.compass.xml:system/etc/permissions/android.hardware.sensor.compass.xml

$(call inherit-product, vendor/sprd/open-source/res/boot/boot_res_8830s.mk)
$(call inherit-product, frameworks/native/build/phone-hdpi-512-dalvik-heap.mk)
$(call inherit-product, vendor/sprd/partner/shark/bluetooth/device-shark-bt.mk)
$(call inherit-product, vendor/sprd/open-source/res/productinfo/connectivity_configure_7715.mk)

# Overrides
PRODUCT_NAME := scx15_sp7715eaplus_dt
PRODUCT_DEVICE := $(TARGET_BOARD)
PRODUCT_MODEL := SP7715A
PRODUCT_BRAND := Spreadtrum
PRODUCT_MANUFACTURER := Spreadtrum

PRODUCT_LOCALES := zh_CN zh_TW en_US
