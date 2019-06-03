#include "sl_esam.h"
#include "isoiec7816.h"
#include "sl_des.h"

char esam_rec_cmd(char *pRecBuf, unsigned short u16RecLen,unsigned char clear_mode)
{
	unsigned char ack_len = 0;
	
	if(clear_mode == ESAM_RX_MODE_RETAIN_RXFIFO)
	{
//		if(!uart_receive_Retain(pRecBuf, u16RecLen, &ack_len))
//		{
//			return 0;
//		}
		if(!isoiec7816_hardware_read(0,pRecBuf, u16RecLen, &ack_len))
		{
			return 0;
		}
	}
	else
	{
		if(!isoiec7816_hardware_read(0,pRecBuf, u16RecLen, &ack_len))
		{
			return 0;
		}
	}
	return 1;
}


int esam_tranceive_receive(char *pSendBuf, unsigned short u16SendLen,char *pResBuf, unsigned short *pResLen)
{
	unsigned char ret_flag    = 0;
	unsigned char cmd_ins;
	unsigned char rec_len;
	unsigned short send_len = u16SendLen;
	unsigned char rec_ins     = 0;
	char *p_rec_buf;
	char *p_send_buf;

	p_rec_buf     = pResBuf;
	p_send_buf    = pSendBuf;
	cmd_ins         = (unsigned char)*(pSendBuf + ESAM_INS_OFFSET);
	rec_len         = (unsigned char)*(pSendBuf + ESAM_LC_OFFSET);

	if(send_len >= ESAM_CMD_HEAD_SIZE)
	{
		send_len -= ESAM_CMD_HEAD_SIZE;
	}
	else
	{
		return 0;
	}
	
	/* 发送命令头 */
	if(isoiec7816_hardware_write(0,p_send_buf, ESAM_CMD_HEAD_SIZE) == 0)
	{
		return 0;
	}
	//ResetGPIOPin(GPIOB,GPIO_Pin_6);

	p_send_buf += 5;
	*pResLen = 0;
	do
	{
		if(0 == esam_rec_cmd((char*)&rec_ins,1,ESAM_RX_MODE_CLEAR_RXFIFO))
		{
			return 0;
		}
		
		if(cmd_ins == rec_ins)
		{
			if(u16SendLen > ESAM_CMD_HEAD_SIZE)
			{
				if(0 == isoiec7816_hardware_write(0,p_send_buf, send_len))
				{
					return 0;
				}
			}
			else
			{   
				if(0 == esam_rec_cmd(p_rec_buf, rec_len,ESAM_RX_MODE_RETAIN_RXFIFO))
				{
					return 0;
				}
				*pResLen = rec_len;
				p_rec_buf += rec_len;
			}
		}
		else if(rec_ins == (cmd_ins^0xff)) 
		{
			if(u16SendLen > ESAM_CMD_HEAD_SIZE)
			{
				if(0 == isoiec7816_hardware_write(0,p_send_buf,1))
				{
					return 0;
				}
				p_send_buf++;
				send_len--;
			}
			else
			{
				if(0 == esam_rec_cmd(p_rec_buf, 1,ESAM_RX_MODE_RETAIN_RXFIFO))
				{
					return 0;
				}
				p_rec_buf++;
				*pResLen += 1;
			}
		}
		else if(0x60 == (rec_ins & 0xF0)) 
		{
			/* 若为0x60等待，反之，为状态字 */
			if(0x60 != rec_ins)
			{
				*p_rec_buf = rec_ins;
				p_rec_buf++;
				*pResLen += 1;
				///接收SW2
				if(0 == esam_rec_cmd(p_rec_buf,1,ESAM_RX_MODE_RETAIN_RXFIFO))
				{
					return 0;
				}
				ret_flag = 1;
				*pResLen += 1;
			}                
		}
		else if(0x90 == (rec_ins&0xF0))            
		{
			*p_rec_buf = rec_ins;
			p_rec_buf++;
			*pResLen += 1;
			//接收SW2
			if(0 == esam_rec_cmd(p_rec_buf,1,ESAM_RX_MODE_RETAIN_RXFIFO))
			{
				return 0;
			}
			ret_flag = 1;
			*pResLen += 1;
		}
		else
		{
			return 0;
		}
	}while(0 == ret_flag);
	return 1;
}

