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

TARGET_PLATFORM := sc8830
TARGET_BOARD := sp8830ssw
BOARDDIR := device/sprd/$(TARGET_BOARD)

DEVICE_PACKAGE_OVERLAYS := $(BOARDDIR)/overlay
PRODUCT_PACKAGE_OVERLAYS := vendor/sprd/operator/cucc/specA/overlay

PRODUCT_AAPT_CONFIG := hdpi

PRODUCT_PROPERTY_OVERRIDES := \
	keyguard.no_require_sim=true \
	ro.com.android.dataroaming=false \
	persist.msms.phone_count=2 \
	universe_ui_support=true \
	ro.msms.phone_count=2 \
	persist.msms.phone_default=0 \
	ro.modem.count=1 \
	ro.digital.fm.support=0 \
	ro.modem.w.enable=1 \
	ro.modem.w.dev=/dev/cpw \
	ro.modem.w.tty=/dev/stty_w \
	ro.modem.w.eth=seth_w \
	ro.modem.w.snd=2 \
	ro.modem.w.diag=/dev/slog_w \
	ro.modem.w.loop=/dev/spipe_w0 \
	ro.modem.w.nv=/dev/spipe_w1 \
	ro.modem.w.assert=/dev/spipe_w2 \
	ro.modem.w.vbc=/dev/spipe_w6 \
	ro.modem.w.id=0 \
	ro.modem.w.count=2

ifeq ($(TARGET_BUILD_VARIANT),user)
  PRODUCT_PROPERTY_OVERRIDES += persist.sys.sprd.modemreset=1
else
  PRODUCT_PROPERTY_OVERRIDES += persist.sys.sprd.modemreset=0
endif

# Set default USB interface
PRODUCT_DEFAULT_PROPERTY_OVERRIDES += \
	persist.sys.usb.config=mass_storage

PRODUCT_PACKAGES := \
    MsmsPhone \
    Settings \
    MsmsStk \
    Stk1 \
    framework2


# prebuild files
PRODUCT_PACKAGES += \
	moffice_5.3.3_1033_cn00353_multidex_135897.apk \
	Weibo3.3.0_300_ZhanXun_0308.apk \
	20130308_iFlyIME_v3.0.1265_Spreadtrum_01430228.apk \
	Wostore.apk \
	UnicomClient.apk \
	116114.apk

# packages files
PRODUCT_PACKAGES += \

# add  system properties
PRODUCT_PROPERTY_OVERRIDES += \
	ro.operator=cucc \
	ro.operator.version=specA

PRODUCT_COPY_FILES := \
	$(BOARDDIR)/sprd-keypad.kl:system/usr/keylayout/sprd-keypad.kl \
	$(BOARDDIR)/sci-keypad.kl:system/usr/keylayout/sci-keypad.kl \
	$(BOARDDIR)/gpio-keys.kl:system/usr/keylayout/gpio-keys.kl \
	$(BOARDDIR)/sec_touchscreen.idc:system/usr/idc/sec_touchscreen.idc \
        device/sprd/common/res/spn/spn-conf.xml:system/etc/spn-conf.xml

$(call inherit-product, frameworks/native/build/phone-HD720-dalvik-heap.mk)

# include classified configs
$(call inherit-product, vendor/sprd/operator/cucc/specA/res/apn/apn_res.mk)
$(call inherit-product, $(BOARDDIR)/base.mk)
$(call inherit-product, $(BOARDDIR)/proprietories.mk)
$(call inherit-product, vendor/sprd/operator/cucc/specA/res/boot/boot_res.mk)
$(call inherit-product, vendor/sprd/UniverseUI/universeui.mk)

# include standard configs
$(call inherit-product, $(SRC_TARGET_DIR)/product/full_base.mk)
$(call inherit-product, $(SRC_TARGET_DIR)/product/telephony.mk)

# Overrides
PRODUCT_NAME := sp8830ssw
PRODUCT_DEVICE := $(TARGET_BOARD)
PRODUCT_MODEL := sp8830ssw
PRODUCT_BRAND := Spreadtrum
PRODUCT_MANUFACTURER := Spreadtrum

PRODUCT_LOCALES := zh_CN zh_TW en_US

