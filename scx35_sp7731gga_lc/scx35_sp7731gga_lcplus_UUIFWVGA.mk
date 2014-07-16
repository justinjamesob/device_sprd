-include vendor/sprd/operator/cucc/specA/res/boot/boot_res_fwvga.mk

PRODUCT_THEME_PACKAGES := SimpleStyle HelloColor
PRODUCT_THEME_FLAGS := shrink

# export original make file
LOCAL_ORIGINAL_PRODUCT_MAKEFILE := device/sprd/scx35_sp7731gga_lc/scx35_sp7731gga_lcplus.mk

include $(LOCAL_ORIGINAL_PRODUCT_MAKEFILE)
# Rename product name and we can lunch it
PRODUCT_NAME := scx35_sp7731gga_lcplus_UUIFWVGA

PRODUCT_PROPERTY_OVERRIDES += \
        persist.sys.support.vt=false \
        persist.surpport.50ksearch=0 \
	ro.sf.lcd_width=48 \
	ro.sf.lcd_height=86 \
	lmk.autocalc=false \
	ro.board_ram_size=low

DEVICE_PACKAGE_OVERLAYS := $(PLATDIR)/overlay_full $(DEVICE_PACKAGE_OVERLAYS)

# Remove video wallpaper application and resources
PRODUCT_VIDEO_WALLPAPERS := none

include vendor/sprd/UniverseUI/ThemeRes/universeui.mk
$(call inherit-product, vendor/sprd/partner/shark/bluetooth/device-shark-bt.mk)
ifeq ($(strip $(GMS_SUPPORT)), true)
$(call inherit-product-if-exists, vendor/sprd/partner/google/products/gms.mk)
endif

# SprdLauncher1
PRODUCT_PACKAGES += \
        SprdLauncher1

#[[ for autotest
PRODUCT_PACKAGES += autotest
#]]

