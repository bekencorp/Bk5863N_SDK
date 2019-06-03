#ifndef _ISOIEC7816_H_
#define _ISOIEC7816_H_


#define ISOIEC7816_DEFAULT_RATE         (264258)  ///(22021) ///(264258)
//#define ISOIEC7816_DEFAULT_RATE        (21973)
#define ISOIEC7816_RECEIVE_TIMEOUT		(300)

typedef struct{
	unsigned op_st : 1;
	unsigned active : 1;
	unsigned rx_st : 1;  ////iso/iec7816 rx status
	unsigned tx_st : 1;  ////iso/iec7816 tx status
	unsigned rec_timerout : 16;
}_isoiec7816_status_t;

typedef enum{
	IOSIEC7816_CTL_RESET = 0,
	IOSIEC7816_CTL_W_RESET,
	IOSIEC7816_CTL_SET_RATE,


	
}_iosiec7816_ctl_t;
	
void isoiec7816_hardware_init(void);
void isoiec7816_hardware_deinit(void);
int isoiec7816_hardware_open(int ops,char *buf,int len);
int isoiec7816_hardware_close(int ops,char *buf,int len);
int isoiec7816_hardware_write(int ops,char *buf,int len);
int isoiec7816_hardware_read(int ops,char *buf,int expect_len,unsigned char *rec_len);
int isoiec7816_hardware_control(_iosiec7816_ctl_t ops,void *arg);


extern _isoiec7816_status_t _isoiec7816;
#endif

/***********************  END OF FILES  ***********************/
