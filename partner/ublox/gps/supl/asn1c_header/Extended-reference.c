/*
 * Generated by asn1c-0.9.22 (http://lionet.info/asn1c)
 * From ASN.1 module "RRLP-Components"
 * 	found in "rrlp-components.asn"
 * 	`asn1c -gen-PER -fnative-types`
 */

#include "Extended-reference.h"

static int
memb_smlc_code_constraint_1(asn_TYPE_descriptor_t *td, const void *sptr,
			asn_app_constraint_failed_f *ctfailcb, void *app_key) {
	long value;
	
	if(!sptr) {
		_ASN_CTFAIL(app_key, td, sptr,
			"%s: value not given (%s:%d)",
			td->name, __FILE__, __LINE__);
		return -1;
	}
	
	value = *(const long *)sptr;
	
	if((value >= 0 && value <= 63)) {
		/* Constraint check succeeded */
		return 0;
	} else {
		_ASN_CTFAIL(app_key, td, sptr,
			"%s: constraint failed (%s:%d)",
			td->name, __FILE__, __LINE__);
		return -1;
	}
}

static int
memb_transaction_ID_constraint_1(asn_TYPE_descriptor_t *td, const void *sptr,
			asn_app_constraint_failed_f *ctfailcb, void *app_key) {
	long value;
	
	if(!sptr) {
		_ASN_CTFAIL(app_key, td, sptr,
			"%s: value not given (%s:%d)",
			td->name, __FILE__, __LINE__);
		return -1;
	}
	
	value = *(const long *)sptr;
	
	if((value >= 0 && value <= 262143)) {
		/* Constraint check succeeded */
		return 0;
	} else {
		_ASN_CTFAIL(app_key, td, sptr,
			"%s: constraint failed (%s:%d)",
			td->name, __FILE__, __LINE__);
		return -1;
	}
}

static asn_per_constraints_t asn_PER_memb_smlc_code_constr_2 = {
	{ APC_CONSTRAINED,	 6,  6,  0,  63 }	/* (0..63) */,
	{ APC_UNCONSTRAINED,	-1, -1,  0,  0 },
	0, 0	/* No PER value map */
};
static asn_per_constraints_t asn_PER_memb_transaction_ID_constr_3 = {
	{ APC_CONSTRAINED,	 18, -1,  0,  262143 }	/* (0..262143) */,
	{ APC_UNCONSTRAINED,	-1, -1,  0,  0 },
	0, 0	/* No PER value map */
};
static asn_TYPE_member_t asn_MBR_Extended_reference_1[] = {
	{ ATF_NOFLAGS, 0, offsetof(struct Extended_reference, smlc_code),
		(ASN_TAG_CLASS_CONTEXT | (0 << 2)),
		-1,	/* IMPLICIT tag at current level */
		&asn_DEF_NativeInteger,
		memb_smlc_code_constraint_1,
		&asn_PER_memb_smlc_code_constr_2,
		0,
		"smlc-code"
		},
	{ ATF_NOFLAGS, 0, offsetof(struct Extended_reference, transaction_ID),
		(ASN_TAG_CLASS_CONTEXT | (1 << 2)),
		-1,	/* IMPLICIT tag at current level */
		&asn_DEF_NativeInteger,
		memb_transaction_ID_constraint_1,
		&asn_PER_memb_transaction_ID_constr_3,
		0,
		"transaction-ID"
		},
};
static ber_tlv_tag_t asn_DEF_Extended_reference_tags_1[] = {
	(ASN_TAG_CLASS_UNIVERSAL | (16 << 2))
};
static asn_TYPE_tag2member_t asn_MAP_Extended_reference_tag2el_1[] = {
    { (ASN_TAG_CLASS_CONTEXT | (0 << 2)), 0, 0, 0 }, /* smlc-code at 830 */
    { (ASN_TAG_CLASS_CONTEXT | (1 << 2)), 1, 0, 0 } /* transaction-ID at 831 */
};
static asn_SEQUENCE_specifics_t asn_SPC_Extended_reference_specs_1 = {
	sizeof(struct Extended_reference),
	offsetof(struct Extended_reference, _asn_ctx),
	asn_MAP_Extended_reference_tag2el_1,
	2,	/* Count of tags in the map */
	0, 0, 0,	/* Optional elements (not needed) */
	-1,	/* Start extensions */
	-1	/* Stop extensions */
};
asn_TYPE_descriptor_t asn_DEF_Extended_reference = {
	"Extended-reference",
	"Extended-reference",
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
	asn_DEF_Extended_reference_tags_1,
	sizeof(asn_DEF_Extended_reference_tags_1)
		/sizeof(asn_DEF_Extended_reference_tags_1[0]), /* 1 */
	asn_DEF_Extended_reference_tags_1,	/* Same as above */
	sizeof(asn_DEF_Extended_reference_tags_1)
		/sizeof(asn_DEF_Extended_reference_tags_1[0]), /* 1 */
	0,	/* No PER visible constraints */
	asn_MBR_Extended_reference_1,
	2,	/* Elements count */
	&asn_SPC_Extended_reference_specs_1	/* Additional specs */
};

