# export original make file
LOCAL_ORIGINAL_PRODUCT_MAKEFILE := device/sprd/scx35_sp7730ec/scx35_sp7730ecplus.mk

include $(LOCAL_ORIGINAL_PRODUCT_MAKEFILE)
# Rename product name and we can lunch it
PRODUCT_NAME := scx35_sp7730eccmccspecAplus_UUI

$(call inherit-product, vendor/sprd/operator/cmcc/specA.mk)
$(call inherit-product-if-exists, vendor/sprd/UniverseUI/ThemeRes/universeui.mk)
$(call inherit-product-if-exists, frameworks/base/data/videos/VideoPackageForUUI.mk)
$(call inherit-product, vendor/sprd/partner/shark/bluetooth/device-shark-bt.mk) 
