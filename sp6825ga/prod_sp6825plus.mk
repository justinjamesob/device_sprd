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

TARGET_PLATFORM := sc8825
TARGET_BOARD := sp6825ga
BOARDDIR := device/sprd/$(TARGET_BOARD)

DEVICE_PACKAGE_OVERLAYS := $(BOARDDIR)/overlay

PRODUCT_PROPERTY_OVERRIDES := \
	keyguard.no_require_sim=true \
	ro.com.android.dataroaming=false \
	ro.device.support.abroad.apn=1\
	h_bright=true \
	persist.msms.phone_count=2 \
	persist.msms.phone_default=0 \
        lmk.autocalc=false \
        zram.support=true \
	persist.blcr.enable=0 \
        persist.sys.service.delay=false \
        persist.sys.lowmem=16 \
	ro.msms.phone_count=2 \
	ro.modem.count=1 \
	ro.modem.t.enable=1 \
	ro.modem.t.tty=/dev/ts0710mux \
	ro.modem.t.eth=veth \
	ro.modem.t.snd=1 \
	ro.modem.t.diag=/dev/vbpipe0 \
	ro.modem.t.nv=/dev/vbpipe1 \
	ro.modem.t.assert=/dev/vbpipe2 \
	ro.modem.t.vbc=/dev/vbpipe6 \
	ro.modem.t.id=0 \
	ro.modem.t.count=2 \
    persist.surpport.oplpnn=true \
    persist.surpport.cphsfirst=false

ifeq ($(TARGET_BUILD_VARIANT),user)
  PRODUCT_PROPERTY_OVERRIDES += persist.sys.sprd.modemreset=1
else
  PRODUCT_PROPERTY_OVERRIDES += persist.sys.sprd.modemreset=0
endif


# Set default USB interface
PRODUCT_DEFAULT_PROPERTY_OVERRIDES += \
	persist.sys.usb.config=mtp

PRODUCT_PACKAGES := \
	Settings \
	MsmsPhone \
	MsmsSettings \
	MsmsStk \
	Stk1 \
        ES_File_Explorer.apk \
        CallFireWall   \
        AudioProfile \
        LiveWallpapers  \
        LiveWallpapersPicker \
        Galaxy4 \
        HoloSpiralWallpaper \
        MagicSmokeWallpapers \
        VisualizationWallpapers \
        ValidationTools \
        ValidationtoolsFm \
        vtserver \
        libvalidationtoolsjni \
        NoiseField \
        PhaseBeam \
        CellBroadcastReceiver \
        Monternet \
	AppBackup   \
	SearchCallLog



PRODUCT_COPY_FILES := \
	$(BOARDDIR)/sprd-keypad.kl:system/usr/keylayout/sprd-keypad.kl \
	$(BOARDDIR)/ft5x0x_ts.idc:system/usr/idc/ft5x0x_ts.idc \
	$(BOARDDIR)/headset-keyboard.kl:system/usr/keylayout/headset-keyboard.kl

$(call inherit-product, frameworks/native/build/phone-hdpi-dalvik-heap.mk)
# include classified configs
$(call inherit-product, $(BOARDDIR)/base.mk)
$(call inherit-product, $(BOARDDIR)/proprietories.mk)
$(call inherit-product, device/sprd/common/res/boot/boot_res.mk)

# include standard configs
$(call inherit-product, $(SRC_TARGET_DIR)/product/full_base.mk)
$(call inherit-product, $(SRC_TARGET_DIR)/product/telephony.mk)

# Overrides
PRODUCT_NAME := sp6825gaplus
PRODUCT_DEVICE := $(TARGET_BOARD)
PRODUCT_MODEL := sp6825ga
PRODUCT_BRAND := Spreadtrum
PRODUCT_MANUFACTURER := Spreadtrum

PRODUCT_LOCALES := zh_CN zh_TW en_US
ifeq ($(MULTILANGUAGE_SUPPORT),true)
  PRODUCT_PACKAGES += $(MULTILANGUAGE_PRODUCT_PACKAGES)
  PRODUCT_LOCALES := zh_CN zh_TW en_US en_AU en_CA en_GB en_IE en_IN en_NZ en_SG en_ZA bn_BD in_ID id_ID ms_MY ar_EG ar_IL th_TH vi_VN es_US es_ES pt_PT pt_BR ru_RU hi_IN my_MM fr_BE fr_CA fr_CH fr_FR tl_PH ur_IN ur_PK fa_AF fa_IR tr_TR sw_KE sw_TZ ro_RO te_IN ta_IN ha_GH ha_NE ha_NG ug_CN ce_PH bo_CN bo_IN it_CH it_IT de_AT de_CH de_DE de_LI el_GR cs_CZ pa_IN gu_IN km_KH lo_LA nl_BE nl_NL pl_PL am_ET
endif
PRODUCT_PROPERTY_OVERRIDES += ro.product.locale.language=zh
PRODUCT_PROPERTY_OVERRIDES += ro.product.locale.region=CN
