#include <cutils/log.h>

#ifndef     _PRONV_SOCKET_H
#define _PRONV_SOCKET_H

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

NVITEM_ERROR_E write_productnv(unsigned int id, void * buffer, unsigned short size);
NVITEM_ERROR_E read_productnv(unsigned  int id, void * buffer, unsigned short size);
#endif

