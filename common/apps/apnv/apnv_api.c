#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>
#include <assert.h>
#include "apnv_api.h"
#include "apnv_common.h"
FILE * _s_ori_fd = NULL;
FILE * _s_bac_fd = NULL;
NV_DEVICE pro_nvd;
uint8 * _apnv_buf = NULL;

#define ROUND_UP(d)			(((d) + sizeof(uint32) - 1) & ~(sizeof(uint32) - 1))


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

        if(fileHandle >= 0) {
            ret = read(fileHandle, _apnv_buf, APNV_SIZE);
            close(fileHandle);
        }
        if(ret == APNV_SIZE){
            if(_chkEcc(_apnv_buf, APNV_SIZE)) {
                return;
            }
        }
        fileHandle = open(secondName, O_RDWR, S_IRWXU | S_IRWXG | S_IRWXO);
        if(fileHandle >= 0) {
            ret = read(fileHandle, _apnv_buf, APNV_SIZE);
            close(fileHandle);
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

    fileHandle  = open(secondName, O_RDWR | O_CREAT | O_TRUNC, S_IRWXU | S_IRWXG | S_IRWXO);
    if(fileHandle >= 0){
        if(APNV_SIZE != write(fileHandle, _apnv_buf, APNV_SIZE)) {
            APNV_PRINT("APNV:sync second handle error");
            return;
        }
    }
    fsync(fileHandle);
    close(fileHandle);

    fileHandle  = open(firstName, O_RDWR | O_CREAT | O_TRUNC, S_IRWXU | S_IRWXG | S_IRWXO);
    if(fileHandle >= 0){
        if(APNV_SIZE != write(fileHandle, _apnv_buf, APNV_SIZE)) {
            APNV_PRINT("APNV:sync first handle error");
            close(fileHandle);
            return;
        }
    }
    else{
        APNV_PRINT("APNV:open handle error");
        return;
    }
    fsync(fileHandle);
    close(fileHandle);
	APNV_PRINT("APNV:sync finish");
}