unsigned short esam_cmd_parse(char *pSendBuf, unsigned short u16SendLen, char *pResBuf, unsigned short *pResLen)
{
	unsigned char ret_flag     = 0;
	unsigned char retry_times    = 0;
	unsigned short res_len = 0;
	*pResLen = 0;
	
	do
	{
		if(0 == esam_tranceive_receive(pSendBuf,u16SendLen,pResBuf,&res_len))
		{
			return 0;
		}
		
		if(res_len == ESAM_SW1SW2_LENGTH)
		{
			///鏀跺埌杩樿鎺ユ敹鐨勪釜鏁帮紝浠庢柊缁勫寘
			if((0x61 == *(pResBuf + res_len - ESAM_SW1SW2_LENGTH))&&(retry_times < ESAM_MAX_RETRY_TIMES))
			{
				///ESAMHaveData_Printf(pResBuf,pResLen[0],pSendBuf[0]);
Re:				
				/* 鍙栧簲绛? */
				if((*(pResBuf + res_len - 1))==0)
				{
					*(pSendBuf + ESAM_LC_OFFSET) = 0xFF;
				}
				else
				{
					*(pSendBuf + ESAM_LC_OFFSET) = *(pResBuf + res_len - 1);
				}
				*(pSendBuf + ESAM_P2_OFFSET) = 0x00;
				*(pSendBuf + ESAM_P1_OFFSET) = 0x00;
				*(pSendBuf + ESAM_INS_OFFSET) = 0xc0;
				*pSendBuf = 0x00;  ///CLA
				u16SendLen = ESAM_CMD_HEAD_SIZE;
				if(retry_times)  ///鎺ユ敹鍦板潃鍋忕Щ
					pResBuf += 0xFF;            
			}
			else
			{
				*pResLen += res_len;
				ret_flag = 1;
			}
			retry_times++;
		}
		else if (res_len > ESAM_SW1SW2_LENGTH)
		{
			if((0x61 == (*(pResBuf + res_len - ESAM_SW1SW2_LENGTH)))&&(retry_times < ESAM_MAX_RETRY_TIMES))
			{
				*pResLen += (res_len - ESAM_SW1SW2_LENGTH);
				goto Re;
			}
			else
			{
				*pResLen += res_len;
				ret_flag = 1;
			}
		}
		else
		{
			//鍏抽棴鎺ユ敹            
			return 0;
		}
	}while(0 == ret_flag);
	//鍏抽棴鎺ユ敹

	return 1;
}


void esam_hardware_init(void)
{
	
}

void esam_hardware_deinit(void)
{
	
}

int esam_hardware_open(int ops,char *buf,int len)
{
	unsigned char rec_len;
	unsigned int rate;
	int ret;
	
	isoiec7816_hardware_open(0,0,0);
	if(ops == 0)
	{
		rate = ESAM_ISO7816_DEFAULT_RATE;
		isoiec7816_hardware_control(IOSIEC7816_CTL_SET_RATE,&rate);
		isoiec7816_hardware_control(IOSIEC7816_CTL_RESET,0);
	}
	else
	{
		isoiec7816_hardware_control(IOSIEC7816_CTL_W_RESET,0);
	}
	
	rec_len = MIN(ESAM_RESET_DEFAULT_LENGTH,len);
	ret = isoiec7816_hardware_read(0,buf,rec_len,&rec_len);
	if(ret == 1)
	{
		ESAM_LOG_ARRAY(ESAM_LOG_OUT_DEFAULT_GRADE,"esam open reset ok",rec_len,buf);
		return (rec_len == ESAM_RESET_DEFAULT_LENGTH)?rec_len:1;
	}
	else
	{
		ESAM_LOG_ARRAY(ESAM_LOG_OUT_DEFAULT_GRADE,"esam open reset failed or buf == null or !RESET_DEFAULT_LENGTH",rec_len,buf);
		return ESAM_ERR_OPS_FLAG;
	}
//	return 0;
}

int esam_hardware_close(int ops,char *buf,int len)
{
	isoiec7816_hardware_close(0,0,0);
	return 0;
}

int esam_hardware_operation(int ops,char *sd,unsigned short sd_len,char *out,int out_len)
{
	unsigned short sw1sw2;
	unsigned short rd_len = 0;
	
	if((ops == 0)&&(!sd || !sd_len || !out || !out_len))
		return ESAM_ERR_OPS_FLAG;
	sw1sw2 = esam_cmd_parse(sd, sd_len, out, &rd_len);
	ESAM_LOG_ARRAY(ESAM_LOG_OUT_DEFAULT_GRADE,"operation",rd_len,out);
	if((1 == sw1sw2)&&(rd_len > 0))
	{
	#if ESAM_GET_ALL_CHIP_RETURN_MSG_CFG
		return rd_len;
	#else
		if(rd_len >= 2)
			return rd_len - 2;
		else
			return ESAM_ERR_OPS_FLAG;
	#endif
	}
	return ESAM_ERR_OPS_FLAG;
}

