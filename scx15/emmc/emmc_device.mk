ifeq ($(strip $(BOARD_KERNEL_SEPARATED_DT)),true)
PRODUCT_COPY_FILES += \
	$(PLATDIR)/emmc/fstab_dt.scx15:root/fstab.scx15
else
PRODUCT_COPY_FILES += \
	$(PLATDIR)/emmc/fstab.scx15:root/fstab.scx15
endif

PRODUCT_PROPERTY_OVERRIDES += \
	ro.storage.flash_type=2