static BOOLEAN __format(void)
{
    uint32  dir_scts;
    uint32  checksum;

    memset(_apnv_buf,0,APNV_SIZE);
    pro_nvd.npb->magic = PRONV_MAGIC;
    pro_nvd.npb->min_id = 0;
    pro_nvd.npb->max_id = 1000;
    pro_nvd.npb->tot_scts = APNV_SIZE/SCT_SIZE;
    pro_nvd.npb->sct_size = SCT_SIZE;
    pro_nvd.npb->dir_entry_count = pro_nvd.npb->max_id - pro_nvd.npb->min_id;
    pro_nvd.npb->dir_entry_size = sizeof(NVDIR_ENTRY);
    dir_scts = (pro_nvd.npb->dir_entry_count * pro_nvd.npb->dir_entry_size + pro_nvd.npb->sct_size - 1)
			/ pro_nvd.npb->sct_size; /* Round up to multiples of sector */
	pro_nvd.first_dir_sct = 1;
    pro_nvd.npb->next_offset = (pro_nvd.first_dir_sct + dir_scts) * pro_nvd.npb->sct_size;
    APNV_PRINT("APNV:npb->magic = 0x%x\n",pro_nvd.npb->magic);
    APNV_PRINT("APNV:npb->min_id = 0x%x\n",pro_nvd.npb->min_id);
    APNV_PRINT("APNV:npb->max_id = 0x%x\n",pro_nvd.npb->max_id);
    APNV_PRINT("APNV:npb->tot_scts = 0x%x\n",pro_nvd.npb->tot_scts);
    APNV_PRINT("APNV:npb->sct_size = 0x%x\n",pro_nvd.npb->sct_size);
    APNV_PRINT("APNV:npb->dir_entry_count = 0x%x\n",pro_nvd.npb->dir_entry_count);
    APNV_PRINT("APNV:npb->dir_entry_size = 0x%x\n",pro_nvd.npb->dir_entry_size);

    pro_nvd.tot_size = pro_nvd.npb->tot_scts * pro_nvd.npb->sct_size;
	/* Data part follows dir table */
	pro_nvd.data_offset = (pro_nvd.first_dir_sct + dir_scts) * pro_nvd.npb->sct_size;
	if (pro_nvd.data_offset >= pro_nvd.tot_size) {
        return FALSE;
	}
	if ((pro_nvd.npb->next_offset > pro_nvd.tot_size)
			|| (pro_nvd.npb->next_offset < pro_nvd.data_offset)) {
		return FALSE;
	}
    _writeFile();
    return TRUE;
}
static void __setup_nvparam(void)
{
    uint32 dir_scts = 0;

    pro_nvd.first_dir_sct = 1;
    pro_nvd.tot_size = pro_nvd.npb->tot_scts * pro_nvd.npb->sct_size;
	/* Data part follows dir table */
    dir_scts = (pro_nvd.npb->dir_entry_count * pro_nvd.npb->dir_entry_size + pro_nvd.npb->sct_size - 1)
			/ pro_nvd.npb->sct_size; /* Round up to multiples of sector */
	pro_nvd.data_offset = (pro_nvd.first_dir_sct + dir_scts) * pro_nvd.npb->sct_size;
	if (pro_nvd.data_offset >= pro_nvd.tot_size){
        return;
	}
	if ((pro_nvd.npb->next_offset > pro_nvd.tot_size)
			|| (pro_nvd.npb->next_offset < pro_nvd.data_offset)){
		return;
	}
	return;
}
void productnv_init(void)
{
    _apnv_buf = malloc(APNV_SIZE);
	if(NULL == _apnv_buf){
	    APNV_PRINT("APNV:malloc buf failed");
	    return;
	}
	APNV_PRINT("APNV:_apnv_buf = 0x%x",_apnv_buf);
	memset(_apnv_buf,0,APNV_SIZE);
    pro_nvd.npb = (NPB*)_apnv_buf;

    _readFile();

    __format();
	if(PRONV_MAGIC != pro_nvd.npb->magic) {
        APNV_PRINT("APNV: magic error!");
         __format();
    }
    else {
        APNV_PRINT("APNV: __setup_nvparam!");
        __setup_nvparam();
    }
}

static BOOLEAN ___check_dir_entry(NV_DEVICE *dev, uint32 item_id, const NVDIR_ENTRY *entry)
{

    APNV_PRINT("APNV:___check_dir_entry status %d, offset %d size %d",entry->status,entry->offset,entry->size);
    APNV_PRINT("APNV:dev_offset %d,dev_total %d",dev->data_offset,dev->tot_size);
    if (0 == (entry->status & STATUS_MASK)) {
        if ((0 == entry->offset) && (0 == entry->size))
            return TRUE;
    }
    else {
        if ((entry->offset >= dev->data_offset) && (entry->offset < dev->tot_size)
                && (entry->size <=  dev->tot_size)
                && (entry->offset + entry->size <= dev->tot_size))
            return TRUE;
    }
    return FALSE;
}

static BOOLEAN __read_dir(NV_DEVICE *dev, uint32 item_id, NVDIR_ENTRY *entry)
{
    uint32 addr;

    addr = (item_id - dev->npb->min_id)* sizeof(NVDIR_ENTRY);

    APNV_PRINT("APNV:__read_dir item_id = %d  addr = 0x%x",item_id,addr);

	memcpy(entry, &_apnv_buf[addr+SCT_SIZE] ,sizeof(NVDIR_ENTRY));
    APNV_PRINT("APNV:__read_dir offset 0x%x,size %d,status %d",entry->offset,entry->size,entry->status);
    if (___check_dir_entry(dev, item_id, entry)) {
        return TRUE;
    }
    return FALSE;
}

