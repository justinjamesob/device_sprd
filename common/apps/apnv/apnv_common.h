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


#ifdef WIN32
#define ORIG_PATH "D:\\juan.wu\\pro_nvtest\\origin.bin"
#define BACKUP_PATH "D:\\juan.wu\\pro_nvtest\\backup.bin"
#else
#define ORIG_PATH "/dev/block/platform/sprd-sdhci.3/by-name/prodinfo1"
#define BACKUP_PATH "/dev/block/platform/sprd-sdhci.3/by-name/prodinfo2"
#endif
#define APNV_SIZE 0x20000
#define PRONV_MAGIC   0x41504E56   ///* 'APNV' */"
#define SCT_SIZE    512
/* Status of NV directory entry */
#define STATUS_VALID		0x0001 /* 01b */
#define STATUS_DELETED      0x0002 /* 10b */
#define STATUS_MASK         0x0003 /* 11b */

typedef enum _NVITEM_ERROR {
    NVERR_NONE  = 0,   			/* Success */
    NVERR_SYSTEM,      			/* System error, e.g. hardware failure */
    NVERR_INVALID_PARAM,
    NVERR_NO_ENOUGH_RESOURCE,
    NVERR_OPERATE_ERROR,
    NVERR_NOT_EXIST,
    NVERR_TIMEOUT,
}NVITEM_ERROR_E;

#ifdef WIN32
#pragma pack(1)
#endif
//__packed
typedef __packed struct _NPB {
    uint32    	magic;
	uint16		min_id;
	uint16		max_id;
	uint16    	tot_scts;			/* Total sectors of NV partition */
	uint16    	sct_size;			/* Size in bytes of a sector */
	uint16    	dir_entry_count;	/* count of NV dir entry */
	uint16    	dir_entry_size;		/* Size in byte of NV dir entry, = sizeof(NVDIR_ENTRY)*/
	uint32      next_offset;	    /* Next offset to allocate for item data */
} NPB;

/* NV Directroy Entry */
typedef __packed struct _NVDIR_ENTRY {
	uint16 size;			/* Size in byte of data field of an item */
	uint16 status;			/* Status of the item */
	uint32 offset;			/* Offset of the item, measured from the beginning
							 * of NV partition */
} NVDIR_ENTRY;
#ifdef WIN32
#pragma pack()
#endif

/* NV Device control block */
typedef struct _NV_DEVICE {
	NPB				*npb;
	/* size of partition unit is bytes */
	uint32			tot_size;
	/* Dir table parameters */
	uint16			first_dir_sct;
	/* Data part parameters */
	uint32			data_offset;
} NV_DEVICE;

//-------------------------------------------------
//				Const config: can not be changed
//-------------------------------------------------

//-------------------------------------------------
//				Config: can be changed if nessarry
//-------------------------------------------------


#endif

