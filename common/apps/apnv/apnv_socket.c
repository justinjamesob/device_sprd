
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <fcntl.h>
#include <sys/mount.h>
#include <sys/stat.h>
#include <sys/statfs.h>
#include <sys/uio.h>
#include <dirent.h>
#include <pthread.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <sys/statfs.h>
#include <sys/select.h>
#include <sys/time.h>
#include <time.h>
#include <cutils/properties.h>
#include "apnv_socket.h"

typedef enum NV_OPERATE_TYPE{
    PRO_NV_WRITE,
    PRO_NV_READ,
}NV_OPERATE_TYPE_E;

typedef struct
{
    uint32 item_id;
    uint16 size;
}PRO_NV_READ_PARAM;

typedef struct
{
    uint32 item_id;
    uint16 size;
}PRO_NV_WRITE_PARAM;

typedef struct
{
    uint32 item_startId;
    uint32 item_endId;
}PRO_NV_DEL_PARAM;

typedef struct _PRO_NV_{
    NV_OPERATE_TYPE_E type;
    NVITEM_ERROR_E ope_result;
    union
    {
        PRO_NV_READ_PARAM readParam;
        PRO_NV_WRITE_PARAM writeParam;
        PRO_NV_DEL_PARAM    delParam;
        // ...
    }param;
}PRO_NV;

typedef struct _RECV_RESULT_{
    NVITEM_ERROR_E error_type;
    int       recv_size;

}RECV_RESULT;

#define LOG_TAG "APNV_SOCKET"


#ifdef WIN32
#define APNV_SOCKET_PRINT  printf
#else
#define APNV_SOCKET_PRINT  ALOGD
#endif

#define PRONV_SOCKET_FILE "/data/local/tmp/pronv_sock"

static NVITEM_ERROR_E __recv_socket(int sockfd, void* buffer, int size)
{
	int received = 0, result;
	int retval;
	fd_set rfds;
	struct timeval timeout;
	RECV_RESULT rec_result;

    timeout.tv_sec = 5;
    timeout.tv_usec = 0;

    rec_result.recv_size = 0;
	while(buffer && (received < size)) {
        FD_ZERO(&rfds);
        FD_SET(sockfd, &rfds);
        retval = select(sockfd + 1, &rfds, NULL, NULL, &timeout);
        APNV_SOCKET_PRINT("APNV_SOCKET:11111 retval = %d",retval);
        if(retval == -1) {
            APNV_SOCKET_PRINT("APNV_SOCKET:__recv_socket error in select!\n");
            close(sockfd);
            rec_result.error_type = NVERR_SYSTEM;
            rec_result.recv_size = result;
            break;
        }
        else if(retval) {
            if (FD_ISSET(sockfd, &rfds)) {
                result = recv(sockfd, (char *)buffer + received, size - received, 0);
                APNV_SOCKET_PRINT("APNV_SOCKET:2222 result = %d",result);
                if (result > 0) {
                    received += result;
                    rec_result.error_type = NVERR_NONE;
                    rec_result.recv_size = received;
                } else {
                    rec_result.error_type = NVERR_SYSTEM;
                    rec_result.recv_size = result;
                    break;
                }
            }
            else{
                 APNV_SOCKET_PRINT("APNV_SOCKET:33333");
            }
        }
        else {
            APNV_SOCKET_PRINT("APNV_SOCKET:__recv_socket timeout\n");
            close(sockfd);
            rec_result.error_type = NVERR_TIMEOUT;
            rec_result.recv_size = -1;
            break;
        }
	}
	APNV_SOCKET_PRINT("APNV_SOCKET:__recv_socket size = %d, type = %d",rec_result.recv_size,rec_result.error_type);

    if(rec_result.recv_size < 0){
        APNV_SOCKET_PRINT("APNV_SOCKET: recv size error size = %d",rec_result.recv_size);
        rec_result.error_type = NVERR_SYSTEM;
    }
	return rec_result.error_type;
}

