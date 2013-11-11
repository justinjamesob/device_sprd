
# include aosp base configs
$(call inherit-product, $(SRC_TARGET_DIR)/product/full_base.mk)

# sprd telephony
PRODUCT_PACKAGES += \
	Dialer \
	Mms

# graphics modules
PRODUCT_PACKAGES += \
	libUMP \
	libEGL_mali.so \
	libGLESv1_CM_mali.so \
	libGLESv2_mali.so \
	libMali.so \
	libboost.so \
	ump.ko \
	mali.ko

# video modules
#PRODUCT_PACKAGES += \
#	libstagefright_sprd_mpeg4enc \
#	libstagefright_sprd_mpeg4dec \
#	libstagefright_sprd_h264dec \
#	libstagefright_sprd_h264enc \
#	libstagefright_sprd_vpxdec \
#	libstagefright_soft_mjpgdec \
#	libstagefright_sprd_aacdec \
#	libstagefright_sprd_mp3dec

# default audio
PRODUCT_PACKAGES += \
	audio.a2dp.default \
	audio.usb.default \
	audio.r_submix.default \
	libaudio-resampler

# sprd HAL modules
PRODUCT_PACKAGES += \
	gralloc.sc8830 \
#	hwcomposer.sc8830 \
#	audio.primary.sc8830 \
#	audio_policy.sc8830 \
#	camera.sc8830 \
#	lights.sc8830 \
#	sensors.sc8830

# misc modules
PRODUCT_PACKAGES += \
	sqlite3 \
	e2fsck \
	tinymix \
	calibration_init \
	modemd \
	engpc \
	libengappjni \
	Engapp \
	modemassert \
	nvitemd \
	batterysrv \
	refnotify

# general configs
PRODUCT_COPY_FILES += \
	$(PLATDIR)/init.sc8830.rc:root/init.sc8830.rc \
	$(PLATDIR)/init.sc8830.usb.rc:root/init.sc8830.usb.rc \
	$(PLATDIR)/ueventd.sc8830.rc:root/ueventd.sc8830.rc \
	$(PLATDIR)/fstab.sc8830:root/fstab.sc8830 \
	$(PLATDIR)/headset-keyboard.kl:system/usr/keylayout/headset-keyboard.kl \
	$(PLATDIR)/sci-keypad.kl:system/usr/keylayout/sci-keypad.kl \
	$(PLATDIR)/media_codecs.xml:system/etc/media_codecs.xml \
	$(PLATDIR)/media_profiles.xml:system/etc/media_profiles.xml \
	$(PLATDIR)/nvitem_td.cfg:system/etc/nvitem_td.cfg \
        vendor/sprd/open-source/res/spn/spn-conf.xml:system/etc/spn-conf.xml \
	vendor/sprd/open-source/res/apn/apns-conf.xml:system/etc/apns-conf.xml \
	vendor/sprd/open-source/res/productinfo/productinfo.bin:prodnv/productinfo.bin \
	vendor/sprd/open-source/res/CDROM/adb.iso:system/etc/adb.iso \
	vendor/sprd/open-source/libs/audio/apm/devicevolume.xml:system/etc/devicevolume.xml \
	vendor/sprd/open-source/libs/audio/apm/formatvolume.xml:system/etc/formatvolume.xml \
	vendor/sprd/open-source/libs/audio/audio_policy.conf:system/etc/audio_policy.conf \
	vendor/sprd/open-source/libs/mali/egl.cfg:system/lib/egl/egl.cfg \
	frameworks/native/data/etc/handheld_core_hardware.xml:system/etc/permissions/handheld_core_hardware.xml \
	frameworks/native/data/etc/android.hardware.bluetooth.xml:system/etc/permissions/android.hardware.bluetooth.xml \
	frameworks/native/data/etc/android.hardware.camera.autofocus.xml:system/etc/permissions/android.hardware.camera.autofocus.xml \
	frameworks/native/data/etc/android.hardware.camera.flash-autofocus.xml:system/etc/permissions/android.hardware.camera.flash-autofocus.xml \
	frameworks/native/data/etc/android.hardware.location.gps.xml:system/etc/permissions/android.hardware.location.gps.xml \
        frameworks/native/data/etc/android.hardware.sensor.light.xml:system/etc/permissions/android.hardware.sensor.light.xml \
        frameworks/native/data/etc/android.hardware.sensor.proximity.xml:system/etc/permissions/android.hardware.sensor.proximity.xml \
        frameworks/native/data/etc/android.hardware.sensor.accelerometer.xml:system/etc/permissions/android.hardware.sensor.accelerometer.xml \
	frameworks/native/data/etc/android.hardware.touchscreen.multitouch.xml:system/etc/permissions/android.hardware.touchscreen.multitouch.xml \
	frameworks/native/data/etc/android.hardware.touchscreen.xml:system/etc/permissions/android.hardware.touchscreen.xml \
	frameworks/native/data/etc/android.hardware.telephony.gsm.xml:system/etc/permissions/android.hardware.telephony.gsm.xml \
	frameworks/native/data/etc/android.hardware.usb.accessory.xml:system/etc/permissions/android.hardware.usb.accessory.xml \
	frameworks/native/data/etc/android.hardware.wifi.xml:system/etc/permissions/android.hardware.wifi.xml

$(call inherit-product, vendor/sprd/open-source/apps/engineeringmodel/module.mk)

ifeq ($(TARGET_BUILD_VARIANT),user)
	PRODUCT_PROPERTY_OVERRIDES += persist.sys.sprd.modemreset=1
else
	PRODUCT_PROPERTY_OVERRIDES += persist.sys.sprd.modemreset=0
endif

