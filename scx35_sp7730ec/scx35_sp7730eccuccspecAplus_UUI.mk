-include vendor/sprd/operator/cucc/specA/res/boot/boot_res_qhd.mk

# export original make file
LOCAL_ORIGINAL_PRODUCT_MAKEFILE := device/sprd/scx35_sp7730ec/scx35_sp7730ecplus.mk

include $(LOCAL_ORIGINAL_PRODUCT_MAKEFILE)
# Rename product name and we can lunch it
PRODUCT_NAME := scx35_sp7730eccuccspecAplus_UUI

$(call inherit-product, vendor/sprd/operator/cucc/specA.mk)
$(call inherit-product-if-exists, vendor/sprd/UniverseUI/ThemeRes/universeui.mk)
$(call inherit-product-if-exists, frameworks/base/data/videos/VideoPackageForUUI.mk)
$(call inherit-product, vendor/sprd/partner/shark/bluetooth/device-shark-bt.mk)

PRODUCT_PROPERTY_OVERRIDES += \
        ro.homekey.physical=true

# SprdLauncher1
PRODUCT_PACKAGES += \
        SprdLauncher1

# SprdLauncher2
PRODUCT_PACKAGES += \
        SprdLauncher2

