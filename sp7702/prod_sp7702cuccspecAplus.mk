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

TARGET_PLATFORM := sc8810
TARGET_BOARD := sp7702
BOARDDIR := device/sprd/$(TARGET_BOARD)

DEVICE_PACKAGE_OVERLAYS := $(BOARDDIR)/overlay
PRODUCT_PACKAGE_OVERLAYS := vendor/sprd/operator/cucc/specA/overlay

PRODUCT_AAPT_CONFIG := hdpi

PRODUCT_PROPERTY_OVERRIDES := \
	keyguard.no_require_sim=true \
	ro.com.android.dataroaming=false \
	h_bright=true \
	persist.msms.phone_count=2 \
	persist.msms.phone_default=0 \
	lmk.autocalc=false \
        ksm.support=true   \
	zram.support=true \
	ro.build.product.lowmem=1 \
        universe_ui_support=true \
        ro.callfirewall.disabled=true \
	ro.msms.phone_count=2 \
	ro.modem.count=1 \
	ro.modem.w.enable=1 \
	ro.modem.w.tty=/dev/ts0710mux \
	ro.modem.w.eth=veth \
	ro.modem.w.id=0 \
	ro.modem.w.count=2 \
    persist.surpport.oplpnn=true \
    persist.surpport.cphsfirst=false

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
	sprdcudm \
	framework2

# prebuild files
PRODUCT_PACKAGES += \
	wps.apk \
	weibopro_10035_0001_0.9.0noshortcut_Android-0506.apk \
	SogouInput_31143_android_oem_zhanxun_20130509.apk \
	SohuNewsClient_v3.3_factory_zhanxun_6230.apk \
	baidumap_andr_3052.apk \
	wostore.apk \
	Unicomclient_SP7710-4.1.apk \
	wo116114.apk \
	AIMail_Android_V375a_orange.apk \
	DeviceRegister_debug.apk

# packages files
PRODUCT_PACKAGES += \
	
# add  system properties
PRODUCT_PROPERTY_OVERRIDES += \
	ro.operator=cucc \
	ro.operator.version=specA
	
PRODUCT_COPY_FILES := \
	$(BOARDDIR)/sprd-keypad.kl:system/usr/keylayout/sprd-keypad.kl \
	$(BOARDDIR)/pixcir_ts.kl:system/usr/keylayout/pixcir_ts.kl \
	$(BOARDDIR)/pixcir_ts.idc:system/usr/idc/pixcir_ts.idc

$(call inherit-product, frameworks/native/build/phone-hdpi-256-dalvik-heap.mk)

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
PRODUCT_NAME := sp7702cuccspecAplus
PRODUCT_DEVICE := $(TARGET_BOARD)
PRODUCT_MODEL := ZTE U970
PRODUCT_BRAND := Spreadtrum
PRODUCT_MANUFACTURER := Spreadtrum

PRODUCT_LOCALES := zh_CN zh_TW en_US
ifeq ($(MULTILANGUAGE_SUPPORT),true)
  PRODUCT_PACKAGES += $(MULTILANGUAGE_PRODUCT_PACKAGES)
  PRODUCT_LOCALES := zh_CN zh_TW en_US en_AU en_CA en_GB en_IE en_IN en_NZ en_SG en_ZA bn_BD in_ID id_ID ms_MY ar_EG ar_IL th_TH vi_VN es_US es_ES pt_PT pt_BR ru_RU hi_IN my_MM fr_BE fr_CA fr_CH fr_FR tl_PH ur_IN ur_PK fa_AF fa_IR tr_TR sw_KE sw_TZ ro_RO te_IN ta_IN ha_GH ha_NE ha_NG ug_CN ce_PH bo_CN bo_IN it_CH it_IT de_AT de_CH de_DE de_LI el_GR cs_CZ pa_IN gu_IN km_KH lo_LA nl_BE nl_NL pl_PL am_ET
endif
PRODUCT_PROPERTY_OVERRIDES += ro.product.locale.language=zh
PRODUCT_PROPERTY_OVERRIDES += ro.product.locale.region=CN