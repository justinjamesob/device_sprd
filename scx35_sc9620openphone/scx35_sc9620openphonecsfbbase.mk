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
PLATDIR := device/sprd/scx35

TARGET_BOARD := scx35_sc9620openphone
BOARDDIR := device/sprd/$(TARGET_BOARD)

# call connectivity_configure_9620.mk before calling device.mk
$(call inherit-product, vendor/sprd/open-source/res/productinfo/connectivity_configure_9620.mk)

# include general common configs
$(call inherit-product, $(PLATDIR)/device.mk)
$(call inherit-product, $(PLATDIR)/emmc/emmc_device.mk)
$(call inherit-product, $(PLATDIR)/proprietories.mk)

DEVICE_PACKAGE_OVERLAYS := $(BOARDDIR)/overlay $(PLATDIR)/overlay

PRODUCT_AAPT_CONFIG := hdpi xhdpi

# Set default USB interface
PRODUCT_DEFAULT_PROPERTY_OVERRIDES += \
	persist.sys.usb.config=mass_storage

PRODUCT_PROPERTY_OVERRIDES += \
        persist.sys.modem.diag=,gser \
        sys.usb.gser.count=4 \
        keyguard.no_require_sim=true \
        ro.com.android.dataroaming=false \
        persist.msms.phone_count=1 \
        persist.msms.phone_default=0 \
        ro.modem.external.enable=1 \
        persist.modem.t.enable=0 \
        persist.modem.t.cs=1 \
        persist.modem.t.ps=1 \
        persist.modem.t.rsim=0 \
        persist.modem.l.nvp=tl_ \
        persist.modem.l.enable=1 \
        persist.modem.l.cs=0 \
        persist.modem.l.ps=1 \
        persist.modem.l.rsim=1 \
        persist.radio.ssda.mode=csfb \
        persist.radio.ssda.testmode=7

# board-specific modules
PRODUCT_PACKAGES += \
        sensors.$(TARGET_PLATFORM) \
        fm.$(TARGET_PLATFORM) \
        ValidationTools \
        libmllite.so \
        libmplmpu.so \
        libinvensense_hal

# NFC packages
PRODUCT_PACKAGES += \
        libnfc-nci \
        libnfc_nci_jni \
        nfc_nci_pn547.default \
        NfcNci \
        Tag \
        com.android.nfc_extras

# board-specific files
PRODUCT_COPY_FILES += \
	$(BOARDDIR)/init.board.rc:root/init.board.rc \
	$(BOARDDIR)/audio_params/tiny_hw_lineincall.xml:system/etc/tiny_hw_lineincall.xml \
	$(BOARDDIR)/audio_params/tiny_hw.xml:system/etc/tiny_hw.xml \
	$(BOARDDIR)/audio_params/codec_pga.xml:system/etc/codec_pga.xml \
	$(BOARDDIR)/audio_params/audio_hw.xml:system/etc/audio_hw.xml \
	$(BOARDDIR)/audio_params/audio_para:system/etc/audio_para \
	$(BOARDDIR)/focaltech_ts.idc:system/usr/idc/focaltech_ts.idc \
	$(BOARDDIR)/imei_config/imei1.txt:prodnv/imei1.txt \
	$(BOARDDIR)/imei_config/imei2.txt:prodnv/imei2.txt \
	$(BOARDDIR)/imei_config/imei3.txt:prodnv/imei3.txt \
	$(BOARDDIR)/imei_config/imei4.txt:prodnv/imei4.txt \
	frameworks/native/data/etc/android.hardware.nfc.xml:system/etc/permissions/android.hardware.nfc.xml \
        external/libnfc-nci/libpn547_fw.so:system/vendor/firmware/libpn547_fw.so \
        external/libnfc-nci/halimpl/pn547/libnfc-nxp.conf:system/etc/libnfc-nxp.conf \
        external/libnfc-nci/halimpl/pn547/libnfc-brcm.conf:system/etc/libnfc-brcm.conf \
        packages/apps/Nfc/nci/route.xml:system/etc/param/route.xml \
        frameworks/native/data/etc/com.nxp.mifare.xml:system/etc/permissions/com.nxp.mifare.xml \
        frameworks/native/data/etc/com.android.nfc_extras.xml:system/etc/permissions/com.android.nfc_extras.xml \
        frameworks/native/data/etc/android.hardware.nfc.hce.xml:system/etc/permissions/android.hardware.nfc.hce.xml

$(call inherit-product, vendor/sprd/open-source/res/boot/boot_res_9620.mk)
$(call inherit-product, frameworks/native/build/phone-hdpi-512-dalvik-heap.mk)

$(call inherit-product-if-exists, vendor/sprd/open-source/common_packages.mk)
$(call inherit-product-if-exists, vendor/sprd/open-source/base_special_packages.mk)
$(call inherit-product, vendor/sprd/partner/shark/bluetooth/device-shark-bt.mk)
$(call inherit-product, vendor/sprd/gps/CellGuide_2351/device-sprd-gps.mk)

# Overrides
PRODUCT_NAME := scx35_sc9620openphonecsfbbase
PRODUCT_DEVICE := $(TARGET_BOARD)
PRODUCT_MODEL := sc9620openphone
PRODUCT_BRAND := Spreadtrum
PRODUCT_MANUFACTURER := Spreadtrum

PRODUCT_LOCALES := zh_CN zh_TW en_US