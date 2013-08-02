#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>
#include <assert.h>
#include "apnv_api.h"
#include "apnv_common.h"
FILE * _s_ori_fd = NULL;
FILE * _s_bac_fd = NULL;
uint8 * _apnv_buf = NULL;

#define ROUND_UP(d)			(((d) + sizeof(uint32) - 1) & ~(sizeof(uint32) - 1))
#define INVALID_ID          0xffff

//------------------- Checksum interface-------------------
static unsigned short calc_checksum(unsigned char *dat, unsigned long len)
{
	unsigned long checksum = 0;
	unsigned short *pstart, *pend;
	if (0 == (unsigned long)dat % 2)  {
		pstart = (unsigned short *)dat;
		pend = pstart + len / 2;
		while (pstart < pend) {
			checksum += *pstart;
			pstart ++;
		}
		if (len % 2)
			checksum += *(unsigned char *)pstart;
		} else {
		pstart = (unsigned char *)dat;
		while (len > 1) {
			checksum += ((*pstart) | ((*(pstart + 1)) << 8));
			len -= 2;
			pstart += 2;
		}
		if (len)
			checksum += *pstart;
	}
	checksum = (checksum >> 16) + (checksum & 0xffff);
	checksum += (checksum >> 16);
	return (~checksum);
}



/*
	TRUE(1): pass
	FALSE(0): fail
*/
static BOOLEAN _chkEcc(uint8* buf, uint32 size)
{
	uint16 crc,crcOri;

	crc = calc_checksum(buf,size-4);
	crcOri = (uint16)((((uint16)buf[size-3])<<8) | ((uint16)buf[size-4]) );

	return (crc == crcOri);
}


static void _makEcc(uint8* buf, uint32 size)
{
	uint16 crc;

    crc = calc_checksum(buf,size-4);
	buf[size-4] = (uint8)(0xFF&crc);
	buf[size-3] = (uint8)(0xFF&(crc>>8));
	buf[size-2] = 0;
	buf[size-1] = 0;
    APNV_PRINT("APNV:_makEcc crc 0x%x",crc);
	return;
}

//------------------- file interface-------------------
static char *firstName = 0, *secondName = 0;

static void _readFile(void)
{
	int fileHandle = 0;
	int ret = 0;

    if(rand()%2){
        firstName = ORIG_PATH;
        secondName = BACKUP_PATH;
    }
    else {
        firstName = BACKUP_PATH;
        secondName = ORIG_PATH;
    }
    do
    {
        fileHandle = open(firstName, O_RDWR, S_IRWXU | S_IRWXG | S_IRWXO);

        if(fileHandle > 0) {
            ret = read(fileHandle, _apnv_buf, APNV_SIZE);
            close(fileHandle);
        }
        else{
            APNV_PRINT("APNV:_readFile fileHandle1 = %d",fileHandle);
        }
        if(ret == APNV_SIZE){
            if(_chkEcc(_apnv_buf, APNV_SIZE)) {
                return;
            }
        }
        fileHandle = open(secondName, O_RDWR, S_IRWXU | S_IRWXG | S_IRWXO);
        if(fileHandle > 0) {
            ret = read(fileHandle, _apnv_buf, APNV_SIZE);
            close(fileHandle);
        }
        else{
            APNV_PRINT("APNV:_readFile fileHandle2 = %d",fileHandle);
        }
        if(ret == APNV_SIZE){
            if(!_chkEcc(_apnv_buf, APNV_SIZE)){
                return;
            }
        }
        fileHandle  = open(firstName, O_RDWR | O_CREAT | O_TRUNC, S_IRWXU | S_IRWXG | S_IRWXO);
        if(fileHandle < 0)return;
        write(fileHandle, _apnv_buf, APNV_SIZE);
        fsync(fileHandle);
        close(fileHandle);
    }while(0);

}
static void _writeFile(void)
{
    int fileHandle = 0;

    APNV_PRINT("APNV:sync enter");
	_makEcc(_apnv_buf,APNV_SIZE);
    fileHandle  = open(secondName, O_RDWR | O_CREAT | O_TRUNC , S_IRWXU | S_IRWXG | S_IRWXO);
    if(fileHandle > 0){
        if(APNV_SIZE != write(fileHandle, _apnv_buf, APNV_SIZE)) {
            APNV_PRINT("APNV:sync second handle error");
            return;
        }
    }
    else{
        APNV_PRINT("APNV:sync fileHandle2 %d secondName %s",fileHandle,secondName);
    }
    fsync(fileHandle);
    close(fileHandle);

    fileHandle  = open(firstName, O_RDWR | O_CREAT | O_TRUNC, S_IRWXU | S_IRWXG | S_IRWXO);
    if(fileHandle > 0){
        if(APNV_SIZE != write(fileHandle, _apnv_buf, APNV_SIZE)) {
            APNV_PRINT("APNV:sync first handle error");
            return;
        }
    }
    else{
        APNV_PRINT("APNV:sync fileHandle1 %d firstName %s",fileHandle,firstName);
    }
    fsync(fileHandle);
    close(fileHandle);
	APNV_PRINT("APNV:sync finish");
}

