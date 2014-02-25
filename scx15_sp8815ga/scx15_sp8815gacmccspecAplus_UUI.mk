include device/sprd/scx15_sp8815ga/scx15_sp8815gaplus.mk

PRODUCT_NAME := scx15_sp8815gacmccspecAplus_UUI

PRODUCT_PACKAGES += \
    SprdLauncher1

PRODUCT_THEME_PAKCAGES := SimpleStyle
PRODUCT_THEME_FLAGS := shrink

DEVICE_PACKAGE_OVERLAYS := $(PLATDIR)/overlay_full $(BOARDDIR)/overlay $(PLATDIR)/overlay

$(call inherit-product, vendor/sprd/operator/cmcc/specA.mk)
$(call inherit-product-if-exists, vendor/sprd/UniverseUI/ThemeRes/universeui.mk)
$(call inherit-product, vendor/sprd/partner/shark/bluetooth/device-shark-bt.mk)