static BOOLEAN __write_dir(NV_DEVICE *dev, uint32 item_id, const NVDIR_ENTRY *entry)
{
	uint32  addr;

	if(!___check_dir_entry(dev, item_id, entry)){
	    return FALSE;
	}
	addr = (item_id - dev->npb->min_id) * sizeof(NVDIR_ENTRY);
	memcpy(&_apnv_buf[addr+SCT_SIZE],(void *)entry,sizeof(NVDIR_ENTRY));
	APNV_PRINT("APNV:__write_dir dir_offet = 0x%x",entry->offset);
	return TRUE;
}

static uint32 __ndev_alloc(NV_DEVICE *dev, uint16 size)
{
	uint32		addr, next_offset;
	NPB			*npb;

	npb = dev->npb;

	size = ROUND_UP(size);
	if (npb->next_offset + size > dev->tot_size) {
		return 0;
	}
	next_offset = npb->next_offset;
	addr = npb->next_offset;
	npb->next_offset = addr + size;
	memcpy(_apnv_buf,dev->npb,SCT_SIZE);//update header

	return addr;
}

static BOOLEAN __read_data(NV_DEVICE *dev, uint32 offset, uint16 size, void *buf)
{
	if(NULL == buf || NULL == dev ||  offset + size > dev->tot_size || offset < dev->data_offset){
	    return FALSE;
	}

	memcpy(buf,&_apnv_buf[offset],size);
	return TRUE;
}

static BOOLEAN __write_data(NV_DEVICE *dev, uint32 offset, uint16 size, const void *buf)
{
	if(NULL == buf || NULL == dev ||  offset + size > dev->tot_size || offset < dev->data_offset){
	    return FALSE;
	}

	memcpy(&_apnv_buf[offset],buf,size);
	return TRUE;
}

static void __ndev_free(NV_DEVICE *dev, uint32 addr, uint16 size)
{
	NPB			*npb;
	uint32		offset;

    if(NULL == dev || NULL == dev->npb) {
        return;
    }
	npb = dev->npb;
	size = ROUND_UP(size);
	assert(addr + size == npb->next_offset);
	if (addr + size != npb->next_offset) {
		return;
	}

	offset = npb->next_offset;
	npb->next_offset = addr;

	memcpy(_apnv_buf,dev->npb,SCT_SIZE);//update header
}

static NVITEM_ERROR_E _create_item(uint32 item_id, uint16 size, const void *buf)
{
	NVDIR_ENTRY 	dir;
	uint32			addr;

	APNV_PRINT("APNV: _create_item enter");
	addr = __ndev_alloc(&pro_nvd, size);
	if (0 == addr) {
		APNV_PRINT("APNV: _create_item 0 == addr!");
		return NVERR_NO_ENOUGH_RESOURCE;
	}

	memset(&dir, 0, sizeof(dir));
	dir.size = size;
	dir.status = STATUS_VALID;
	dir.offset = addr;

	if (!__write_dir(&pro_nvd, item_id, &dir)){
		__ndev_free(&pro_nvd, addr, size);
		APNV_PRINT("APNV: _create_item __write_dir error!");
		return NVERR_SYSTEM;
	}
	if(!__write_data(&pro_nvd, dir.offset, size, buf)){
	    __ndev_free(&pro_nvd, addr, size);
	    APNV_PRINT("APNV: _create_item __write_data error!");
	    return NVERR_SYSTEM;
	}
	return NVERR_NONE;
}

