PRODUCT_COPY_FILES += \
	$(PLATDIR)/nand/fstab.sc8830:root/fstab.sc8830 \
	$(PLATDIR)/nand/nvitem_td.cfg:system/etc/nvitem_td.cfg \
	$(PLATDIR)/nand/nvitem_w.cfg:system/etc/nvitem_w.cfg
# SPRD:add for mount cache to sdcard @{
PRODUCT_PACKAGES += \
    mke2fs \
    mkcached.sh \
	busybox
# @}