static int __send_socket(int sockfd, void* buffer, int size)
{
	int result = -1;
	int ioffset = 0;

	while(sockfd > 0 && ioffset < size) {
		result = send(sockfd, (char *)buffer + ioffset, size - ioffset, 0);
		if (result > 0) {
			ioffset += result;
		} else {
			break;
		}
	}
	return result;
}

static int32 _create_socket(void)
{
     int sockfd;
     struct sockaddr_un address;

    /* init unix domain socket */
     memset(&address, 0, sizeof(address));
     address.sun_family=AF_UNIX;
     strcpy(address.sun_path, PRONV_SOCKET_FILE);

     sockfd = socket(AF_UNIX, SOCK_STREAM, 0);
         if (sockfd < 0) {
         APNV_SOCKET_PRINT("APNV_SOCKET:create socket failed");
         return -1;
     }

    return sockfd;
}

static APNV_SOCKET_ERROR_E _connect_socket(int sockfd)
{
    struct timeval timeout;
    struct sockaddr_un address;
    int retval,flags;
    APNV_SOCKET_ERROR_E serr;
    fd_set Write, Err;

    /* init unix domain socket */
    memset(&address, 0, sizeof(address));
    address.sun_family=AF_UNIX;
    strcpy(address.sun_path, PRONV_SOCKET_FILE);

    timeout.tv_sec = 5;
    timeout.tv_usec = 0;

    //set the socket in non-blocking
    flags = fcntl(sockfd, F_GETFL, 0);
    if (fcntl(sockfd, F_SETFL, flags | O_NONBLOCK) < 0){
        APNV_SOCKET_PRINT("APNV_SOCKET set socket non-blocking error\n" );
        return SOCKET_CONNECT_ERROR;
    }

    if(connect(sockfd, (struct sockaddr*)&address, sizeof(address)) < 0){
        APNV_SOCKET_PRINT("APNV_SOCKET:connect failed");
        return SOCKET_CONNECT_ERROR;
    }
    FD_ZERO(&Write);
    FD_ZERO(&Err);
    FD_SET(sockfd, &Write);
    FD_SET(sockfd, &Err);
    retval = select(sockfd+1,NULL,&Write,&Err,&timeout);
    if(-1 == retval){
        APNV_SOCKET_PRINT("APNV_SOCKET connect retval -1\n");
        serr = SOCKET_CONNECT_ERROR;
    }
    else if (retval){
        APNV_SOCKET_PRINT("APNV_SOCKET connect success");
        serr = SOCKET_ERROR_NONE;
    }
    else{
        //timeout
        APNV_SOCKET_PRINT("APNV_SOCKET connect timeout\n");
        serr = SOCKET_TIMEOUT;;
    }

    // restart the socket mode
    flags = fcntl(sockfd, F_GETFL, 0);
    if (fcntl(sockfd, F_SETFL, flags & ~O_NONBLOCK) <0){
        APNV_SOCKET_PRINT("APNV_SOCKET reset socket mode  failed\n");
        return SOCKET_CONNECT_ERROR;
    }

    return serr;
}

