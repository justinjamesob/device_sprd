#include "apnv_common.h"

void productnv_init(void);

NVITEM_ERROR_E PRONV_WriteItem(uint32 nv_id,uint8 * buf,uint16 size);

NVITEM_ERROR_E PRONV_ReadItem(uint32 nv_id,uint8 * buf,uint16 size);

NVITEM_ERROR_E PRONV_DelItem(uint32 From,uint32 To);

void productnv_unInit(void);