int esam_hardware_control(_esam_ctl_t ops,void *arg)
{
	int ret = 0;
	switch(ops)
	{
		case ESAM_CTL_GET_RD4:
			esam_get_random(4,arg);
			ret = (ret > 0)?0:-1;
			break;
		case ESAM_CTL_GET_RD8:
			ret = esam_get_random(8,arg);
			ret = (ret > 0)?0:-1;
			break;
		case ESAM_CTL_GET_RD16:
			esam_get_random(0x10,arg);
			ret = (ret > 0)?0:-1;
			break;
		case ESAM_CTL_SEL_FIL:
			
			break;
		default:
			break;
	}
	return ret;
}

int esam_get_random(unsigned char read_len,unsigned char *rd)
{
	unsigned short sw1sw2;
	char t_rd[10];
	unsigned short rd_len;
	char  CmdPack[5] = {0x00,0x84,0x00,0x00,0x08}; 
	
	if(read_len != 8 && read_len != 4 && read_len != 0x10)
		return ESAM_ARG_ERR_FLAG;
	CmdPack[4] = read_len;
	
	sw1sw2 = esam_cmd_parse(CmdPack, 5, t_rd, &rd_len);
	ESAM_LOG_ARRAY(ESAM_LOG_OUT_DEFAULT_GRADE,"Ramdom",rd_len,t_rd);
	if((1 == sw1sw2)&&(rd_len >= 2))
	{
		sw1sw2 = (t_rd[rd_len - 2]<<8)+t_rd[rd_len - 1];
		if(sw1sw2 == 0x9000)
		{
			ESAM_SUCCEEND_RETURN(t_rd,rd_len,rd,read_len);
		}
		else
		{
			ESAM_STATUS_RETURN(t_rd,rd_len,rd,read_len);
		}
	}
	return  ESAM_ERR_OPS_FLAG;       
}

int esam_select_file(unsigned int fid,unsigned char *resdat,unsigned short llen)
{
	unsigned short sw1sw2;
	unsigned short length;
	char ack[255+2];
	char  CmdPack[7] = {0x00,0xA4,0x00,0x00,0x02}; 
	
	CmdPack[5] = 0xFF&(fid>>8);
	CmdPack[6] = 0xFF&fid;
	sw1sw2 = esam_cmd_parse(CmdPack, 7, (char *)ack, &length);
	ESAM_LOG_ARRAY(ESAM_LOG_OUT_DEFAULT_GRADE,"fid",length,(char*)ack);
	if((1 == sw1sw2) && (length >= 2))
	{
		sw1sw2 = (ack[length-2]<<8)+ack[length-1];
		if(sw1sw2 == 0x9000)
		{
			ESAM_SUCCEEND_RETURN(ack,length,resdat,llen);
		}
		else
		{
			ESAM_STATUS_RETURN(ack,length,resdat,llen);
		}
	}

	return  ESAM_ERR_OPS_FLAG;       
}

int esam_get_sn(char *resdat,unsigned short llen)
{
	unsigned short sw1sw2;
	unsigned short length;
	char ack[100];
	char  CmdPack[5] = {0x80,0xf6,0x00,0x03,0x04}; 
	sw1sw2 = esam_cmd_parse(CmdPack, 5, ack, &length);
	ESAM_LOG_ARRAY(ESAM_LOG_OUT_DEFAULT_GRADE,"sn",length,(char*)ack);
	if((1 == sw1sw2) && (length >= 2))
	{
		sw1sw2 = (ack[length-2]<<8)+ack[length-1];
		if(sw1sw2 == 0x9000)
		{
			ESAM_SUCCEEND_RETURN(ack,length,resdat,llen);
		}
		else
		{
			ESAM_STATUS_RETURN(ack,length,resdat,llen);
		}
	}

	return  ESAM_ERR_OPS_FLAG;  
}

