#include "apnv_common.h"
#include "apnv_api.h"
#include <sys/socket.h>
#include <sys/un.h>
#include <sys/types.h>

#define MAX_NAME_LEN 128
#define PRONV_SOCKET_FILE "/data/local/tmp/pronv_sock"

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

static NVITEM_ERROR_E _recv_socket(int sockfd, void* buffer, int size)
{
	int received = 0, result;
	int retval;
	fd_set rfds;
	struct timeval timeout;
	RECV_RESULT rec_result;

    timeout.tv_sec = 5;
    timeout.tv_usec = 0;

	while(buffer && (received < size)) {
        FD_ZERO(&rfds);
        FD_SET(sockfd, &rfds);

        retval = select(sockfd + 1, &rfds, NULL, NULL, &timeout);

        if(retval == -1) {
            APNV_PRINT("APNV:__recv_socket error in select!\n");
            close(sockfd);
            rec_result.error_type = NVERR_SYSTEM;
            rec_result.recv_size = result;
            break;
        }
        else if(retval) {
            if (FD_ISSET(sockfd, &rfds)){
                result = recv(sockfd, (char *)buffer + received, size - received, 0);
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
                APNV_PRINT("APNV:__recv_socket 888888888888888888\n");
            }
        }
        else {
            APNV_PRINT("APNV:__recv_socket timeout\n");
            close(sockfd);
            rec_result.error_type = NVERR_TIMEOUT;
            rec_result.recv_size = -1;
            break;
        }
	}
    if(rec_result.recv_size < 0){
        rec_result.error_type = NVERR_SYSTEM;
    }
	return rec_result.error_type;
}


static int _send_socket(int sockfd, void* buffer, int size)
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

int main(void)
{
    struct sockaddr_un serv_addr;
    int ret, server_sock, client_sock,retval;
    char filename[MAX_NAME_LEN];
    PRO_NV pro_nv;
    uint8 * buffer = NULL;
    NVITEM_ERROR_E err;

    productnv_init();
    /* init unix domain socket */
    memset(&serv_addr, 0, sizeof(serv_addr));
    serv_addr.sun_family = AF_UNIX;
    strcpy(serv_addr.sun_path, PRONV_SOCKET_FILE);
    unlink(serv_addr.sun_path);

    server_sock = socket(AF_UNIX, SOCK_STREAM, 0);
    if (server_sock < 0) {
        APNV_PRINT("APNV:create socket failed!");
        return -1;
    }
    APNV_PRINT("APNV:create socket success!");

    if (bind(server_sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) {
        APNV_PRINT("APNV:bind socket failed!");
        close(server_sock);
        return -1;
    }
    APNV_PRINT("APNV:bind socket success!");

    if (listen(server_sock, 10) < 0) {
        APNV_PRINT("APNV:listen socket failed!");
        close(server_sock);
        return -1;
    }
    APNV_PRINT("APNV:listen socket success!");
    while(1){
		client_sock = accept(server_sock, NULL, NULL);
		if (client_sock < 0) {
			APNV_PRINT("APNV:accept failed!");
			sleep(1);
			continue;
		}
		APNV_PRINT("APNV:accept success!");

		err = _recv_socket(client_sock, &pro_nv, sizeof(pro_nv));
		if(err != NVERR_NONE){
            pro_nv.ope_result = err;
            _send_socket(client_sock, &pro_nv, sizeof(pro_nv));
            close(client_sock);
            continue;
		}

		APNV_PRINT("APNV:recv data success! pro_nv.type = %d",pro_nv.type);

        switch(pro_nv.type) {
        case PRO_NV_WRITE:
            buffer = malloc(pro_nv.param.writeParam.size);
            err = _recv_socket(client_sock,buffer, pro_nv.param.writeParam.size);
            if(err != NVERR_NONE){
                pro_nv.ope_result = err;
                APNV_PRINT("APNV:recv buffer failed rev_res.error_type = %d!",err);
                _send_socket(client_sock, &pro_nv, sizeof(pro_nv));
                break;
            }
            pro_nv.ope_result = PRONV_WriteItem(pro_nv.param.writeParam.item_id,buffer,pro_nv.param.writeParam.size);
            _send_socket(client_sock, &pro_nv, sizeof(pro_nv));
            free(buffer);
            break;

        case PRO_NV_READ:
            buffer = malloc(pro_nv.param.readParam.size);
            pro_nv.ope_result = PRONV_ReadItem(pro_nv.param.readParam.item_id,buffer,pro_nv.param.readParam.size);
            _send_socket(client_sock, &pro_nv, sizeof(pro_nv));
            if(NVERR_NONE == pro_nv.ope_result) {
                _send_socket(client_sock, buffer, pro_nv.param.readParam.size);
            }
            free(buffer);
            break;

        default:
            APNV_PRINT("APNV: nv type %d error  failed!",pro_nv.type);
            break;
        }
        close(client_sock);
    }
    return 0;
}

void apnv_exit(void)
{
    productnv_unInit();
}
