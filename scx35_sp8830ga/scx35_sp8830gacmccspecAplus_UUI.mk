# export original make file
LOCAL_ORIGINAL_PRODUCT_MAKEFILE := device/sprd/scx35_sp8830ga/scx35_sp8830gaplus.mk

include $(LOCAL_ORIGINAL_PRODUCT_MAKEFILE)
# Rename product name and we can lunch it
PRODUCT_NAME := scx35_sp8830gacmccspecAplus_UUI

# TODO The SprdLauncher2 is only used to test in sp8830ec, it will be removed
# in future
PRODUCT_PACKAGES += \
    SprdLauncher2

PRODUCT_PACKAGES += \
    SprdLauncher1

$(call inherit-product, vendor/sprd/operator/cmcc/specA.mk)
$(call inherit-product-if-exists, vendor/sprd/UniverseUI/ThemeRes/universeui.mk)
$(call inherit-product, vendor/sprd/partner/shark/bluetooth/device-shark-bt.mk)