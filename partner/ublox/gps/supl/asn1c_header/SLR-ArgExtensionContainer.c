/*
 * Generated by asn1c-0.9.22 (http://lionet.info/asn1c)
 * From ASN.1 module "MAP-ExtensionDataTypes"
 * 	found in "MAP-ExtensionDataTypes.asn"
 * 	`asn1c -gen-PER -fnative-types`
 */

#include "SLR-ArgExtensionContainer.h"

static asn_TYPE_member_t asn_MBR_SLR_ArgExtensionContainer_1[] = {
	{ ATF_POINTER, 2, offsetof(struct SLR_ArgExtensionContainer, privateExtensionList),
		(ASN_TAG_CLASS_CONTEXT | (0 << 2)),
		-1,	/* IMPLICIT tag at current level */
		&asn_DEF_PrivateExtensionList,
		0,	/* Defer constraints checking to the member type */
		0,	/* No PER visible constraints */
		0,
		"privateExtensionList"
		},
	{ ATF_POINTER, 1, offsetof(struct SLR_ArgExtensionContainer, slr_Arg_PCS_Extensions),
		(ASN_TAG_CLASS_CONTEXT | (1 << 2)),
		-1,	/* IMPLICIT tag at current level */
		&asn_DEF_SLR_Arg_PCS_Extensions,
		0,	/* Defer constraints checking to the member type */
		0,	/* No PER visible constraints */
		0,
		"slr-Arg-PCS-Extensions"
		},
};
static int asn_MAP_SLR_ArgExtensionContainer_oms_1[] = { 0, 1 };
static ber_tlv_tag_t asn_DEF_SLR_ArgExtensionContainer_tags_1[] = {
	(ASN_TAG_CLASS_UNIVERSAL | (16 << 2))
};
static asn_TYPE_tag2member_t asn_MAP_SLR_ArgExtensionContainer_tag2el_1[] = {
    { (ASN_TAG_CLASS_CONTEXT | (0 << 2)), 0, 0, 0 }, /* privateExtensionList at 33 */
    { (ASN_TAG_CLASS_CONTEXT | (1 << 2)), 1, 0, 0 } /* slr-Arg-PCS-Extensions at 34 */
};
static asn_SEQUENCE_specifics_t asn_SPC_SLR_ArgExtensionContainer_specs_1 = {
	sizeof(struct SLR_ArgExtensionContainer),
	offsetof(struct SLR_ArgExtensionContainer, _asn_ctx),
	asn_MAP_SLR_ArgExtensionContainer_tag2el_1,
	2,	/* Count of tags in the map */
	asn_MAP_SLR_ArgExtensionContainer_oms_1,	/* Optional members */
	2, 0,	/* Root/Additions */
	1,	/* Start extensions */
	3	/* Stop extensions */
};
asn_TYPE_descriptor_t asn_DEF_SLR_ArgExtensionContainer = {
	"SLR-ArgExtensionContainer",
	"SLR-ArgExtensionContainer",
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
	asn_DEF_SLR_ArgExtensionContainer_tags_1,
	sizeof(asn_DEF_SLR_ArgExtensionContainer_tags_1)
		/sizeof(asn_DEF_SLR_ArgExtensionContainer_tags_1[0]), /* 1 */
	asn_DEF_SLR_ArgExtensionContainer_tags_1,	/* Same as above */
	sizeof(asn_DEF_SLR_ArgExtensionContainer_tags_1)
		/sizeof(asn_DEF_SLR_ArgExtensionContainer_tags_1[0]), /* 1 */
	0,	/* No PER visible constraints */
	asn_MBR_SLR_ArgExtensionContainer_1,
	2,	/* Elements count */
	&asn_SPC_SLR_ArgExtensionContainer_specs_1	/* Additional specs */
};

