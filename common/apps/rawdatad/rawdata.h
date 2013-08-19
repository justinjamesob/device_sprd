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

#endif