int esam_decrease_counter(char *resdat,unsigned short llen)
{
	unsigned short sw1sw2;
	unsigned short length;
	char ack[100];
	char  CmdPack[5] = {0x00,0x59,0x00,0x00,0x01};
	
	sw1sw2 = esam_cmd_parse(CmdPack, 5, ack, &length);
	ESAM_LOG_ARRAY(ESAM_LOG_OUT_DEFAULT_GRADE,"decrease_counter",length,(char*)ack);
	if((1 == sw1sw2) && (length >= 2))
	{
		sw1sw2 = (ack[length-2]<<8)+ack[length-1];
		if(sw1sw2 == 0x9000)
		{
			ESAM_SUCCEEND_RETURN(ack,length,resdat,llen);
		}
		else
		{
			ESAM_STATUS_RETURN(ack,length,resdat,llen);
		}
	}
	
	return  ESAM_ERR_OPS_FLAG;       
}

int esam_get_response(char le,unsigned char *resdat,unsigned short llen)
{
	unsigned short sw1sw2;
	unsigned short length;
	char ack[255+2+2];
	char  CmdPack[5] = {0x00,0xC0,0x00,0x00,0}; 
	CmdPack[4] = le;
	sw1sw2 = esam_cmd_parse(CmdPack, 5, (char *)ack, &length);
	ESAM_LOG_ARRAY(ESAM_LOG_OUT_DEFAULT_GRADE,"get_response",length,(char*)ack);
	if((1 == sw1sw2) && (length >= 2))
	{
		sw1sw2 = (ack[length-2]<<8)+ack[length-1];
		if(sw1sw2 == 0x9000)
		{
			ESAM_SUCCEEND_RETURN(ack,length,resdat,llen);
		}
		else
		{
			ESAM_STATUS_RETURN(ack,length,resdat,llen);
		}
	}

	return  ESAM_ERR_OPS_FLAG;       
}

/*
 *DF01--EF01
 *用于读出应用车辆信息文件中的数据，读出的数据为密文
 *dat:随机数(8B) + 期望读取的数据明文长度(1B) +密钥版本(1B)
 *
*/
int esam_read_data(char p1,char p2,char *dat,char*resdat,unsigned short llen)
{
	unsigned short sw1sw2;
	unsigned short length;
	char ack[255+2+2];
	char  CmdPack[5 + 0x0A];
	
	CmdPack[0] = 0x00;
	CmdPack[1] = 0xB4;
	CmdPack[2] = p1;
	CmdPack[3] = p2;
	CmdPack[4] = 0x0A;
	memcpy(&CmdPack[5],dat,0x0A);
	sw1sw2 = esam_cmd_parse(CmdPack, 5+0x0A, ack, &length);
	ESAM_LOG_ARRAY(ESAM_LOG_OUT_DEFAULT_GRADE,"read_data",length,(char*)ack);
	if((1 == sw1sw2) && (length >= 2))
	{
		sw1sw2 = (ack[length-2]<<8)+ack[length-1];
		if(sw1sw2 == 0x9000)
		{
			ESAM_SUCCEEND_RETURN(ack,length,resdat,llen);
		}
		else
		{
			ESAM_STATUS_RETURN(ack,length,resdat,llen);
		}
	}
	
	return  ESAM_ERR_OPS_FLAG;
}

int esam_read_binary_no_mac(char p1,char p2,unsigned char ReadLen,char*resdat,unsigned short llen)
{
	unsigned short sw1sw2;
	unsigned short length;
	char ack[255+2+2];
	char  CmdPack[5];   /// = {0x00,0xB0,0x00,0x00,0x02}; 
	
	CmdPack[0] = 0x00;
	CmdPack[1] = 0xB0;
	CmdPack[2] = p1;
	CmdPack[3] = p2;
	CmdPack[4] = ReadLen;
	sw1sw2 = esam_cmd_parse(CmdPack, 5, ack, &length);
	ESAM_LOG_ARRAY(ESAM_LOG_OUT_DEFAULT_GRADE,"read_binary(no mac)",length,(char*)ack);
	if((1 == sw1sw2) && (length >= 2))
	{
		sw1sw2 = (ack[length-2]<<8)+ack[length-1];
		if(sw1sw2 == 0x9000)
		{
			ESAM_SUCCEEND_RETURN(ack,length,resdat,llen);
		}
		else
		{
			ESAM_STATUS_RETURN(ack,length,resdat,llen);
		}
	}
	
	return  ESAM_ERR_OPS_FLAG;
}

