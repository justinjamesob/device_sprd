#
# Copyright (C) 2014 Spreadtrum Communication Inc.
#

# The uui and cmcc spec may share the configs as non-uui, XXX so DO NOT add
# additional settings here about board, only add the vendor specific configs.

# XXX I will REVERT the config which is duplicated!
#security support config
$(call inherit-product-if-exists, vendor/sprd/open-source/security_support.mk)
# TODO Add the customized boot animation here.

include device/sprd/scx35l_sp9630ea_4mod/scx35l_sp9630ea_4mod_dt.mk

include vendor/sprd/UniverseUI/ThemeRes/universeui.mk
include vendor/sprd/operator/cmcc/spec3.mk

PRODUCT_PROPERTY_OVERRIDES += \
                        persist.sys.support.vt=false

# Build the SprdLauncher1
PRODUCT_PACKAGES += \
    SprdLauncher1

# Overrides
PRODUCT_NAME := scx35l_sp9630ea_4mod_dt_base_cuccspec_UUI
PRODUCT_DEVICE := $(TARGET_BOARD)
PRODUCT_MODEL := sp9630ea_4mod
PRODUCT_BRAND := Spreadtrum
PRODUCT_MANUFACTURER := Spreadtrum

PRODUCT_LOCALES := zh_CN zh_TW en_US
