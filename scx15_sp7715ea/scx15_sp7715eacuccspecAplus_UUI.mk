include device/sprd/scx15_sp7715ea/scx15_sp7715eaplus.mk

PRODUCT_NAME := scx15_sp7715eacuccspecAplus_UUI

$(call inherit-product, vendor/sprd/operator/cucc/specA.mk)
$(call inherit-product-if-exists, vendor/sprd/UniverseUI/ThemeRes/universeui.mk)