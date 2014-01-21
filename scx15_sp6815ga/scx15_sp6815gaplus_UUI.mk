include device/sprd/scx15_sp6815ga/scx15_sp6815gaplus.mk

PRODUCT_NAME := scx15_sp6815gaplus_UUI

$(call inherit-product-if-exists, vendor/sprd/UniverseUI/ThemeRes/universeui.mk)
$(call inherit-product, vendor/sprd/partner/shark/bluetooth/device-shark-bt.mk)

# SprdLauncher1
PRODUCT_PACKAGES += \
         SprdLauncher1

PRODUCT_PROPERTY_OVERRIDES += \
	ro.device.support.geocode=false
