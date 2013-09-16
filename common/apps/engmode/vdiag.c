#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <errno.h>
#include <signal.h>
#include <sys/ioctl.h>
#include "engopt.h"
#include "eng_audio.h"
#include "eng_diag.h"
#include <ctype.h>
#include "cutils/properties.h"
#include "private/android_filesystem_config.h"
#include <termios.h>
#include "eng_pcclient.h"
#include "eng_util.h"

int g_ass_start = 0;

#define DATA_BUF_SIZE (4096*2)
#define MAX_OPEN_TIMES  10
#define DATA_EXT_DIAG_SIZE (4096*2)

static char log_data[DATA_BUF_SIZE];
static char ext_data_buf[DATA_EXT_DIAG_SIZE];
static char backup_data_buf[DATA_EXT_DIAG_SIZE];
static int ext_buf_len,backup_data_len;
static int g_diag_status = ENG_DIAG_RECV_TO_AP;
//AUDIO_TOTAL_T audio_total[4];
extern int adev_get_audiomodenum4eng(void);
AUDIO_TOTAL_T *audio_total = NULL;

static int s_speed_arr[] = {B921600,B115200, B38400, B19200, B9600, B4800, B2400, B1200, B300,
    B921600,B115200, B38400, B19200, B9600, B4800, B2400, B1200, B300, };
static int s_name_arr[] = {921600,115200,38400,  19200, 9600,  4800,  2400,  1200,  300,
    921600, 115200,38400,  19200,  9600, 4800, 2400, 1200,  300, };

static void print_log_data(int cnt)
{
    int i;

    if (cnt > DATA_BUF_SIZE/2)
        cnt = DATA_BUF_SIZE/2;

    ENG_LOG("eng_vdiag vser receive:\n");
    for(i = 0; i < cnt; i++) {
        if (isalnum(log_data[i])){
            ENG_LOG("eng_vdiag %c ", log_data[i]);
        }else{
            ENG_LOG("eng_vdiag %2x ", log_data[i]);
        }
    }
    ENG_LOG("\n");
}

void init_user_diag_buf(void)
{
    memset(ext_data_buf,0,DATA_EXT_DIAG_SIZE);
    ext_buf_len = 0;
}

// Find a valid diag framer.
int get_user_diag_buf(char* buf,int len)
{
    int i;
    int is_find = 0;

    for(i = 0; i< len; i++){
        ENG_LOG("eng_vdiag %s: %x\n",__FUNCTION__, buf[i]);

        if (buf[i] == 0x7e && ext_buf_len ==0){ //start
            ext_data_buf[ext_buf_len++] = buf[i];
        }else if (ext_buf_len > 0 && ext_buf_len < DATA_EXT_DIAG_SIZE){
            ext_data_buf[ext_buf_len]=buf[i];
            ext_buf_len++;
            if ( buf[i] == 0x7e ){
                is_find = 1;
                break;
            }
        }
    }
#if 0 // FOR DBEUG
    if ( is_find ) {
        for(i = 0; i < ext_buf_len; i++) {
            ENG_LOG("eng_vdiag 0x%x, ",ext_data_buf[i]);
        }
    }
#endif    
    return is_find;
}

int check_audio_para_file_size(char *config_file)
{
    int fileSize = 0;
    int tmpFd;

    ENG_LOG("%s: enter",__FUNCTION__);
    tmpFd = open(config_file, O_RDONLY);
    if (tmpFd < 0) {
        ENG_LOG("%s: open error",__FUNCTION__);
        return -1;
    }
    fileSize = lseek(tmpFd, 0, SEEK_END);
    if (fileSize <= 0) {
        ENG_LOG("%s: file size error",__FUNCTION__);
        close(tmpFd);
        return -1;
    }
    close(tmpFd);
    ENG_LOG("%s: check OK",__FUNCTION__);
    return 0;
}

