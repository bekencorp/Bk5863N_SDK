#include "isoiec7816.h"
#include "isoiec7816_intf.h"

_isoiec7816_status_t _isoiec7816 = {0};

void isoiec7816_hardware_active(void)
{
	
}

void isoiec7816_hardware_deactivation(void)
{
	
}

void isoiec7816_hardware_init(void)
{
	isoiec7816_hardware_intf_init();
}

void isoiec7816_hardware_deinit(void)
{
	isoiec7816_hardware_intf_deinit();
}

int isoiec7816_hardware_open(int ops,char *buf,int len)
{
	if(!_isoiec7816.op_st)
	{
		isoiec7816_hardware_init();
		_isoiec7816.op_st = 1;
	}
	
	if(!_isoiec7816.active)
	{
		isoiec7816_hardware_active();
		_isoiec7816.active = 1;
	}

	return 0;
}

int isoiec7816_hardware_close(int ops,char *buf,int len)
{
	if(_isoiec7816.op_st)
	{
		isoiec7816_hardware_deinit();
		_isoiec7816.op_st = 0;
	}
	
	if(_isoiec7816.active)
	{
		isoiec7816_hardware_deactivation();
		_isoiec7816.active = 0;
	}
	
	return 0;
}


int isoiec7816_hardware_write(int ops,char *buf,int len)
{
	return isoiec7816_hardware_intf_send((const unsigned char *)buf,len&0x0FFFF);
}

int isoiec7816_hardware_read(int ops,char *buf,int expect_len,unsigned char *rec_len)
{
	if(!buf) return -1;
	return isoiec7816_hardware_intf_receive((unsigned char *)buf,expect_len&0xFF,rec_len);
}

int isoiec7816_hardware_control(_iosiec7816_ctl_t ops,void *arg)
{
	unsigned int param;
	
	switch(ops)
	{
		case IOSIEC7816_CTL_RESET:
			isoiec7816_hardware_intf_cold_reset();
			break;
		case IOSIEC7816_CTL_W_RESET:
			isoiec7816_hardware_intf_reset();
			break;
		case IOSIEC7816_CTL_SET_RATE:
			param = *((unsigned int*)arg);
			isoiec7816_io_rate_set(param);
			break;
		default:
			break;
	}
	return 0;
}

/***********************  END OF FILES  ***********************/
