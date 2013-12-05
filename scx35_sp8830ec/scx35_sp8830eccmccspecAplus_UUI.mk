# export original make file
LOCAL_ORIGINAL_PRODUCT_MAKEFILE := device/sprd/scx35_sp8830ec/scx35_sp8830ecplus.mk

include $(LOCAL_ORIGINAL_PRODUCT_MAKEFILE)
# Rename product name and we can lunch it
PRODUCT_NAME := scx35_sp8830eccmccspecAplus_UUI

$(call inherit-product, vendor/sprd/operator/cmcc/specA.mk)
$(call inherit-product-if-exists, vendor/sprd/UniverseUI/universeui.mk)