int ensure_audio_para_file_exists(char *config_file)
{
    char buf[2048];
    int srcfd, destfd;
    struct stat sb;
    int nread;
    int ret;

    ENG_LOG("%s: enter",__FUNCTION__);
    ret = access(config_file, R_OK|W_OK);
    if ((ret == 0) || (errno == EACCES)) {
        if ((ret != 0) &&
                (chmod(config_file, S_IRUSR|S_IWUSR|S_IRGRP|S_IWGRP) != 0)) {
            ALOGE("eng_vdiag Cannot set RW to \"%s\": %s", config_file, strerror(errno));
            return -1;
        }
        if (0 == check_audio_para_file_size(config_file)) {
            ENG_LOG("%s: ensure OK",__FUNCTION__);
            return 0;
        }
    } else if (errno != ENOENT) {
        ALOGE("eng_vdiag Cannot access \"%s\": %s", config_file, strerror(errno));
        return -1;
    }

    srcfd = open((char *)(ENG_AUDIO_PARA), O_RDONLY);
    if (srcfd < 0) {
        ALOGE("eng_vdiag Cannot open \"%s\": %s", (char *)(ENG_AUDIO_PARA), strerror(errno));
        return -1;
    }

    destfd = open(config_file, O_CREAT|O_RDWR, 0660);
    if (destfd < 0) {
        close(srcfd);
        ALOGE("eng_vdiag Cannot create \"%s\": %s", config_file, strerror(errno));
        return -1;
    }

    ENG_LOG("%s: start copy",__FUNCTION__);
    while ((nread = read(srcfd, buf, sizeof(buf))) != 0) {
        if (nread < 0) {
            ALOGE("eng_vdiag Error reading \"%s\": %s",(char *)(ENG_AUDIO_PARA) , strerror(errno));
            close(srcfd);
            close(destfd);
            unlink(config_file);
            return -1;
        }
        write(destfd, buf, nread);
    }

    close(destfd);
    close(srcfd);

    /* chmod is needed because open() didn't set permisions properly */
    if (chmod(config_file, 0660) < 0) {
        ALOGE("eng_vdiag Error changing permissions of %s to 0660: %s",
                config_file, strerror(errno));
        unlink(config_file);
        return -1;
    }

    if (chown(config_file, AID_SYSTEM, AID_SYSTEM) < 0) {
        ALOGE("eng_vdiag Error changing group ownership of %s to %d: %s",
                config_file, AID_SYSTEM, strerror(errno));
        unlink(config_file);
        return -1;
    }
    ENG_LOG("%s: ensure done",__FUNCTION__);
    return 0;
}

static void set_raw_data_speed(int fd, int speed)
{
    unsigned long i = 0;
    int   status = 0;
    struct termios Opt;

    tcflush(fd,TCIOFLUSH);

    tcgetattr(fd, &Opt);
    for ( i= 0;  i  < sizeof(s_speed_arr) / sizeof(int);  i++){
        if  (speed == s_name_arr[i])  {
            /*set raw data mode */
            Opt.c_iflag &= ~(IGNBRK | BRKINT | PARMRK | ISTRIP | INLCR | IGNCR | ICRNL | IXON);
            Opt.c_oflag &= ~OPOST;
            Opt.c_lflag &= ~(ECHO | ECHONL | ICANON | ISIG | IEXTEN);
            Opt.c_cflag &= ~(CSIZE | PARENB);
            Opt.c_cflag |= CS8;
            Opt.c_iflag = ~(ICANON|ECHO|ECHOE|ISIG);
            Opt.c_oflag = ~OPOST;
            cfmakeraw(&Opt);
            /* set baudrate*/
            cfsetispeed(&Opt, s_speed_arr[i]);
            cfsetospeed(&Opt, s_speed_arr[i]);
            status = tcsetattr(fd, TCSANOW, &Opt);
            if  (status != 0)
                perror("tcsetattr fd1");
            break;
        }
    }
}

