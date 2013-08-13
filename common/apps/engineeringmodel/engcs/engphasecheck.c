#include <fcntl.h>
#include "engopt.h"
#include <pthread.h>
#include "engphasecheck.h"


#define PHASE_CHECKE_FILE "/productinfo/productinfo.bin"
#define PHASE_CHECKE_FILE0 "/dev/block/mmcblk0p13" //add by kenyliu on 2013 08 13 for read sn bug 201276

int eng_phasechecktest(void)
{
	SP09_PHASE_CHECK_T phase;
	
	return eng_getphasecheck(&phase);
}

int eng_getphasecheck(SP09_PHASE_CHECK_T* phase_check)
{
	int ret = 0;
	int len;
	//modify  by kenyliu on 2013 08 13 for read sn bug 201276

       SP09_PHASE_CHECK_T phase_check_temp;
	int fd = open(PHASE_CHECKE_FILE0,O_RDONLY);
	if(fd >= 0)
	{
		ENG_LOG("%s open Ok PHASE_CHECKE_FILE0 = %s ",__FUNCTION__ , PHASE_CHECKE_FILE0);
		len = read(fd,&phase_check_temp,sizeof(SP09_PHASE_CHECK_T));

		ENG_LOG("Magic0=%d",phase_check_temp.Magic);

		ENG_LOG("SN01=%s",phase_check_temp.SN1);
		ENG_LOG("SN02=%s",phase_check_temp.SN2);

		close(fd);
	}else{
		ENG_LOG("%s open fail PHASE_CHECKE_FILE0 = %s ",__FUNCTION__ , PHASE_CHECKE_FILE0);
	}

	fd = open(PHASE_CHECKE_FILE,O_RDONLY);
	if (fd >= 0){
		ENG_LOG("%s open Ok PHASE_CHECKE_FILE = %s ",__FUNCTION__ , PHASE_CHECKE_FILE);
		len = read(fd,phase_check,sizeof(SP09_PHASE_CHECK_T));

		ENG_LOG("Magic=%d",phase_check->Magic);
	
		ENG_LOG("SN1=%s",phase_check->SN1);
		ENG_LOG("SN2=%s",phase_check->SN2);

		if((phase_check_temp.Magic == SP09_SPPH_MAGIC_NUMBER) || (phase_check_temp.Magic == SP05_SPPH_MAGIC_NUMBER)){
			strcpy(phase_check->SN1, phase_check_temp.SN1);
			strcpy(phase_check->SN2, phase_check_temp.SN2);
		}

		ENG_LOG("SN1=%s",phase_check->SN1);
		ENG_LOG("SN2=%s",phase_check->SN2);

		if (len <= 0){
			ret = 1;
		}
		close(fd);
	} else {
		ENG_LOG("%s open fail PHASE_CHECKE_FILE = %s ",__FUNCTION__ , PHASE_CHECKE_FILE);
		ret = 1;
	}
//end modify kenyliu
	return ret;
}