/*
 *
 *若P1的b8 = 0，P1<6:0>为文件高位地址，P2为文件的低位地址
 *若P1的b8 = 1，P2为文件地址
 *
*/
int esam_read_binary_mac(char p1,char p2,char *mac,char *key,char*resdat,unsigned short llen)
{
	unsigned short sw1sw2;
	unsigned short length;
	char ack[255+2+2];
	char CmdPack[15]; /// = {0x04,0xB0,0x00,0x00,0x04}; 
	
	CmdPack[0] = 0x04;
	CmdPack[1] = 0xB0;
	CmdPack[2] = p1;
	CmdPack[3] = p2;
	CmdPack[4] = 4;
	memcpy(&CmdPack[5],mac,4);
	
	sw1sw2 = esam_cmd_parse(CmdPack, 9, ack, &length);
	ESAM_LOG_ARRAY(ESAM_LOG_OUT_DEFAULT_GRADE,"read_binary(no mac)",length,(char*)ack);
	if((1 == sw1sw2) && (length >= 2))
	{
		sw1sw2 = (ack[length-2]<<8)+ack[length-1];
		if(sw1sw2 == 0x9000)
		{
			ESAM_SUCCEEND_RETURN(ack,length,resdat,llen);
		}
		else
		{
			ESAM_STATUS_RETURN(ack,length,resdat,llen);
		}
	}
	
	return	ESAM_ERR_OPS_FLAG;
}

/**
 * read record
 * P1 : 记录号
 * P2 ：xxxx x100  --> P1 指定的记录号
 *      0000 0---  --> 当前文件
 *      xxxx x---  --> 通过SFI方式访问
 * Le : 期望返回的记录数据
 *
**/
int esam_read_record(char p1,char p2,char le,
	char *key,char*resdat,unsigned short llen)
{
	unsigned short sw1sw2;
	unsigned short length;
	char ack[255+2+2];
	char CmdPack[15]; /// = {0x04,0xB0,0x00,0x00,0x04}; 
	
	CmdPack[0] = 0x00;
	CmdPack[1] = 0xB2;
	CmdPack[2] = p1;       ////
	CmdPack[3] = p2;       ////
	CmdPack[4] = le;       ////
	
	sw1sw2 = esam_cmd_parse(CmdPack, 5, ack, &length);
	ESAM_LOG_ARRAY(ESAM_LOG_OUT_DEFAULT_GRADE,"read_record",length,(char*)ack);
	if((1 == sw1sw2) && (length >= 2))
	{
		sw1sw2 = (ack[length-2]<<8)+ack[length-1];
		if(sw1sw2 == 0x9000)
		{
			ESAM_SUCCEEND_RETURN(ack,length,resdat,llen);
		}
		else
		{
			ESAM_STATUS_RETURN(ack,length,resdat,llen);
		}
	}
	
	return	ESAM_ERR_OPS_FLAG;
}

/**
 * read record mac
 * P1 : 记录号
 * P2 ：xxxx x100  --> P1 指定的记录号
 *      0000 0---  --> 当前文件
 *      xxxx x---  --> 通过SFI方式访问
**/
int esam_read_record_mac(char p1,char p2,char *mac,
	char *key,char*resdat,unsigned short llen)
{
	unsigned short sw1sw2;
	unsigned short length;
	char ack[255+2+2];
	char CmdPack[15]; /// = {0x04,0xB0,0x00,0x00,0x04}; 
	
	CmdPack[0] = 0x04;
	CmdPack[1] = 0xB2;
	CmdPack[2] = p1;       ////
	CmdPack[3] = p2;       ////
	CmdPack[4] = 4;        ////
	memcpy(&CmdPack[5],mac,4);
	sw1sw2 = esam_cmd_parse(CmdPack, 5 + 4, ack, &length);
	ESAM_LOG_ARRAY(ESAM_LOG_OUT_DEFAULT_GRADE,"read_record_mac",length,(char*)ack);
	if((1 == sw1sw2) && (length >= 2))
	{
		sw1sw2 = (ack[length-2]<<8)+ack[length-1];
		if(sw1sw2 == 0x9000)
		{
			ESAM_SUCCEEND_RETURN(ack,length,resdat,llen);
		}
		else
		{
			ESAM_STATUS_RETURN(ack,length,resdat,llen);
		}
	}
	
	return	ESAM_ERR_OPS_FLAG;
}

