/*
 * Generated by asn1c-0.9.22 (http://lionet.info/asn1c)
 * From ASN.1 module "SUPL-AUTH-RESP"
 * 	found in "supl.asn"
 * 	`asn1c -gen-PER -fnative-types`
 */

#include "SUPLAUTHRESP.h"

static asn_TYPE_member_t asn_MBR_SUPLAUTHRESP_1[] = {
	{ ATF_POINTER, 3, offsetof(struct SUPLAUTHRESP, sPCAuthKey),
		(ASN_TAG_CLASS_CONTEXT | (0 << 2)),
		+1,	/* EXPLICIT tag at current level */
		&asn_DEF_SPCAuthKey,
		0,	/* Defer constraints checking to the member type */
		0,	/* No PER visible constraints */
		0,
		"sPCAuthKey"
		},
	{ ATF_POINTER, 2, offsetof(struct SUPLAUTHRESP, keyIdentity3),
		(ASN_TAG_CLASS_CONTEXT | (1 << 2)),
		-1,	/* IMPLICIT tag at current level */
		&asn_DEF_KeyIdentity3,
		0,	/* Defer constraints checking to the member type */
		0,	/* No PER visible constraints */
		0,
		"keyIdentity3"
		},
	{ ATF_POINTER, 1, offsetof(struct SUPLAUTHRESP, statusCode),
		(ASN_TAG_CLASS_CONTEXT | (2 << 2)),
		-1,	/* IMPLICIT tag at current level */
		&asn_DEF_StatusCode,
		0,	/* Defer constraints checking to the member type */
		0,	/* No PER visible constraints */
		0,
		"statusCode"
		},
};
static int asn_MAP_SUPLAUTHRESP_oms_1[] = { 0, 1, 2 };
static ber_tlv_tag_t asn_DEF_SUPLAUTHRESP_tags_1[] = {
	(ASN_TAG_CLASS_UNIVERSAL | (16 << 2))
};
static asn_TYPE_tag2member_t asn_MAP_SUPLAUTHRESP_tag2el_1[] = {
    { (ASN_TAG_CLASS_CONTEXT | (0 << 2)), 0, 0, 0 }, /* sPCAuthKey at 294 */
    { (ASN_TAG_CLASS_CONTEXT | (1 << 2)), 1, 0, 0 }, /* keyIdentity3 at 295 */
    { (ASN_TAG_CLASS_CONTEXT | (2 << 2)), 2, 0, 0 } /* statusCode at 296 */
};
static asn_SEQUENCE_specifics_t asn_SPC_SUPLAUTHRESP_specs_1 = {
	sizeof(struct SUPLAUTHRESP),
	offsetof(struct SUPLAUTHRESP, _asn_ctx),
	asn_MAP_SUPLAUTHRESP_tag2el_1,
	3,	/* Count of tags in the map */
	asn_MAP_SUPLAUTHRESP_oms_1,	/* Optional members */
	3, 0,	/* Root/Additions */
	2,	/* Start extensions */
	4	/* Stop extensions */
};
asn_TYPE_descriptor_t asn_DEF_SUPLAUTHRESP = {
	"SUPLAUTHRESP",
	"SUPLAUTHRESP",
	SEQUENCE_free,
	SEQUENCE_print,
	SEQUENCE_constraint,
	SEQUENCE_decode_ber,
	SEQUENCE_encode_der,
	SEQUENCE_decode_xer,
	SEQUENCE_encode_xer,
	SEQUENCE_decode_uper,
	SEQUENCE_encode_uper,
	0,	/* Use generic outmost tag fetcher */
	asn_DEF_SUPLAUTHRESP_tags_1,
	sizeof(asn_DEF_SUPLAUTHRESP_tags_1)
		/sizeof(asn_DEF_SUPLAUTHRESP_tags_1[0]), /* 1 */
	asn_DEF_SUPLAUTHRESP_tags_1,	/* Same as above */
	sizeof(asn_DEF_SUPLAUTHRESP_tags_1)
		/sizeof(asn_DEF_SUPLAUTHRESP_tags_1[0]), /* 1 */
	0,	/* No PER visible constraints */
	asn_MBR_SUPLAUTHRESP_1,
	3,	/* Elements count */
	&asn_SPC_SUPLAUTHRESP_specs_1	/* Additional specs */
};

