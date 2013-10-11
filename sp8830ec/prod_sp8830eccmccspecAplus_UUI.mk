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
TARGET_BOARD := sp8830ec
BOARDDIR := device/sprd/$(TARGET_BOARD)

DEVICE_PACKAGE_OVERLAYS := $(BOARDDIR)/overlay
PRODUCT_PACKAGE_OVERLAYS := vendor/sprd/operator/cmcc/specA/overlay

PRODUCT_AAPT_CONFIG := hdpi xhdpi

PRODUCT_PROPERTY_OVERRIDES := \
	keyguard.no_require_sim=true \
	ro.com.android.dataroaming=false \
	persist.msms.phone_count=2 \
	universe_ui_support=true \
	ro.msms.phone_count=2 \
	h_bright=true \
	persist.msms.phone_default=0 \
	ro.modem.count=2 \
	ro.digital.fm.support=1 \
	ro.modem.t.enable=1 \
	ro.modem.t.dev=/dev/cpt \
	ro.modem.t.tty=/dev/stty_td \
	ro.modem.t.eth=seth_td \
	ro.modem.t.snd=1 \
	ro.modem.t.diag=/dev/slog_td \
	ro.modem.t.loop=/dev/spipe_td0 \
	ro.modem.t.nv=/dev/spipe_td1 \
	ro.modem.t.assert=/dev/spipe_td2 \
	ro.modem.t.vbc=/dev/spipe_td6 \
	ro.modem.t.id=0 \
	ro.modem.t.count=2 \
	ro.config.hw.cmmb_support=false \
	ro.config.hw.camera_support=false \
	ro.config.hw.search_support=false \
    persist.surpport.oplpnn=true \
    persist.surpport.cphsfirst=false \
	ro.modem.wcn.enable=1 \
	ro.modem.wcn.dev=/dev/cpwcn \
	ro.modem.wcn.tty=/deiv/stty_wcn \
	ro.modem.wcn.diag=/dev/slog_wcn \
	ro.modem.wcn.assert=/dev/spipe_wcn2 \
	ro.modem.wcn.id=1 \
	ro.modem.wcn.count=1 \
    lmk.autocalc=false

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
    SprdDM \
    framework2

# prebuild files
PRODUCT_PACKAGES += \
    moffice.apk \
    CMCCFetion.apk \
    CMCCGameHall.apk \
    CMCCMM.apk \
    CMCCMobileMusic.apk \
    CMCCMV_W.apk \
    CMCCREAD.apk \
    CMCCTelenav_Nav.apk \
    CMCCWIFI.apk \
    PimClient.apk \
    SogouInput.apk

	
# packages files
PRODUCT_PACKAGES += \
	ForOPCModuleTest
	
# own copyright packages files
PRODUCT_PACKAGES += \
    10086cn \
    Monternet \
    MyFavorites

	
# add  system properties
PRODUCT_PROPERTY_OVERRIDES += \
	ro.operator=cmcc \
	ro.operator.version=specA
	
PRODUCT_COPY_FILES := \
	$(BOARDDIR)/sprd-keypad.kl:system/usr/keylayout/sprd-keypad.kl \
	$(BOARDDIR)/sci-keypad.kl:system/usr/keylayout/sci-keypad.kl \
	$(BOARDDIR)/pixcir_ts.kl:system/usr/keylayout/pixcir_ts.kl \
	$(BOARDDIR)/pixcir_ts.idc:system/usr/idc/pixcir_ts.idc \
	$(BOARDDIR)/focaltech_ts.idc:system/usr/idc/focaltech_ts.idc \
        device/sprd/common/res/spn/spn-conf.xml:system/etc/spn-conf.xml

$(call inherit-product, frameworks/native/build/phone-HD720-dalvik-heap.mk)

USE_PROJECT_SEC :=false

# include classified configs
$(call inherit-product, $(BOARDDIR)/base.mk)
$(call inherit-product, $(BOARDDIR)/proprietories.mk)
$(call inherit-product, device/sprd/common/res/boot/boot_res.mk)
$(call inherit-product, vendor/sprd/UniverseUI/universeui.mk)

# include standard configs
$(call inherit-product, $(SRC_TARGET_DIR)/product/full_base.mk)
$(call inherit-product, $(SRC_TARGET_DIR)/product/telephony.mk)

# Overrides
PRODUCT_NAME := sp8830eccmccspecAplus_UUI
PRODUCT_DEVICE := $(TARGET_BOARD)
PRODUCT_MODEL := sp8830ec
PRODUCT_BRAND := Spreadtrum
PRODUCT_MANUFACTURER := Spreadtrum

PRODUCT_LOCALES := zh_CN zh_TW en_US

ifeq ($(USE_PROJECT_SEC),true)
PRODUCT_PROPERTY_OVERRIDES += \
persist.support.securetest=1
endif

ifeq ($(USE_PROJECT_SEC),true)
# prebuild files
PRODUCT_PACKAGES += \
        Permission.apk \
        choose_secure

PRODUCT_COPY_FILES += \
        frameworks/base/core/java/com/sprd/telephonesec.db:/system/etc/telephonesec.db
endif