/**
 * update binary
 * P1 : 0xxx xxxx 当前文件高位地址
 *      100x xxxx 通过SFI方式访问
 * P2 ：若P1的b8 = 0，P2为文件的低位地址
 *      若P1的b8 = 1，P2为文件地址
 * Lc : Data Length
 * dat: 明文数据
**/
int esam_update_binary_no_mac(char p1,char p2,char lc,
	char *dat,char*resdat,unsigned short llen)
{
	unsigned short sw1sw2;
	unsigned short length;
	char *ack;
	char  CmdPack[255+10+2];
	
	ack = CmdPack;
	CmdPack[0] = 0x00;
	CmdPack[1] = 0xD6;
	CmdPack[2] = p1;
	CmdPack[3] = p2;
	CmdPack[4] = lc;
	memcpy(&CmdPack[5],dat,lc);
	sw1sw2 = esam_cmd_parse(CmdPack, 5 + lc, ack, &length);
	ESAM_LOG_ARRAY(ESAM_LOG_OUT_DEFAULT_GRADE,"update_binary",length,(char*)ack);
	if((1 == sw1sw2) && (length >= 2))
	{
		sw1sw2 = (ack[length-2]<<8)+ack[length-1];
		if(sw1sw2 == 0x9000)
		{
			ESAM_SUCCEEND_RETURN(ack,length,resdat,llen);
		}
		else
		{
			ESAM_STATUS_RETURN(ack,length,resdat,llen);
		}
	}
	
	return  ESAM_ERR_OPS_FLAG;
}

/**
 * update binary
 * P1 : 0xxx xxxx 当前文件高位地址
 *      100x xxxx 通过SFI方式访问
 * P2 ：若P1的b8 = 0，P2为文件的低位地址
 *      若P1的b8 = 1，P2为文件地址
 * Lc : Data Length
 * dat: 加密方式：     密文数据
 *      校验方式：          明文数据 || 校验码
 *      校验加密方式： 密文数据 || 校验码
**/
int esam_update_binary_mac(char p1,char p2,char lc,
	char *dat,char*resdat,unsigned short llen)
{
	unsigned short sw1sw2;
	unsigned short length;
	char *ack;
	char  CmdPack[255+10+2];
	
	ack = CmdPack;
	CmdPack[0] = 0x04;
	CmdPack[1] = 0xD6;
	CmdPack[2] = p1;
	CmdPack[3] = p2;
	CmdPack[4] = lc;
	memcpy(&CmdPack[5],dat,lc);
	sw1sw2 = esam_cmd_parse(CmdPack, 5 + lc, ack, &length);
	ESAM_LOG_ARRAY(ESAM_LOG_OUT_DEFAULT_GRADE,"update_binary",length,(char*)ack);
	if((1 == sw1sw2) && (length >= 2))
	{
		sw1sw2 = (ack[length-2]<<8)+ack[length-1];
		if(sw1sw2 == 0x9000)
		{
			ESAM_SUCCEEND_RETURN(ack,length,resdat,llen);
		}
		else
		{
			ESAM_STATUS_RETURN(ack,length,resdat,llen);
		}
	}
	
	return  ESAM_ERR_OPS_FLAG;
}

/**
 * update record
 * P1 : 0x00 : 当前记录
 *      =/= 0x00 :  指定记录号
 * P2 ：0000 0---: 当前文件
 *      xxxx x---: 通过SFI方式访问
 *      ---- -100: P1指定的记录号
 *      ---- -000: 第一条记录
 *      ---- -001: 最后一条记录
 *      ---- -010: 下一条记录
 *      ---- -011: 前一条记录
 *      保留
 * Lc : Data Length
 * dat: 明文数据
**/
int esam_update_record_no_mac(char p1,char p2,char lc,
	char *dat,char*resdat,unsigned short llen)
{
	unsigned short sw1sw2;
	unsigned short length;
	char *ack;
	char  CmdPack[255+10+2];
	
	ack = CmdPack;
	CmdPack[0] = 0x00;
	CmdPack[1] = 0xDC;
	CmdPack[2] = p1;
	CmdPack[3] = p2;
	CmdPack[4] = lc;
	memcpy(&CmdPack[5],dat,lc);
	sw1sw2 = esam_cmd_parse(CmdPack, 5 + lc, ack, &length);
	ESAM_LOG_ARRAY(ESAM_LOG_OUT_DEFAULT_GRADE,"update_record(no mac)",length,(char*)ack);
	if((1 == sw1sw2) && (length >= 2))
	{
		sw1sw2 = (ack[length-2]<<8)+ack[length-1];
		if(sw1sw2 == 0x9000)
		{
			ESAM_SUCCEEND_RETURN(ack,length,resdat,llen);
		}
		else
		{
			ESAM_STATUS_RETURN(ack,length,resdat,llen);
		}
	}
	
	return  ESAM_ERR_OPS_FLAG;
}

