-include vendor/sprd/operator/cucc/specA/res/boot/boot_res.mk

include device/sprd/scx15_sp7715ga/scx15_sp7715gaplus.mk

PRODUCT_NAME := scx15_sp7715gacuccspecAplus_UUI

$(call inherit-product-if-exists, vendor/sprd/operator/cucc/specA.mk)
$(call inherit-product-if-exists, vendor/sprd/UniverseUI/ThemeRes/universeui.mk)