void *eng_vdiag_thread(void *x)
{
    int sipc_fd;
    int ser_fd;
    int r_cnt, w_cnt, offset;
    int has_processed = 0;
    int audio_fd;
    int wait_cnt = 0;
    int type;
    int ret=0;
    struct eng_param * param = (struct eng_param *)x;

    if(param == NULL){
        ALOGE("eng_vdiag invalid input\n");
        return NULL;
    }

    if(param->califlag)
        initialize_ctrl_file();

    /*open usb/usart*/
    ser_fd = eng_open_dev(s_connect_ser_path[param->connect_type], O_RDONLY);
    if(ser_fd < 0){
        ENG_LOG("eng_vdiag cannot open general serial\n");
        return NULL;
    }

    if(param->connect_type == CONNECT_UART){
        set_raw_data_speed(ser_fd, 115200);
    }

    /*open SIPC*/
    do{
        sipc_fd = open(s_cp_pipe[param->cp_type], O_WRONLY);
        if(sipc_fd < 0) {
            ENG_LOG("eng_vdiag cannot open %s, times:%d\n", s_cp_pipe[param->cp_type], wait_cnt);
            if(wait_cnt++ >= MAX_OPEN_TIMES){
                ENG_LOG("eng_vdiag cannot open SIPC, try times exceed the max open times\n");
                close(ser_fd);
                return NULL;
            }
            sleep(5);
        }
    }while(sipc_fd < 0);

 
    audio_total = calloc(1,sizeof(AUDIO_TOTAL_T)*adev_get_audiomodenum4eng());
    if(!audio_total)
    {
        ENG_LOG("eng_vdiag_thread malloc audio_total memory error\n");
	 close(sipc_fd);
	 close(ser_fd);
        return NULL;
    }
    memset(audio_total, 0, sizeof(AUDIO_TOTAL_T)*adev_get_audiomodenum4eng());
    ret = ensure_audio_para_file_exists((char *)(ENG_AUDIO_PARA_DEBUG));
    eng_getpara();
    ENG_LOG("eng_vdiag put diag data from serial to SIPC\n");

    // initialize extra data buffer
    init_user_diag_buf();

    while(1) {
        memset(log_data, 0, sizeof(log_data));
        r_cnt = read(ser_fd, log_data, DATA_BUF_SIZE/2);
        if (r_cnt == DATA_BUF_SIZE/2) {
            r_cnt += read(ser_fd, log_data+r_cnt, DATA_BUF_SIZE/2);
        }

        if (r_cnt <= 0) {
            ENG_LOG("eng_vdiag read log data error  from serial: %s\n", strerror(errno));
            close(ser_fd);

            wait_cnt = 0; //reset wait count
            do {
                ser_fd = eng_open_dev(s_connect_ser_path[param->connect_type], O_RDONLY);
                if(ser_fd < 0) {
                    ENG_LOG("eng_vdiag cannot open vendor serial: %s, error: %s\n",
                            s_connect_ser_path[param->connect_type], strerror(errno));
                    sleep(1);
                }else {
                    ENG_LOG("eng_vdiag reopen serial port success.\n");
                    break;
                }

                if((++wait_cnt) > MAX_OPEN_TIMES) {
                    ENG_LOG("eng_vdiag serial port open times exceed the max open times !!!\n");
                    close(sipc_fd);
                    return NULL;
                }
            } while(ser_fd < 0);

            continue; // nothing has been read
        }

        has_processed = 0; // reset the process flag
        switch(g_diag_status) {
            case ENG_DIAG_RECV_TO_CP:
                memcpy(backup_data_buf,log_data,r_cnt);
                backup_data_len = r_cnt;
                if(0x7E == log_data[r_cnt - 1]){
                    g_diag_status = ENG_DIAG_RECV_TO_AP;
                }else{
                    g_diag_status = ENG_DIAG_RECV_TO_CP;
                }
                break;
            case ENG_DIAG_RECV_TO_AP:
                if(get_user_diag_buf(log_data,r_cnt)){
                    g_diag_status = ENG_DIAG_RECV_TO_AP;
                    memcpy(backup_data_buf,ext_data_buf,ext_buf_len);
                    backup_data_len = ext_buf_len;
                    has_processed = eng_diag(ext_data_buf,ext_buf_len);
                    init_user_diag_buf(); // complete diag framer, so AP decide the following oper
                }else if(0 == ext_buf_len){
                    g_diag_status = ENG_DIAG_RECV_TO_AP;
                    memcpy(backup_data_buf,log_data,r_cnt);
                    backup_data_len = r_cnt; // not a diag framer, so send data to CP
                }else if(DATA_EXT_DIAG_SIZE == ext_buf_len){
                        ENG_LOG("%s: Current data is not a complete diag framer,but buffer is full\n", __FUNCTION__);
                        type = eng_diag_parse(ext_data_buf, ext_buf_len);
                        if(type != CMD_COMMON){
                            g_diag_status = ENG_DIAG_RECV_TO_AP;
                            ENG_LOG("%s: Buffer is full, so AP will not process the next package\n", __FUNCTION__);
                            has_processed = 1; // continue to receive the diag_framer
                        }else{
                            memcpy(backup_data_buf,ext_data_buf,ext_buf_len);
                            backup_data_len = ext_buf_len;
                            init_user_diag_buf();
                            g_diag_status = ENG_DIAG_RECV_TO_CP; // send diag framer to CP
                        }
                } else {
                    g_diag_status = ENG_DIAG_RECV_TO_AP;
                    has_processed = 1; // continue to receive the diag framer
                }
                break;
            default:
                ENG_LOG("%s: ERROR STATUS: %d!!!\n", __FUNCTION__, g_diag_status);
                break;
        }

        if(1 == has_processed){// Data has been processed & should not send to modem
            backup_data_len = 0;
            continue;
        }

        if(2 == r_cnt && log_data[1] == 0xa){
            ENG_LOG("eng_vdiag: start to dump memory");
            g_ass_start = 1;
        }

        offset = 0; // reset offset value
        do {
            w_cnt = write(sipc_fd, backup_data_buf + offset, backup_data_len);
            if (w_cnt < 0) {
                ENG_LOG("eng_vdiag no log data write:%d ,%s\n", w_cnt, strerror(errno));
                continue;
            }else{
                backup_data_len -= w_cnt;
                offset += w_cnt;
            }
            ENG_LOG("eng_vdiag: rcnt:%d, w_cnt:%d, offset:%d\n", r_cnt, w_cnt, offset);
        }while(backup_data_len >0);
    }
    close(sipc_fd);
    close(ser_fd);
    return 0;
}
