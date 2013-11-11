/*
 * Generated by asn1c-0.9.22 (http://lionet.info/asn1c)
 * From ASN.1 module "MAP-ExtensionDataTypes"
 * 	found in "MAP-ExtensionDataTypes.asn"
 * 	`asn1c -gen-PER -fnative-types`
 */

#ifndef	_ExtensionContainer_H_
#define	_ExtensionContainer_H_


#include <asn_application.h>

/* Including external dependencies */
#include <constr_SEQUENCE.h>

#ifdef __cplusplus
extern "C" {
#endif

/* Forward declarations */
struct PrivateExtensionList;
struct PCS_Extensions;

/* ExtensionContainer */
typedef struct ExtensionContainer {
	struct PrivateExtensionList	*privateExtensionList	/* OPTIONAL */;
	struct PCS_Extensions	*pcs_Extensions	/* OPTIONAL */;
	/*
	 * This type is extensible,
	 * possible extensions are below.
	 */
	
	/* Context for parsing across buffer boundaries */
	asn_struct_ctx_t _asn_ctx;
} ExtensionContainer_t;

/* Implementation */
extern asn_TYPE_descriptor_t asn_DEF_ExtensionContainer;

#ifdef __cplusplus
}
#endif

/* Referred external types */
#include "PrivateExtensionList.h"
#include "PCS-Extensions.h"

#endif	/* _ExtensionContainer_H_ */
#include <asn_internal.h>