static BOOLEAN _checkfile(uint16 * buf,uint32 size)
{
    uint32 offset = 4;
    uint16 tmp;

    APNV_PRINT("APNV:_checkfile enter");
    while(1){
        if(offset + sizeof(tmp) > APNV_SIZE) {
            APNV_PRINT("APNV:_checkfile failed");
            return 0;
        }
        offset += sizeof(tmp);
        APNV_PRINT("APNV:_checkfile buf[offset] 0x%x",buf[offset]);
        if(buf[offset] == INVALID_ID){
            APNV_PRINT("APNV:_checkfile %x",offset);
            break;
        }
    }
    return 1;
}

void productnv_init(void)
{
    _apnv_buf = malloc(APNV_SIZE);
	if(NULL == _apnv_buf){
	    APNV_PRINT("APNV:malloc buf failed");
	    return;
	}
	memset(_apnv_buf,0xff,APNV_SIZE);
	APNV_PRINT("APNV:_apnv_buf = 0x%x",_apnv_buf);	
	 _readFile();
    if(!_checkfile((uint16*)_apnv_buf,APNV_SIZE)){
        APNV_PRINT("APNV:productnv_init failed");
        return;
    }
}

static NVITEM_ERROR_E _writeItem(uint32 nv_id,uint8 * buf,uint16 size)
{
    uint16 tmp[2];
    uint32 offset = 4;
    BOOLEAN is_create = FALSE;
    uint32 count = 0;

    if(NULL == _apnv_buf){
        APNV_PRINT("APNV:_writeItem error! NULL == _apnv_buf");
        return NVERR_SYSTEM;
    }

    size = ROUND_UP(size);//4byte align
    while(1){
        if(offset + sizeof(tmp) > APNV_SIZE){
            APNV_PRINT("APNV:_writeItem out of boundary");
            return NVERR_SYSTEM;
        }
        memcpy(tmp,_apnv_buf+offset,sizeof(tmp));
        if(nv_id == tmp[0]){
            APNV_PRINT("APNV:_writeItem find a id %d",tmp[0]);
            break;
        }
        if(tmp[0] == INVALID_ID){
            APNV_PRINT("APNV:_writeItem count = %d",count);
            is_create = TRUE;
            break;
        }
        offset += sizeof(tmp);
        offset += tmp[1];
        count++;
    }
    if(is_create){
        APNV_PRINT("APNV:_writeItem create item");
        tmp[0] = nv_id;
        tmp[1] = size;
        APNV_PRINT("APNV:_writeItem id 0x%x size 0x%x",tmp[0],tmp[1]);
        //tmp[0] = htod16(tmp[0]);//reserved for big endian
        //tmp[1] = htod16(tmp[1]);
        APNV_PRINT("APNV:_writeItem 2 id 0x%x size 0x%x",tmp[0],tmp[1]);
        memcpy(_apnv_buf+offset,tmp,sizeof(tmp));
    }
    else{
        if(size != tmp[1]){
            APNV_PRINT("APNV:_writeItem size %d != tmp[1] %d",size,tmp[1]);
            return NVERR_INVALID_PARAM;
        }
    }
    memcpy(_apnv_buf+offset+sizeof(tmp),buf,size);
    return NVERR_NONE;
}

static NVITEM_ERROR_E _readItem(uint32 nv_id,uint8 * buf,uint16 size)
{
    uint16 tmp[2];
    uint32 offset = 4;

    if(NULL == _apnv_buf){
        APNV_PRINT("APNV:_readItem error! NULL == _apnv_buf");
        return NVERR_SYSTEM;
    }
    while(1){
        if(offset + sizeof(tmp) > APNV_SIZE){
        APNV_PRINT("APNV:_readItem out of boundary");
        return NVERR_SYSTEM;
        }
        memcpy(tmp,_apnv_buf+offset,sizeof(tmp));
        //tmp[0] = htod16(tmp[0]);//reserved for big endian
        //tmp[1] = htod16(tmp[1]);
        offset += sizeof(tmp);
        if(nv_id == tmp[0]){
            APNV_PRINT("APNV:_readItem find a id %d",tmp[0]);
            break;
        }
        if(tmp[0] == INVALID_ID){
            return NVERR_NOT_EXIST;
        }
        offset += tmp[1];
    }
    if(size > tmp[1]){
        APNV_PRINT("APNV:_readItem size %d != tmp[1] %d",size,tmp[1]);
        return NVERR_INVALID_PARAM;
    }
    memcpy(buf,_apnv_buf+offset,size);
    return NVERR_NONE;
}

NVITEM_ERROR_E PRONV_WriteItem(uint32 nv_id,uint8 * buf,uint16 size)
{
    NVITEM_ERROR_E err = 0;

    APNV_PRINT("APNV: nv_id = %d",nv_id);

    err = _writeItem(nv_id,buf,size);
    APNV_PRINT("APNV:_writeItem err = %d",err);

    if(NVERR_NONE == err) {
        _writeFile();
    }
    return err;
}

NVITEM_ERROR_E PRONV_ReadItem(uint32 nv_id,uint8 * buf,uint16 size)
{
    return _readItem(nv_id,buf,size);
}

void productnv_unInit(void)
{
    if(NULL != _apnv_buf)
    {
        free(_apnv_buf);
        _apnv_buf = NULL;
    }
}

