-include vendor/sprd/operator/cucc/specA/res/boot/boot_res.mk

include device/sprd/scx15_sp6815ga/scx15_sp6815gaplus.mk

PRODUCT_NAME := scx15_sp6815gacmccspecAplus_UUI

$(call inherit-product-if-exists, vendor/sprd/operator/cucc/specA.mk)
$(call inherit-product-if-exists, vendor/sprd/UniverseUI/ThemeRes/universeui.mk)
$(call inherit-product, vendor/sprd/partner/shark/bluetooth/device-shark-bt.mk)
