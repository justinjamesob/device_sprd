/*
 * Generated by asn1c-0.9.22 (http://lionet.info/asn1c)
 * From ASN.1 module "RRLP-Components"
 * 	found in "rrlp-components.asn"
 * 	`asn1c -gen-PER -fnative-types`
 */

#ifndef	_AcquisAssist_H_
#define	_AcquisAssist_H_


#include <asn_application.h>

/* Including external dependencies */
#include "TimeRelation.h"
#include "SeqOfAcquisElement.h"
#include <constr_SEQUENCE.h>

#ifdef __cplusplus
extern "C" {
#endif

/* AcquisAssist */
typedef struct AcquisAssist {
	TimeRelation_t	 timeRelation;
	SeqOfAcquisElement_t	 acquisList;
	
	/* Context for parsing across buffer boundaries */
	asn_struct_ctx_t _asn_ctx;
} AcquisAssist_t;

/* Implementation */
extern asn_TYPE_descriptor_t asn_DEF_AcquisAssist;

#ifdef __cplusplus
}
#endif

#endif	/* _AcquisAssist_H_ */
#include <asn_internal.h>
