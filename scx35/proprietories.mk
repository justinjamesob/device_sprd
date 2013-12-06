
ifneq ($(shell ls -d vendor/sprd/proprietories-source 2>/dev/null),)
# for spreadtrum internal proprietories modules: only support package module names

OPENMAX := libomx_m4vh263dec_sw_sprd libomx_m4vh263dec_hw_sprd libomx_m4vh263enc_hw_sprd \
	libomx_avcdec_hw_sprd libomx_avcdec_sw_sprd libomx_avcenc_hw_sprd libomx_vpxdec_hw_sprd

# include PRODUCT_PACKAGES
include device/sprd/common/product_packages.mk

else
# for spreadtrum customer proprietories modules: only support direct copy

PROPMODS := \
	system/lib/libomx_m4vh263dec_sw_sprd.so \
	system/lib/libomx_m4vh263dec_hw_sprd.so	\
	system/lib/libomx_m4vh263enc_hw_sprd.so \
	system/lib/libomx_avcdec_hw_sprd.so \
	system/lib/libomx_avcdec_sw_sprd.so \
	system/lib/libomx_avcenc_hw_sprd.so	\
	system/lib/libomx_vpxdec_hw_sprd.so	\
	system/bin/rild_sp \
	system/lib/libril_sp.so \
	system/lib/libreference-ril_sp.so \
	system/bin/phoneserver

PRODUCT_COPY_FILES := $(foreach f,$(PROPMODS),vendor/sprd/proprietories/scx35/$(f):$(f))

endif
