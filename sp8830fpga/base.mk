
MALI := libUMP libEGL_mali.so libGLESv1_CM_mali.so libGLESv2_mali.so libMali.so libboost.so ump.ko mali.ko

PRODUCT_PROPERTY_OVERRIDES :=

PRODUCT_PACKAGES := \
	DeskClock \
	Bluetooth \
	Calculator \
	Calendar \
	CertInstaller \
	DrmProvider \
	Email \
	Exchange2 \
	Gallery2 \
	InputDevices \
	LatinIME \
	Launcher2 \
	Music \
	MusicFX \
	Provision \
	QuickSearchBox \
	SystemUI \
	CalendarProvider \
	bluetooth-health \
	hciconfig \
	hcitool \
	hcidump \
	bttest\
	hostapd \
	wpa_supplicant.conf \
	audio.a2dp.default

PRODUCT_PACKAGES += \
	nvitemd \
	charge \
	vcharged \
	poweroff_alarm \
	mplayer \
	sqlite3 \
	modemd \
	calibration_init \
	gralloc.$(TARGET_PLATFORM) \
	hwcomposer.$(TARGET_PLATFORM) \
	camera.$(TARGET_PLATFORM) \
        libisp.so \
	lights.$(TARGET_PLATFORM) \
	audio.primary.$(TARGET_PLATFORM) \
	tinymix \
	libvbeffect \
	sensors.$(TARGET_BOARD)  \
	$(MALI)

PRODUCT_COPY_FILES := \
	$(BOARDDIR)/init.rc:root/init.rc \
	$(BOARDDIR)/init.sc8830.rc:root/init.sc8830.rc \
	$(BOARDDIR)/init.sc8830.usb.rc:root/init.sc8830.usb.rc \
	$(BOARDDIR)/ueventd.sc8830.rc:root/ueventd.sc8830.rc \
	$(BOARDDIR)/fstab.sc8830:root/fstab.sc8830 \
	$(BOARDDIR)/vold.fstab:system/etc/vold.fstab \
	device/sprd/common/libs/audio/apm/devicevolume.xml:system/etc/devicevolume.xml \
	device/sprd/common/libs/audio/apm/formatvolume.xml:system/etc/formatvolume.xml \
        $(BOARDDIR)/hw_params/tiny_hw.xml:system/etc/tiny_hw.xml \
        $(BOARDDIR)/hw_params/codec_pga.xml:system/etc/codec_pga.xml \
	$(BOARDDIR)/hw_params/audio_hw.xml:system/etc/audio_hw.xml \
        $(BOARDDIR)/hw_params/audio_para:system/etc/audio_para \
	$(BOARDDIR)/headset-keyboard.kl:system/usr/keylayout/headset-keyboard.kl \
	device/sprd/common/libs/mali/egl.cfg:system/lib/egl/egl.cfg \
	device/sprd/common/libs/audio/audio_policy.conf:system/etc/audio_policy.conf \
	device/sprd/common/res/media/media_codecs.xml:system/etc/media_codecs.xml \
	device/sprd/common/res/media/media_profiles.xml:system/etc/media_profiles.xml \
	frameworks/native/data/etc/android.hardware.bluetooth.xml:system/etc/permissions/android.hardware.bluetooth.xml \
	frameworks/native/data/etc/android.hardware.wifi.xml:system/etc/permissions/android.hardware.wifi.xml \
	device/sprd/common/res/apn/apns-conf.xml:system/etc/apns-conf.xml

BOARD_WLAN_DEVICE_REV       := bcm4330_b2
$(call inherit-product, hardware/broadcom/wlan/bcmdhd/firmware/bcm4330/device-bcm.mk)