static NVITEM_ERROR_E _writeItem(uint32 nv_id,uint8 * buf,uint16 size)
{
    NVDIR_ENTRY entry;
    NV_DEVICE * dev = &pro_nvd;

	if((nv_id < dev->npb->min_id) || (nv_id > dev->npb->max_id) || NULL == buf)	{
	    APNV_PRINT("APNV: _writeItem invalid param nv_id %d min_id %d max_id %d buf 0x%x",nv_id,dev->npb->min_id,dev->npb->max_id,buf);
		return NVERR_INVALID_PARAM;
	}
    if(!__read_dir(dev, nv_id, &entry)) {
        APNV_PRINT("APNV: _writeItem read dir error!");
        return NVERR_SYSTEM;
    }
    else {
        APNV_PRINT("APNV: _writeItem read dir success!");
    }

    //new id
	if (0 == (entry.status & STATUS_MASK)) {
		/* The item has never been written */
		if (NVERR_NONE != _create_item(nv_id, size, buf)){
		    APNV_PRINT("APNV: _writeItem create_item error!");
			return NVERR_SYSTEM;
		}
		APNV_PRINT("APNV: _writeItem create_item success!");
		return NVERR_NONE;
	}
	if (entry.status & STATUS_DELETED)
		entry.status = (entry.status & ~STATUS_MASK) | STATUS_VALID;

	//id exsit,but size is error
    if(entry.size != size) {
        APNV_PRINT("APNV: _writeItem entry size error!entry.size %d size %d",entry.size,size);
        return NVERR_SYSTEM;
    }
	/* Write NV item data */
	if (!__write_data(&pro_nvd, entry.offset, size, buf)) {
	    APNV_PRINT("APNV: _writeItem __write_data error!");
		return NVERR_SYSTEM;
	}
	return NVERR_NONE;
}

static NVITEM_ERROR_E _readItem(uint32 nv_id,uint8 * buf,uint16 size)
{
    NVDIR_ENTRY entry;
    uint32 checksum = 0;
    uint32 checksum_buf = 0;

    APNV_PRINT("APNV:_readItem enter");
	if((nv_id < pro_nvd.npb->min_id) || (nv_id > pro_nvd.npb->max_id) || NULL == buf){
	    APNV_PRINT("APNV: _readItem invalid param");
		return NVERR_INVALID_PARAM;
	}
    if(!__read_dir(&pro_nvd, nv_id, &entry)) {
        APNV_PRINT("APNV:_readItem read dir error");
        return NVERR_SYSTEM;
    }

	if (!(STATUS_VALID & entry.status))	{
	    APNV_PRINT("APNV:_readItem entry status %d error",entry.status);
		return NVERR_NOT_EXIST;
	}
	if(!__read_data(&pro_nvd, entry.offset, size, buf))	{
	    APNV_PRINT("APNV:_readItem read data error");
	    return NVERR_SYSTEM;
	}
	APNV_PRINT("APNV:_readItem success");
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

NVITEM_ERROR_E PRONV_DelItem(uint32 From,uint32 To)
{
    NVDIR_ENTRY entry;
    NV_DEVICE *dev = &pro_nvd;
    uint32 item_id = 0;

	if((From > To) || (From < pro_nvd.npb->min_id) || (From > pro_nvd.npb->max_id)||
	   (To < pro_nvd.npb->min_id) || (To > pro_nvd.npb->max_id)){
		return NVERR_INVALID_PARAM;
	}

    for(item_id = From;From<=To;From++) {
        if (!__read_dir(&pro_nvd, item_id, &entry)) {
            APNV_PRINT("APNV:PRONV_DelItem error __read_dir failed");
            return NVERR_INVALID_PARAM;
        }

        if (entry.status & STATUS_VALID) {
            entry.status = (entry.status & ~STATUS_MASK) | STATUS_DELETED;
            if(!__write_dir(&pro_nvd, item_id, &entry)){
                //delete error!
                APNV_PRINT("APNV:PRONV_DelItem error write dir failed");
                return NVERR_OPERATE_ERROR;
            }
        }
	}
	_writeFile();
    return NVERR_NONE;
}

void productnv_unInit(void)
{
    if(NULL != _apnv_buf)
    {
        free(_apnv_buf);
        _apnv_buf = NULL;
    }
}

