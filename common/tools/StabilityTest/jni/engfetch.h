/* DO NOT EDIT THIS FILE - it is machine generated */
#include <jni.h>
#include <stdio.h>

#ifndef _Included_engfetch_stability
#define _Included_engfetch_stability
#ifdef __cplusplus
extern "C" {
#endif

#define MAX_SN_LEN  (24)
#define SP09_MAX_SN_LEN 				    MAX_SN_LEN
#define SP09_MAX_STATION_NUM		   	    (15)
#define SP09_MAX_STATION_NAME_LEN		    (10)
#define SP09_SPPH_MAGIC_NUMBER             (0X53503039)
#define SP09_MAX_LAST_DESCRIPTION_LEN      (32)

typedef struct _tagSP09_PHASE_CHECK
{
	unsigned long Magic;
	char    	SN1[SP09_MAX_SN_LEN];
	char    	SN2[SP09_MAX_SN_LEN];
	int     	StationNum;
	char    	StationName[SP09_MAX_STATION_NUM][SP09_MAX_STATION_NAME_LEN];
	unsigned char 	Reserved[13];
	unsigned char 	SignFlag;
	char    	szLastFailDescription[SP09_MAX_LAST_DESCRIPTION_LEN];
	unsigned short  iTestSign;
	unsigned short  iItem;
}SP09_PHASE_CHECK_T, *LPSP09_PHASE_CHECK_T;

int eng_getphasecheck(SP09_PHASE_CHECK_T* phase_check);

int engapi_getphasecheck(void* buf, int size);


/********************************************************************************/

#ifdef __cplusplus
}
#endif
#endif

