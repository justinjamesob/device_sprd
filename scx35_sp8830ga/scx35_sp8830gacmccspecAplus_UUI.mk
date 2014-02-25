# export original make file
LOCAL_ORIGINAL_PRODUCT_MAKEFILE := device/sprd/scx35_sp8830ga/scx35_sp8830gaplus.mk

PRODUCT_THEME_PACKAGES := SimpleStyle
PRODUCT_THEME_FLAGS := shrink

include $(LOCAL_ORIGINAL_PRODUCT_MAKEFILE)
# Rename product name and we can lunch it
PRODUCT_NAME := scx35_sp8830gacmccspecAplus_UUI

PRODUCT_PACKAGES += \
    SprdLauncher1

DEVICE_PACKAGE_OVERLAYS := $(PLATDIR)/overlay_full $(BOARDDIR)/overlay $(PLATDIR)/overlay

$(call inherit-product, vendor/sprd/operator/cmcc/specA.mk)
$(call inherit-product-if-exists, vendor/sprd/UniverseUI/ThemeRes/universeui.mk)
$(call inherit-product, vendor/sprd/partner/shark/bluetooth/device-shark-bt.mk)