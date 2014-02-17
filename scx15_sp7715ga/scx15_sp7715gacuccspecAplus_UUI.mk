-include vendor/sprd/operator/cucc/specA/res/boot/boot_res_fwvga.mk

PRODUCT_THEME_PACKAGES := SimpleStyle
PRODUCT_THEME_FLAGS := shrink

$(call inherit-product, vendor/sprd/operator/cucc/specA/res/apn/apn_res.mk)

include device/sprd/scx15_sp7715ga/scx15_sp7715gaplus.mk

PRODUCT_NAME := scx15_sp7715gacuccspecAplus_UUI

DEVICE_PACKAGE_OVERLAYS := $(PLATDIR)/overlay_full $(BOARDDIR)/overlay $(PLATDIR)/overlay

$(call inherit-product-if-exists, vendor/sprd/operator/cucc/specA.mk)
$(call inherit-product-if-exists, vendor/sprd/UniverseUI/ThemeRes/universeui.mk)
$(call inherit-product, vendor/sprd/partner/shark/bluetooth/device-shark-bt.mk)

# SprdLauncher2
PRODUCT_PACKAGES += \
        SprdLauncher2

PRODUCT_PACKAGES := $(filter-out PinyinIME, $(PRODUCT_PACKAGES))