/**
 * update record
 * P1 : 0x00 : 当前记录
 *      =/= 0x00 :  指定记录号
 * P2 ：0000 0---: 当前文件
 *      xxxx x---: 通过SFI方式访问
 *      ---- -100: P1指定的记录号
 *      ---- -000: 第一条记录
 *      ---- -001: 最后一条记录
 *      ---- -010: 下一条记录
 *      ---- -011: 前一条记录
 *      保留
 * Lc : Data Length
 * dat: 加密方式：     密文记录数据
 *      校验方式：          明文记录数据 || 校验码
 *      校验加密方式： 密文记录数据 || 校验码
**/
int esam_update_record_mac(char p1,char p2,char lc,
	char *dat,char*resdat,unsigned short llen)
{
	unsigned short sw1sw2;
	unsigned short length;
	char *ack;
	char  CmdPack[255+10+2];
	
	ack = CmdPack;
	CmdPack[0] = 0x04;
	CmdPack[1] = 0xDC;
	CmdPack[2] = p1;
	CmdPack[3] = p2;
	CmdPack[4] = lc;
	memcpy(&CmdPack[5],dat,lc);
	sw1sw2 = esam_cmd_parse(CmdPack, 5 + lc, ack, &length);
	ESAM_LOG_ARRAY(ESAM_LOG_OUT_DEFAULT_GRADE,"update_record",length,(char*)ack);
	if((1 == sw1sw2) && (length >= 2))
	{
		sw1sw2 = (ack[length-2]<<8)+ack[length-1];
		if(sw1sw2 == 0x9000)
		{
			ESAM_SUCCEEND_RETURN(ack,length,resdat,llen);
		}
		else
		{
			ESAM_STATUS_RETURN(ack,length,resdat,llen);
		}
	}
	
	return  ESAM_ERR_OPS_FLAG;
}

/**
 * update key
 * P2 ：0x00 : 更新主控密钥
 *      0xFF ：更新其它密钥
 * Lc : Data Length  0x14 or 0x1C
 * dat: 密文密钥信息 || MAC
 *
**/
int esam_update_key(char p2,char lc,char *dat,
	char*resdat,unsigned short llen)
{
	unsigned short sw1sw2;
	unsigned short length;
	char ack[255+10+2];
	char  CmdPack[5+0x1C];
	
	if((lc != 14) && (lc != 0x1C))
	{
		return -1;
	}
	CmdPack[0] = 0x84;
	CmdPack[1] = 0xD4;
	CmdPack[2] = 0x01;
	CmdPack[3] = p2;
	CmdPack[4] = lc;
	memcpy(&CmdPack[5],dat,lc);
	sw1sw2 = esam_cmd_parse(CmdPack, 5 + lc, ack, &length);
	ESAM_LOG_ARRAY(ESAM_LOG_OUT_DEFAULT_GRADE,"update_record",length,(char*)ack);
	if((1 == sw1sw2) && (length >= 2))
	{
		sw1sw2 = (ack[length-2]<<8)+ack[length-1];
		if(sw1sw2 == 0x9000)
		{
			ESAM_SUCCEEND_RETURN(ack,length,resdat,llen);
		}
		else
		{
			ESAM_STATUS_RETURN(ack,length,resdat,llen);
		}
	}
	
	return  ESAM_ERR_OPS_FLAG;
}

/**
 *内部认证
 *P2:内部认证密钥标识
 *命令报文数据域DATA的内容是应用专用的认证数据
 *67 00:长度Lc不正确
 *69 01:状态无效
 *69 82:不满足安全条件
 *69 85:使用条件不满足
 *6A 82:Key文件未找到
 *94 03:密钥未找到
 *
**/
int esam_internal_authentication(char type,char p2,
	char*dat,char *out,unsigned short limit_len)
{
	unsigned short sw1sw2;
	unsigned char cmd[5+0x08];
	char ack[50];
	unsigned short ack_len;

	cmd[0] = 0x00;
	cmd[1] = 0x88;
	cmd[2] = 0x00;
	cmd[3] = p2;
	cmd[4] = 0x08;
	memcpy(&cmd[5],dat,0x08);
	sw1sw2 = esam_cmd_parse((char*)cmd, 5+0x08, ack, &ack_len);
	ESAM_LOG_ARRAY(ESAM_LOG_OUT_DEFAULT_GRADE,"internal_authentication",ack_len,(char*)ack);
	if((1 == sw1sw2) && (ack_len >= 2))
	{
		sw1sw2 = (ack[ack_len-2]<<8)+ack[ack_len-1];
		if(sw1sw2 == 0x9000)
		{
			ESAM_SUCCEEND_RETURN(ack,ack_len,out,limit_len);
		}
		ESAM_STATUS_RETURN(ack,ack_len,out,limit_len);
	}
	return ESAM_ERR_OPS_FLAG;   ///FALSE
}

