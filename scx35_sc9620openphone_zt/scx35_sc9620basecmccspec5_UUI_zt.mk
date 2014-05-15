# export original make file
LOCAL_ORIGINAL_PRODUCT_MAKEFILE := device/sprd/scx35_sc9620openphone_zt/scx35_sc9620openphone_ztbase.mk

include $(LOCAL_ORIGINAL_PRODUCT_MAKEFILE)
# Rename product name and we can lunch it
PRODUCT_NAME := scx35_sc9620basecmccspec5_UUI_zt

PRODUCT_PROPERTY_OVERRIDES += \
        persist.sys.support.vt=false

PRODUCT_PACKAGES += \
    SprdLauncher1

DEVICE_PACKAGE_OVERLAYS := $(PLATDIR)/overlay_full $(BOARDDIR)/overlay $(PLATDIR)/overlay

include vendor/sprd/UniverseUI/ThemeRes/universeui.mk
include vendor/sprd/operator/cmcc/spec3.mk

#security support config
$(call inherit-product-if-exists, vendor/sprd/open-source/security_support.mk)

