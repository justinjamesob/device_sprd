
# storage init files
ifeq ($(STORAGE_INTERNAL), emulated)
  ifeq ($(STORAGE_PRIMARY), internal)
    PRODUCT_COPY_FILES += \
	    $(PLATDIR)/init.storage1.rc:root/init.storage.rc
    PRODUCT_PACKAGE_OVERLAYS := $(PLATDIR)/storage1_overlay
  endif
  ifeq ($(STORAGE_PRIMARY), external)
    PRODUCT_COPY_FILES += \
	    $(PLATDIR)/init.storage2.rc:root/init.storage.rc
    PRODUCT_PACKAGE_OVERLAYS := $(PLATDIR)/storage2_overlay
  endif
endif

ifeq ($(STORAGE_INTERNAL), physical)
  ifeq ($(STORAGE_PRIMARY), internal)
    PRODUCT_COPY_FILES += \
	    $(PLATDIR)/init.storage3.rc:root/init.storage.rc
    PRODUCT_PACKAGE_OVERLAYS := $(PLATDIR)/storage3_overlay
  endif
  ifeq ($(STORAGE_PRIMARY), external)
    PRODUCT_COPY_FILES += \
	    $(PLATDIR)/init.storage4.rc:root/init.storage.rc
    PRODUCT_PACKAGE_OVERLAYS := $(PLATDIR)/storage4_overlay
  endif
endif

