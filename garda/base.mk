
MALI := libUMP libEGL_mali.so libGLESv1_CM_mali.so libGLESv2_mali.so libMali.so libboost.so ump.ko mali.ko



SPRD_FM_APP := FMPlayer

BRCM_FM := \
    fm.$(TARGET_PLATFORM) \
    FmDaemon \
    FmTest

PRODUCT_PROPERTY_OVERRIDES :=

# original apps copied from generic_no_telephony.mk
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
	hostapd \
	wpa_supplicant.conf \
	hciconfig \
	hcitool \
	hcidump \
	bttest\
	calibration_init \
	rawdatad \
	nvm_daemon \
	modemd\
	audio.a2dp.default

# own copyright packages files
PRODUCT_PACKAGES += \
    AppBackup \
    AudioProfile \
    NoteBook \
    CallFireWall \
    ValidationTools \
    libvalidationtoolsjni \
    vtserver

# prebuild files
PRODUCT_PACKAGES += \
    ES_File_Explorer.apk

PRODUCT_PACKAGES += \
	charge \
	vcharged \
	mfserial \
	poweroff_alarm \
	mplayer \
	sqlite3 \
	gralloc.$(TARGET_PLATFORM) \
	hwcomposer.$(TARGET_PLATFORM) \
	camera.$(TARGET_PLATFORM) \
	libisp.so \
	lights.$(TARGET_PLATFORM) \
	audio.primary.$(TARGET_PLATFORM) \
	audio_policy.$(TARGET_PLATFORM) \
	tinymix \
	libvbeffect \
	libvbpga \
	sensors.$(TARGET_BOARD)  \
	$(MALI) \
	hciconfig \
	hcitool \
	hcidump \
	bttest\
	hostapd \
	wpa_supplicant.conf

PRODUCT_PACKAGES += \
            $(BRCM_FM) \
            $(SPRD_FM_APP)
PRODUCT_COPY_FILES := \
	$(BOARDDIR)/init.rc:root/init.rc \
	$(BOARDDIR)/init.sc8825.rc:root/init.sc8825.rc \
	$(BOARDDIR)/init.sc8825.usb.rc:root/init.sc8825.usb.rc \
	$(BOARDDIR)/ueventd.sc8825.rc:root/ueventd.sc8825.rc \
	$(BOARDDIR)/fstab.sc8825:root/fstab.sc8825 \
	$(BOARDDIR)/vold.fstab:system/etc/vold.fstab \
	device/sprd/common/libs/audio/apm/devicevolume.xml:system/etc/devicevolume.xml \
	device/sprd/common/libs/audio/apm/formatvolume.xml:system/etc/formatvolume.xml \
        $(BOARDDIR)/hw_params/tiny_hw.xml:system/etc/tiny_hw.xml \
        $(BOARDDIR)/hw_params/codec_pga.xml:system/etc/codec_pga.xml \
	$(BOARDDIR)/hw_params/audio_hw.xml:system/etc/audio_hw.xml \
        $(BOARDDIR)/hw_params/audio_para:system/etc/audio_para \
	$(BOARDDIR)/scripts/ext_symlink.sh:system/bin/ext_symlink.sh \
	$(BOARDDIR)/scripts/ext_data.sh:system/bin/ext_data.sh \
	$(BOARDDIR)/scripts/ext_kill.sh:system/bin/ext_kill.sh \
	$(BOARDDIR)/scripts/ext_chown.sh:system/bin/ext_chown.sh \
	device/sprd/common/libs/mali/egl.cfg:system/lib/egl/egl.cfg \
	device/sprd/common/libs/audio/audio_policy.conf:system/etc/audio_policy.conf \
	device/sprd/common/res/media/media_codecs.xml:system/etc/media_codecs.xml \
	device/sprd/common/res/media/media_profiles.xml:system/etc/media_profiles.xml \
	device/sprd/common/res/apn/apns-conf.xml:system/etc/apns-conf.xml \
	frameworks/native/data/etc/android.hardware.bluetooth.xml:system/etc/permissions/android.hardware.bluetooth.xml \
	frameworks/native/data/etc/android.hardware.wifi.xml:system/etc/permissions/android.hardware.wifi.xml \
	$(BOARDDIR)/scripts/ext_symlink.sh:system/bin/ext_symlink.sh \
	$(BOARDDIR)/scripts/ext_data.sh:system/bin/ext_data.sh



BOARD_WLAN_DEVICE_REV       := bcm4330_b2
$(call inherit-product, hardware/broadcom/wlan/bcmdhd/firmware/bcm4330/device-bcm.mk)
$(call inherit-product, $(BOARDDIR)/../common/apps/engineeringmodel/module.mk)
$(call inherit-product, $(BOARDDIR)/../common/apps/modemassert/module.mk)
