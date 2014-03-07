PRODUCT_COPY_FILES += \
	$(PLATDIR)/emmc/fstab.sc8830:root/fstab.sc8830 \
	$(PLATDIR)/emmc/nvitem_w.cfg:system/etc/nvitem_w.cfg \
	$(PLATDIR)/emmc/nvitem_td.cfg:system/etc/nvitem_td.cfg

PRODUCT_PROPERTY_OVERRIDES += \
	ro.storage.flash_type=2