NVITEM_ERROR_E write_productnv(uint32 id, void * buffer, uint16 size)
{
    PRO_NV nv_item;
    int sockfd;
    APNV_SOCKET_ERROR_E serr;
    NVITEM_ERROR_E nv_res;

    nv_item.type = PRO_NV_WRITE;
    nv_item.param.writeParam.item_id = id;
    nv_item.param.writeParam.size = size;

    sockfd = _create_socket();

    if(sockfd < 0){
        APNV_SOCKET_PRINT("APNV_SOCKET:write_productnv create socket failed!\n");
        return NVERR_SYSTEM;
    }

    serr = _connect_socket(sockfd);

    switch(serr)
    {
        case SOCKET_ERROR_NONE:
        if(__send_socket(sockfd, &nv_item, sizeof(nv_item)) <= 0){
            APNV_SOCKET_PRINT("APNV_SOCKET:write_productnv send head failed!");
            nv_res = NVERR_SYSTEM;
            break;
        }
        if(__send_socket(sockfd, buffer, size) <= 0){
            APNV_SOCKET_PRINT("APNV_SOCKET:write_productnv send body failed!");
            nv_res = NVERR_SYSTEM;
            break;
        }

        nv_res = __recv_socket(sockfd, &nv_item , sizeof(nv_item));
        if(NVERR_NONE != nv_res) {
            APNV_SOCKET_PRINT("APNV_SOCKET:write_productnv--__recv_socket nv_res = %d",nv_res);
            break;
        }
        if(NVERR_NONE != nv_item.ope_result){
            nv_res = nv_item.ope_result;
            APNV_SOCKET_PRINT("APNV_SOCKET:write_productnv  nv_item.ope_result = %d",nv_item.ope_result);
            break;
        }
        nv_res = NVERR_NONE;
        break;

        case SOCKET_TIMEOUT:
        APNV_SOCKET_PRINT("APNV_SOCKET:write_productnv connect timeout");
        nv_res = NVERR_TIMEOUT;
        break;

        case SOCKET_CONNECT_ERROR:
        APNV_SOCKET_PRINT("APNV_SOCKET:write_productnv socket error");
        nv_res = NVERR_SYSTEM;
        break;

        default:
        APNV_SOCKET_PRINT("APNV_SOCKET:write_productnv error unknow");
        nv_res = NVERR_SYSTEM;
        break;
    }
    close(sockfd);
    return nv_res;
}

NVITEM_ERROR_E read_productnv(uint32 id, void * buffer, uint16 size)
{
    PRO_NV nv_item;
    int sockfd;
    struct timeval timeout;
    APNV_SOCKET_ERROR_E serr;
    NVITEM_ERROR_E nv_res;

    timeout.tv_sec = 5; // 1second
    timeout.tv_usec = 0; // milli 	second
    nv_item.type = PRO_NV_READ;
    nv_item.param.readParam.item_id = id;
    nv_item.param.readParam.size = size;

    sockfd = _create_socket();

   if(sockfd < 0){
        APNV_SOCKET_PRINT("APNV_SOCKET:read_productnv connect failed!\n");
        return NVERR_SYSTEM;
    }

    serr = _connect_socket(sockfd);

    switch(serr)
    {
        case SOCKET_ERROR_NONE:
        if(0 >=__send_socket(sockfd, &nv_item, sizeof(nv_item))){
            APNV_SOCKET_PRINT("APNV_SOCKET:read_productnv send head failed!");
            nv_res =  NVERR_SYSTEM;
            break;
        }

        nv_res = __recv_socket(sockfd, &nv_item , sizeof(nv_item));
        if(nv_res != NVERR_NONE) {
            APNV_SOCKET_PRINT("APNV_SOCKET:read_productnv recv head failed nv_res = %d",nv_res);
            break;
        }

        if(NVERR_NONE != nv_item.ope_result){
            APNV_SOCKET_PRINT("APNV_SOCKET:read_productnv read failed!");
            nv_res =  nv_item.ope_result;
            break;
        }

        nv_res = __recv_socket(sockfd, buffer , size);

        if(nv_res != NVERR_NONE) {
            APNV_SOCKET_PRINT("APNV_SOCKET:read_productnv read buffer failed nv_res = %d",nv_res);
            break;
        }
        nv_res = NVERR_NONE;
        break;

        case SOCKET_TIMEOUT:
        APNV_SOCKET_PRINT("APNV_SOCKET:read_productnv connect socket timeout");
        nv_res = NVERR_TIMEOUT;
        break;

        case SOCKET_CONNECT_ERROR:
        APNV_SOCKET_PRINT("APNV_SOCKET:read_productnv connect socket error");
        nv_res = NVERR_SYSTEM;
        break;

        default:
        APNV_SOCKET_PRINT("APNV_SOCKET:read_productnv error unknow");
        nv_res = NVERR_SYSTEM;
        break;
    }
    close(sockfd);
    return nv_res;
}
