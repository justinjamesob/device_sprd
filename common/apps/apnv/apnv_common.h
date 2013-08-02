#ifdef WIN32
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <windows.h>
#define  APNV_PRINT printf
#else
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <pthread.h>
#include "engapi.h"
#include "engat.h"
#define LOG_TAG "APNV"
#include <cutils/log.h>
#define APNV_PRINT ALOGD
#endif

#ifndef _PRODNV_COMMON_H_
#define _PRODNV_COMMON_H_

typedef unsigned char		BOOLEAN;
typedef unsigned char 		uint8;
typedef unsigned short		uint16;
typedef unsigned  int		uint32;

typedef signed char	    	int8;
typedef signed short		int16;
typedef signed int		    int32;
#define TRUE   1;
#define FALSE  0;

//#ifdef __BIG_ENDIAN
//#define htod16(s) ((((s)>>8)&0xff)|(((s)<<8)&0xff00))
//#else
#define htod16(s)  s
//#endif

#ifdef WIN32
#define ORIG_PATH "D:\\pro_nvtest\\origin.bin"
#define BACKUP_PATH "D:\\pro_nvtest\\backup.bin"
#else
#define ORIG_PATH "/dev/block/platform/sprd-sdhci.3/by-name/prodinfo1"
#define BACKUP_PATH "/dev/block/platform/sprd-sdhci.3/by-name/prodinfo2"
#endif
#define APNV_SIZE 0x20000
#define PRONV_MAGIC   0x41504E56   ///* 'APNV' */"
#define SCT_SIZE    512

typedef enum _NVITEM_ERROR {
    NVERR_NONE  = 0,   			/* Success */
    NVERR_SYSTEM,      			/* System error, e.g. hardware failure */
    NVERR_INVALID_PARAM,
    NVERR_NO_ENOUGH_RESOURCE,
    NVERR_OPERATE_ERROR,
    NVERR_NOT_EXIST,
    NVERR_TIMEOUT,
}NVITEM_ERROR_E;
//-------------------------------------------------
//				Const config: can not be changed
//-------------------------------------------------

//-------------------------------------------------
//				Config: can be changed if nessarry
//-------------------------------------------------


#endif

