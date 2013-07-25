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

TARGET_PLATFORM := sc7710
TARGET_BOARD := sp7710ga
BOARDDIR := device/sprd/$(TARGET_BOARD)

DEVICE_PACKAGE_OVERLAYS := $(BOARDDIR)/overlay
PRODUCT_PACKAGE_OVERLAYS := vendor/sprd/operator/cmcc/specA/overlay

PRODUCT_AAPT_CONFIG := hdpi

PRODUCT_PROPERTY_OVERRIDES := \
	keyguard.no_require_sim=true \
	ro.com.android.dataroaming=false \
	persist.msms.phone_count=3 \
	persist.msms.phone_default=0 \
        lmk.autocalc=false \
        ksm.support=false    \
        zram.support=true \
        zram_for_android.enable=true  \
        ro.build.product.lowmem=1 \
	h_bright=true \
        universe_ui_support=true \
        ro.callfirewall.disabled=true \
	ro.msms.phone_count=3 \
	ro.modem.count=1 \
	ro.modem.w.enable=1 \
	ro.modem.w.tty=/dev/ts0710mux \
	ro.modem.w.eth=veth \
	ro.modem.w.id=0 \
	ro.modem.w.count=3 \
	ro.config.hw.cmmb_support=false \
	ro.config.hw.search_support=false \
    persist.surpport.oplpnn=true \
    persist.surpport.cphsfirst=false

ifeq ($(TARGET_LOWCOST_SUPPORT),true)
  PRODUCT_PROPERTY_OVERRIDES += persist.sys.lowcost=true
endif

ifeq ($(TARGET_BUILD_VARIANT),user)
  PRODUCT_PROPERTY_OVERRIDES += persist.sys.sprd.modemreset=1
else
  PRODUCT_PROPERTY_OVERRIDES += persist.sys.sprd.modemreset=0
endif

ifeq ($(TARGET_HVGA_ENABLE), true)
PRODUCT_PROPERTY_OVERRIDES += ro.sf.lcd_density=160
else
PRODUCT_PROPERTY_OVERRIDES += ro.sf.lcd_density=240
endif

# Set default USB interface
PRODUCT_DEFAULT_PROPERTY_OVERRIDES += \
	persist.sys.usb.config=mass_storage

PRODUCT_PACKAGES := \
	MsmsPhone \
	Settings \
	MsmsStk \
	Stk1 \
	Stk2 \
	framework2 

# prebuild files
PRODUCT_PACKAGES += \

# packages files
PRODUCT_PACKAGES += \
	
PRODUCT_COPY_FILES := \
	$(BOARDDIR)/sprd-keypad.kl:system/usr/keylayout/sprd-keypad.kl \
	$(BOARDDIR)/ft5x0x_ts.idc:system/usr/idc/ft5x0x_ts.idc \
	$(BOARDDIR)/headset-keyboard.kl:system/usr/keylayout/headset-keyboard.kl

$(call inherit-product, frameworks/native/build/phone-hdpi-256-dalvik-heap.mk)

# include classified configs
$(call inherit-product, $(BOARDDIR)/base.mk)
$(call inherit-product, $(BOARDDIR)/proprietories.mk)
$(call inherit-product, vendor/sprd/operator/cucc/specA/res/boot/boot_res.mk)
$(call inherit-product, vendor/sprd/UniverseUI/universeui.mk)


# include standard configs
$(call inherit-product, $(SRC_TARGET_DIR)/product/full_base.mk)
$(call inherit-product, $(SRC_TARGET_DIR)/product/telephony.mk)

# Overrides
PRODUCT_NAME := sp7710gatri
PRODUCT_DEVICE := $(TARGET_BOARD)
PRODUCT_MODEL := sp7710ga
PRODUCT_BRAND := Spreadtrum
PRODUCT_MANUFACTURER := Spreadtrum

PRODUCT_LOCALES := zh_CN zh_TW en_US
ifeq ($(MULTILANGUAGE_SUPPORT),true)
  PRODUCT_PACKAGES += $(MULTILANGUAGE_PRODUCT_PACKAGES)
  PRODUCT_LOCALES := zh_CN zh_TW en_US en_AU en_CA en_GB en_IE en_IN en_NZ en_SG en_ZA bn_BD in_ID id_ID ms_MY ar_EG ar_IL th_TH vi_VN es_US es_ES pt_PT pt_BR ru_RU hi_IN my_MM fr_BE fr_CA fr_CH fr_FR tl_PH ur_IN ur_PK fa_AF fa_IR tr_TR sw_KE sw_TZ ro_RO te_IN ta_IN ha_GH ha_NE ha_NG ug_CN ce_PH bo_CN bo_IN it_CH it_IT de_AT de_CH de_DE de_LI el_GR cs_CZ pa_IN gu_IN km_KH lo_LA nl_BE nl_NL pl_PL am_ET
endif
PRODUCT_PROPERTY_OVERRIDES += ro.product.locale.language=zh
PRODUCT_PROPERTY_OVERRIDES += ro.product.locale.region=CN
