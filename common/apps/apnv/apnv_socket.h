#include <cutils/log.h>

#ifndef     _PRONV_SOCKET_H
#define _PRONV_SOCKET_H

typedef unsigned char		BOOLEAN;
typedef unsigned char 		uint8;
typedef unsigned short		uint16;
typedef unsigned  int		uint32;

typedef signed char		int8;
typedef signed short		int16;
typedef signed int		int32;

typedef enum _APNV_SOCKET_ERROR {
    SOCKET_ERROR_NONE = 0,
    SOCKET_CONNECT_ERROR,
    SOCKET_TIMEOUT,



}APNV_SOCKET_ERROR_E;


typedef enum _NVITEM_ERROR {
    NVERR_NONE  = 0,   			/* Success */
    NVERR_SYSTEM,      			/* System error, e.g. hardware failure */
    NVERR_INVALID_PARAM,
    NVERR_NO_ENOUGH_RESOURCE,
    NVERR_OPERATE_ERROR,
    NVERR_NOT_EXIST,
    NVERR_TIMEOUT,
}NVITEM_ERROR_E;

NVITEM_ERROR_E write_productnv(uint32 id, void * buffer, uint16 size);
NVITEM_ERROR_E read_productnv(uint32 id, void * buffer, uint16 size);
#endif

