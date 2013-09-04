#ifndef __RAWDATAD_H__
#define __RAWDATAD_H__

#include <utils/Log.h>

#ifdef LOG_TAG
#undef LOG_TAG
#define LOG_TAG         		"RAWDATAD"
#endif

#define DBG						ALOGD
#define RAWDATA_PARTITION_NUM 	2
#define RAWDATA_MONITOR_ITEM	10
#define RAWDATA_DUMP_SIZE	10

#define EVENT_NUM 		16
#define MAX_BUF_SIZE 		1024

#define RAWDATA_BUFFER_SIZE 	(1024*4)
#define RAWDATA_DATASIZE 	(1024*3+4)
#define RAWDATA_COUNTER_SIZE	4
#define RAWDATA_CRC_OFFSET	RAWDATA_DATASIZE+RAWDATA_COUNTER_SIZE
#define RAWDATA_COUNTER_THRES	0xEFFFFFFF
#define RAWDATA_PROPERTY	"sys.rawdata.ready"
#define RAWDATA_READY		"1"
#define RAM_SP09_SPPH_MAGIC_NUMBER             (0X53503039)    // "SP09"

#define NUM_ELEMS(x) (sizeof(x)/sizeof(x[0]))

typedef enum {
	RAWDATA_PARTITION0=0,
	RAWDATA_PARTITION1,
	RAWDATA_PARTITION_END
}RAWDATA_PARTITION_NUMBER;

typedef enum {
	RAWDATA_BLOCK_PHASECHECK=0,
	RAWDATA_BLOCK_FACTORYMODE,
	RAWDATA_BLOCK_ALARM,
	RAWDATA_BLOCK_END
}RAWDATA_BLOCK_NUMBER;

struct wd_name {
	int wd;
	const char * name;
};

typedef struct
{
    unsigned int     	adc[2];           // calibration of ADC, two test point
    unsigned int 	battery[2];       // calibraton of battery(include resistance), two test point
    unsigned int    	reserved[8];      // reserved for feature use.
} RAW_ADC_T;

typedef struct
{
	unsigned int	magic;		  // when create ,magic = "CALI"
	unsigned int	cali_flag;        // cali_flag   default 0xFFFFFFFF, when calibration finished,it is set "COMP"
	RAW_ADC_T 	adc_para;         // ADC calibration data.
}RAW_INFO_DATA_T;

typedef struct RAM_tagSP09_PHASE_CHECK
{
	unsigned long Magic;                	// "SP09"   (¿œ\u0153”ø⁄Œ™SP05)
	char    	SN1[24]; 	// SN , SN_LEN=24
	char    	SN2[24];    // add for Mobile
	int     	StationNum;                 	// the test station number of the testing
	char    	StationName[15][10];
	unsigned char 	Reserved[13];
	unsigned char 	SignFlag;
	char    	szLastFailDescription[32];
	unsigned short  iTestSign;				// Bit0~Bit14 ---> station0~station 14
										//if tested. 0: tested, 1: not tested
	unsigned short  iItem;    // part1: Bit0~ Bit_14 indicate test Station,1±Ì \u0178Pass

}RAM_SP09_PHASE_CHECK_T, *RAM_LPSP09_PHASE_CHECK_T;

#endif