/**
 *外部认证
 *P2:指定密钥的算法标识进行数据加密
 *命令报文数据域DATA用指定密钥对随机数(后面填充0x00至8字节(3des)或16字节(SM4)后)做加密运算产生
 *对于SM4算法，认证数据为16字节密文前后8字节进行异或的结果，长度仍为8字节
 *63 Cx:认证失败，X表示还允许重试的次数
 *65 81:写EEPROM失败
 *67 00:长度错误
 *69 01:状态无效
 *69 82:不满足安全条件
 *69 83:认证方法锁定
 *69 85:使用条件不满足
 *6A 82:Key文件未找到
 *94 03:密钥未找到
 *
**/
int esam_external_authentication(char type,char p2,
	char*dat,char *out,unsigned short limit_len)
{
	unsigned short sw1sw2;
	unsigned char cmd[5+0x08];
	char ack[50];
	unsigned short ack_len;

	cmd[0] = 0x00;
	cmd[1] = 0x82;
	cmd[2] = 0x00;
	cmd[3] = p2;
	cmd[4] = 0x08;
	memcpy(&cmd[5],dat,0x08);
	sw1sw2 = esam_cmd_parse((char*)cmd, 5+0x08, ack, &ack_len);
	ESAM_LOG_ARRAY(ESAM_LOG_OUT_DEFAULT_GRADE,"internal_authentication",ack_len,(char*)ack);
	if((1 == sw1sw2) && (ack_len >= 2))
	{
		sw1sw2 = (ack[ack_len-2]<<8)+ack[ack_len-1];
		if(sw1sw2 == 0x9000)
		{
			ESAM_SUCCEEND_RETURN(ack,ack_len,out,limit_len);
		}
		ESAM_STATUS_RETURN(ack,ack_len,out,limit_len);
	}
	return ESAM_ERR_OPS_FLAG;   ///FALSE
}


void esam_des_calculate(char In[8], char key[8], 
	char Type,char out[8])
{
	des_cryption(Type,In,key,out);
}

/**
 * pin    : CLA INS P1 P2 Lc
 * length : pin length
 * initv  : random 
 * key    : 
 * mac    : out mac
 * 说明 : esam_des_calculate_mac(CmdPack,5,Rd,key,MAC);
**/
int esam_des_calculate_mac(unsigned char *pin, unsigned short length, 
	unsigned char initv[8],char key[16], unsigned char mac[4])
{
	unsigned char count, n, k;
	char iv[8];
	char buf[8];
	char tdes_calculate_buf[8];

	memcpy(iv, initv, 8);
	memset(buf, 0, 8);
	n = length & 0x07;
	count = length >> 3;
	if(n == 0)
	{
		buf[0] = 0x80;
	}
	else
	{
		memcpy(buf, pin + count * 8, n);
		buf[n] = 0x80;
		n = n + 1;
		while(n < 8)
		{
			buf[n] = 0;
			n = n + 1;
		}
	}

	for(n = 0; n < count; n++)
	{
		for(k = 0; k < 8; k++)
		{
			iv[k] = iv[k] ^ pin[n * 8 + k];
		}
		esam_des_calculate(iv, key, SL_DES_ENCRYPTION,tdes_calculate_buf);
		memcpy(iv, tdes_calculate_buf, 8);
	}
	for(k = 0; k < 8; k++)
	{
		iv[k] = iv[k] ^ buf[k];
	}
	esam_des_calculate(iv, key, SL_DES_ENCRYPTION,tdes_calculate_buf);
	memcpy(iv, tdes_calculate_buf, 8);
	esam_des_calculate(iv, key + 8, SL_DES_DECRYPTION,tdes_calculate_buf);
	memcpy(iv, tdes_calculate_buf, 8);
	esam_des_calculate(iv, key, SL_DES_ENCRYPTION,tdes_calculate_buf);
	memcpy(iv, tdes_calculate_buf, 8);
	memcpy(mac, iv, 4);  

	return 0;
}

/***********************  END OF FILES  ***********************/

