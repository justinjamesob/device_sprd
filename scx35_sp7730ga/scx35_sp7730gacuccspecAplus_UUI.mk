-include vendor/sprd/operator/cucc/specA/res/boot/boot_res.mk

PRODUCT_THEME_PACKAGES := SimpleStyle

# export original make file
LOCAL_ORIGINAL_PRODUCT_MAKEFILE := device/sprd/scx35_sp7730ga/scx35_sp7730gaplus.mk

include $(LOCAL_ORIGINAL_PRODUCT_MAKEFILE)
# Rename product name and we can lunch it
PRODUCT_NAME := scx35_sp7730gacuccspecAplus_UUI

$(call inherit-product, vendor/sprd/operator/cucc/specA.mk)
$(call inherit-product-if-exists, vendor/sprd/UniverseUI/ThemeRes/universeui.mk)
$(call inherit-product-if-exists, frameworks/base/data/videos/VideoPackageForUUI.mk)
$(call inherit-product, vendor/sprd/partner/shark/bluetooth/device-shark-bt.mk)

# SprdLauncher2
PRODUCT_PACKAGES += \
        SprdLauncher2

