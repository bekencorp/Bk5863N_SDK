#include "Flash.h"
#include "BK5863.h"

typedef struct
{
	uint32 flash_id;
	FLASH_LINE_MODE  line_mode;
	uint8_t  bp_wrsr;
	uint8_t  protect_post;
	uint8_t  protect_mask;
	uint16_t protect_all;
	uint16_t protect_none;
	uint16_t protect_half;
	uint16_t unprotect_last_block;
	uint8_t  clk;
	int8     qe_bit_post;
	uint8_t  qe_bit;
	int8     cmp_post;
	uint32   flash_size;
} flash_config_t;

static const flash_config_t flash_config[] =
{
    /* flash_id  line_mode,  wrsr,post,mask, protect_all,protect_none,protect_half,unprotect_last_block,  clk ,    qe_bit_post,qe,  cmp,  flash_size */
////{0x00c22314, FLASH_LINE_2, 2,  2,   0x0F,0x0F|(0U<<8),0x00|(0U<<8),0x04|(0U<<8),   0x04|(0U<<8), FLASH_CLK_32MHz,  6,       1,  -1,   1024*1024}, ////MXIC
	{0x00c86413, FLASH_LINE_2, 1,  2,   0x07,0x07|(0U<<8),0x00|(0U<<8),0x06|(0U<<8),   0x01|(0U<<8), FLASH_CLK_32MHz, -1,       0,  -1,   512*1024}, //GD25WD40C
	{0x00514013, FLASH_LINE_2, 1,  2,   0x07,0x07|(0U<<8),0x00|(0U<<8),0x06|(0U<<8),   0x01|(0U<<8), FLASH_CLK_32MHz, -1,       0,  -1,   512*1024}, ///MD25D40D
	{0x00856013, FLASH_LINE_2, 2,  2,   0x1F,0x00|(1U<<8),0x1F|(1U<<8),0x03|(1U<<8),   0x16|(1U<<8), FLASH_CLK_64MHz, -1,       1,  14,   512*1024}, ///P25Q40H
	{0x00c84013, FLASH_LINE_2, 2,  2,   0x1F,0x1F|(0U<<8),0x00|(0U<<8),0x0B|(0U<<8),   0x16|(1U<<8), FLASH_CLK_32MHz,  9,       1,  14,   512*1024} ///GD25Q40C
////{0x00000000, FLASH_LINE_2, 2,  2,   0x1F,0x00|(1U<<8),0x1F|(1U<<8),0x03|(1U<<8),   0x16|(1U<<8), FLASH_CLK_32MHz, -1,       1,  -1,   512*1024}, ///
};

static UINT32 flash_id = 0;
static const flash_config_t *flash_current_config = NULL;

/**************************************************
FunctionName:FlashOperationAddress
Describe:
InputParameter:
OutputParameter:
Notice:
**************************************************/
void FlashOperationAddress(unsigned int addr)
{
	unsigned int temp;
	if(addr>0xFFFFFF) //<0:23>
		return;  
	
	temp = reg_FLASH_OPERATE_SW&0xFF000000;
	temp |= addr;
	reg_FLASH_OPERATE_SW = temp;
}
/**************************************************
FunctionName:ReadFlashOperationAddress
Describe:
InputParameter:
OutputParameter:
Notice:
**************************************************/
unsigned int ReadFlashOperationAddress(void)
{
	return (reg_FLASH_OPERATE_SW&0xFF000000);
}
/**************************************************
FunctionName:FlashOperationCommandType
Describe:
InputParameter:
OutputParameter:
Notice:
**************************************************/
void FlashOperationCommandType(unsigned char Cmd)
{
	unsigned int temp;
	if(Cmd>0x1F) //<24:28>
		return;  
	
	temp = reg_FLASH_OPERATE_SW&(~0x1F000000);
	temp |= (Cmd<<24);
	reg_FLASH_OPERATE_SW = temp;
}
/**************************************************
FunctionName:ReadFlashOperationCommandType
Describe:
InputParameter:
OutputParameter:
Notice:
**************************************************/
unsigned char ReadFlashOperationCommandType(void)
{
	return ((reg_FLASH_OPERATE_SW&(0x1F000000))>>24);//<24:28>
}
/**************************************************
FunctionName:ReadFlashStatusBusyStatus
Describe:
InputParameter:
OutputParameter:
Notice:
**************************************************/
unsigned char ReadFlashStatusBusyStatus(void)
{
	return ((reg_FLASH_OPERATE_SW>>31)&0x1);
}
/**************************************************
FunctionName:ReadSrDataFlash
Describe:
InputParameter:
OutputParameter:
Notice:
**************************************************/
unsigned char ReadSrDataFlash(void)
{
	return (reg_FLASH_SR_DATA_CRC_CNT&0xFF);
}

/**************************************************
FunctionName:
Describe:
InputParameter:
OutputParameter:
Notice:
**************************************************/
unsigned char ReadCrcErrCounter(void)
{
	return ((reg_FLASH_SR_DATA_CRC_CNT>>8)&0xFF);
}

/**************************************************
FunctionName:
Describe:
InputParameter:
OutputParameter:
Notice:
**************************************************/
unsigned char ReadDataFlashSwSel(void)
{
	return ((reg_FLASH_SR_DATA_CRC_CNT>>16)&0x07);
}

/**************************************************
FunctionName:
Describe:
InputParameter:
OutputParameter:
Notice:
**************************************************/
unsigned char ReadDataSwFlashSel(void)
{
	return ((reg_FLASH_SR_DATA_CRC_CNT>>19)&0x07);
}

/**************************************************
FunctionName:
Describe:
InputParameter:
OutputParameter:
Notice:
**************************************************/
unsigned int ReadFlashCONF(void)
{
	return (reg_FLASH_CONF&0x07FFFFFF);
}

/**************************************************
FunctionName:FlashCONFIG
Describe:
InputParameter:
OutputParameter:
Notice:
**************************************************/
void FlashCONFIG(FLASHPara_T *par)
{
	uint32_t temp= 0;

	temp |= (par->FlashClkConf&0x0f);
	temp |= ((par->ModeSel&0x1f)<<4);
	temp |= ((par->FwrenFlashCPU&0x01)<<9);
	temp |= ((par->CRC_EN&0x01)<<26);
	
  while(reg_FLASH_OPERATE_SW & 0x80000000);
	temp |= (reg_FLASH_CONF&(0XFFFF<<10));	
	reg_FLASH_CONF = temp;
}

/**************************************************
FunctionName:
Describe:
InputParameter:
OutputParameter:
Notice:
**************************************************/
void set_flash_clk(unsigned char clk_conf)
{
	unsigned int temp0;
	temp0 = reg_FLASH_CONF;
	reg_FLASH_CONF = ((clk_conf << BIT_FLASH_CLK_CONF)
					| (temp0    &  SET_MODE_SEL)
					| (temp0    &  SET_FWREN_FLASH_CPU)
					| (temp0    &  SET_WRSR_DATA)
					| (temp0    &  SET_CRC_EN));

	/* Flash read data operation after setting clock */
	while(reg_FLASH_OPERATE_SW & 0x80000000);

	temp0 = reg_FLASH_OPERATE_SW;
	reg_FLASH_OPERATE_SW = ((0              << BIT_ADDRESS_SW)
							| (FLASH_OPCODE_READ << BIT_OP_TYPE_SW)
							| (0x1               << BIT_OP_SW)
							| (temp0          &  SET_WP_VALUE));
	while(reg_FLASH_OPERATE_SW & 0x80000000);

	temp0 = reg_FLASH_DATA_SW_FLASH;
	temp0 = reg_FLASH_DATA_SW_FLASH;
	temp0 = reg_FLASH_DATA_SW_FLASH;
	temp0 = reg_FLASH_DATA_SW_FLASH;
	temp0 = reg_FLASH_DATA_SW_FLASH;
	temp0 = reg_FLASH_DATA_SW_FLASH;
	temp0 = reg_FLASH_DATA_SW_FLASH;
	temp0 = reg_FLASH_DATA_SW_FLASH;
}

/**************************************************
FunctionName:
Describe:
InputParameter:
OutputParameter:
Notice:
**************************************************/
void set_flash_qe(void)
{
	unsigned int temp0;

	if((flash_current_config == NULL)&&(flash_id != 0x00c22314))
		return;
	if((flash_current_config != NULL)&&(flash_current_config->qe_bit_post == -1)){
		///printf("no support line4\r\n");
		return;
	}
	while(reg_FLASH_OPERATE_SW & 0x80000000){}
	//WRSR QE=1
	temp0 = reg_FLASH_CONF; //??WRSR Status data
	if(flash_id == 0x00c22314){
		reg_FLASH_CONF = ((temp0 &  SET_FLASH_CLK_CONF)
						| (temp0 &  SET_MODE_SEL)
						| (temp0 &  SET_FWREN_FLASH_CPU)
						| (0x40 << BIT_WRSR_DATA) // SET QE=1  don't protect
						| (temp0 &  SET_CRC_EN));
		//Start WRSR
		temp0 = reg_FLASH_OPERATE_SW;
		reg_FLASH_OPERATE_SW = ((temp0              &  SET_ADDRESS_SW)
								| (FLASH_OPCODE_WRSR2 << BIT_OP_TYPE_SW)
								| (0x1                << BIT_OP_SW)
								| (0x1                << BIT_WP_VALUE)); // make WP equal 1 not protect SRP
		while(reg_FLASH_OPERATE_SW & 0x80000000);
	}
	else{
		//temp0 &= ~(SET_WRSR_DATA);
		//reg_FLASH_CONF = temp0 | ((flash_current_config->qe_bit << flash_current_config->qe_bit_post) << BIT_WRSR_DATA);
		reg_FLASH_CONF = ((temp0 &  SET_FLASH_CLK_CONF)
						| (temp0 &  SET_MODE_SEL)
						| (temp0 &  SET_FWREN_FLASH_CPU)
						| ((flash_current_config->qe_bit << flash_current_config->qe_bit_post) << BIT_WRSR_DATA) // SET QE=1  don't protect
						| (temp0 &  SET_CRC_EN));
		//Start WRSR
		temp0 = reg_FLASH_OPERATE_SW;
		if(flash_current_config->bp_wrsr == 2){
			reg_FLASH_OPERATE_SW = ((temp0              &  SET_ADDRESS_SW)
									| (FLASH_OPCODE_WRSR2 << BIT_OP_TYPE_SW)
									| (0x1                << BIT_OP_SW)
									| (0x1                << BIT_WP_VALUE)); // make WP equal 1 not protect SRP
		}
		else{
			reg_FLASH_OPERATE_SW = ((temp0              &  SET_ADDRESS_SW)
									| (FLASH_OPCODE_WRSR << BIT_OP_TYPE_SW)
									| (0x1                << BIT_OP_SW)
									| (0x1                << BIT_WP_VALUE)); // make WP equal 1 not protect SRP
		}
		while(reg_FLASH_OPERATE_SW & 0x80000000);
	}
}

/**************************************************
FunctionName:
Describe:
InputParameter:
OutputParameter:
Notice:
**************************************************/
void set_flash_protect(uint8 all) 
{
	unsigned int param, value, cmp;
	PROTECT_TYPE type = (PROTECT_TYPE)all;
	
	if(flash_current_config == NULL) {
	#if (FLASH_4MBIT != 1)
		unsigned int temp0;

		while(reg_FLASH_OPERATE_SW & 0x80000000){}
	 
		temp0 = reg_FLASH_CONF; //??WRSR Status data
		temp0 &= (~(SET_WRSR_DATA));  // set [BP4:BP0] = 0
		if(flash_id == 0x00c22314)   //MXIC
		{
			if(all == 1)
				reg_FLASH_CONF = ((temp0 &  SET_FLASH_CLK_CONF)
								| (0x1087C << BIT_WRSR_DATA));  // protect all sectors for ZETTA
			else if(all == 2)
				reg_FLASH_CONF = ((temp0 &  SET_FLASH_CLK_CONF)
								| (0x10850 << BIT_WRSR_DATA));
			else if(all == 0)
				reg_FLASH_CONF = ((temp0 &  SET_FLASH_CLK_CONF)
								| (0x10840 << BIT_WRSR_DATA)); // only 255th&256th sectors may be written for ZETTA
			else ///if(all == 3)
				reg_FLASH_CONF = ((temp0 &  SET_FLASH_CLK_CONF)
								| (0x10850 << BIT_WRSR_DATA));

			//Start WRSR
			temp0 = reg_FLASH_OPERATE_SW;
			reg_FLASH_OPERATE_SW = ((temp0                &  SET_ADDRESS_SW)
									| (FLASH_OPCODE_WRSR2  << BIT_OP_TYPE_SW)
									| (0x1                << BIT_OP_SW)
									| (0x1                << BIT_WP_VALUE)); // make WP equal 1 not protect SRP
			while(reg_FLASH_OPERATE_SW & 0x80000000);
		}
		else
		{
			if(all == 1)
				reg_FLASH_CONF = ((temp0 &  SET_FLASH_CLK_CONF)
								| (0x10040 << BIT_WRSR_DATA));  // protect all sectors for BOYA & GD
			else
				reg_FLASH_CONF = ((temp0 &  SET_FLASH_CLK_CONF)
								| (0x10050 << BIT_WRSR_DATA)); // only 256th sector may be written for BOYA & GD

			//Start WRSR
			temp0 = reg_FLASH_OPERATE_SW;
			reg_FLASH_OPERATE_SW = ((temp0              &  SET_ADDRESS_SW)
									 | (FLASH_OPCODE_WRSR2 << BIT_OP_TYPE_SW)
									 | (0x1                << BIT_OP_SW)
									 | (0x1                << BIT_WP_VALUE)); // make WP equal 1 not protect SRP
			while(reg_FLASH_OPERATE_SW & 0x80000000);
		}
//		value = get_flash_rdsr(2);
//		printf("read rdsr:%x,%x,%x\r\n",value,flash_read_sr2(),flash_read_sr());
	#endif
		 return;
	}

	switch (type)
	{
		case FLASH_PROTECT_NONE:
			param = flash_current_config->protect_none & 0xff;
			cmp = (flash_current_config->protect_none >> 8) & 0xff;
			break;
            
		case FLASH_PROTECT_ALL:
			param = flash_current_config->protect_all & 0xff;
			cmp = (flash_current_config->protect_all >> 8) & 0xff;
			break;

		case FLASH_PROTECT_HALF:
			param = flash_current_config->protect_half & 0xff;
			cmp = (flash_current_config->protect_half >> 8) & 0xff;
			break;

		case FLASH_UNPROTECT_LAST_BLOCK:
			param = flash_current_config->unprotect_last_block& 0xff;
			cmp = (flash_current_config->unprotect_last_block >> 8) & 0xff;
			break;
			
		default:
			param = flash_current_config->protect_all & 0xff;
			cmp = (flash_current_config->protect_all >> 8) & 0xff;
			break;
	}

	value = get_flash_rdsr(flash_current_config->bp_wrsr);
	///printf("[L%d]type:%D param:%x,cmp:%d\r\n",__LINE__,type,param,cmp);

	if(((param << flash_current_config->protect_post) != 
        (value & (flash_current_config->protect_mask << flash_current_config->protect_post)))
        || ((flash_current_config->cmp_post >= 0)
           &&((cmp << flash_current_config->cmp_post) != (value & (0x01 << flash_current_config->cmp_post)))))
	{
		value = (value & (~(flash_current_config->protect_mask << flash_current_config->protect_post))) 
				| (param << flash_current_config->protect_post);
		if(flash_current_config->cmp_post >= 0) {
			value &= ~(1 << flash_current_config->cmp_post);
			value |= ((cmp & 0x01) << flash_current_config->cmp_post);
		}
		////printf("--write status reg:%x,%x--\r\n", value, flash_current_config->bp_wrsr);
		set_flash_wrsr(flash_current_config->bp_wrsr, value);
	}
	///printf("[L%d]flash sr:%x\r\n",__LINE__,get_flash_rdsr(2));
}

/**************************************************
FunctionName:
Describe:
InputParameter:
OutputParameter:
Notice:
**************************************************/
static void set_flash_qwfr(void)
{
	unsigned int temp0;
	//QWFR
	temp0 = reg_FLASH_CONF;
	reg_FLASH_CONF = (  (temp0 &  SET_FLASH_CLK_CONF)
					| (0x2   << BIT_MODE_SEL)  //QWFR????MODE use QWFR to fetch data  //for BG flash
					| (temp0 &  SET_FWREN_FLASH_CPU)
					| (temp0 &  SET_WRSR_DATA)
					| (temp0 &  SET_CRC_EN));
}
/**************************************************
FunctionName:
Describe:
InputParameter:
OutputParameter:
Notice:
**************************************************/
void clr_flash_qwfr(void) 
{
    unsigned int temp0;
    //CRMR ??continuous?? ???mode_sel?????????
    temp0 = reg_FLASH_CONF;
    reg_FLASH_CONF = (  (temp0 &  SET_FLASH_CLK_CONF)
                      | (0x0   << BIT_MODE_SEL) //??mode_sel????????? ??????? //for BG flash
                      | (temp0 &  SET_FWREN_FLASH_CPU)
                      | (temp0 &  SET_WRSR_DATA)
                      | (temp0 &  SET_CRC_EN));

    temp0 = reg_FLASH_OPERATE_SW;
    reg_FLASH_OPERATE_SW = (  (0                 << BIT_ADDRESS_SW)
                            | (FLASH_OPCODE_CRMR << BIT_OP_TYPE_SW)
                            | (0x1               << BIT_OP_SW)
                            | (temp0             &  SET_WP_VALUE));
    while(reg_FLASH_OPERATE_SW & 0x80000000);
}
/**************************************************
FunctionName:
Describe:
InputParameter:
OutputParameter:
Notice:
**************************************************/
void flash_erase_sector(uint32 address) 
{
	unsigned int temp0;
	uint32 cpu_flags;

	cpu_flags = REG_APB3_ICU_IRQTYPE_EN;
	REG_APB3_ICU_IRQTYPE_EN &= (~(cpu_flags|0x03));

	set_flash_clk(FLASH_CLK_32MHz);
	flash_set_line_mode(FLASH_LINE_2);
	set_flash_protect(0);
	while(reg_FLASH_OPERATE_SW & 0x80000000);
	temp0 = reg_FLASH_OPERATE_SW;
	reg_FLASH_OPERATE_SW = (  (address         << BIT_ADDRESS_SW)
													| (FLASH_OPCODE_SE << BIT_OP_TYPE_SW)
													| (0x1             << BIT_OP_SW)
													| (temp0           &  SET_WP_VALUE));
	while(reg_FLASH_OPERATE_SW & 0x80000000);
	set_flash_protect(1);

	flash_set_line_mode(FLASH_LINE_2);
	set_flash_clk(FLASH_CLK_SEL);
	
	REG_APB3_ICU_IRQTYPE_EN = cpu_flags;
}

/**************************************************
FunctionName:
Describe:
InputParameter:
OutputParameter:
Notice:
**************************************************/
void flash_set_line_mode(FLASH_LINE_MODE mode)
{
	uint32 temp0 = 0;

	if((mode == FLASH_LINE_4)
		&&((flash_current_config != NULL)&&(flash_current_config->qe_bit_post == ((char)-1)))) {
		return;
	}
	if((mode == FLASH_LINE_4) 
		&&((flash_current_config == NULL)&&(flash_id != 0x00c22314))){
		return;
	}
	switch(mode) {
	    case FLASH_LINE_1:
	        clr_flash_qwfr();
	        break;
	    case FLASH_LINE_2:
	        clr_flash_qwfr();
	        reg_FLASH_CONF &= (~(SET_MODE_SEL));
	        reg_FLASH_CONF |= (1<<BIT_MODE_SEL);
	        break;
	    case FLASH_LINE_4:
		#if !CFG_DEFAULT_FLASH_LINE_OPEN
			set_flash_qe();
		#endif
	        set_flash_qwfr();                  /**< 4??? */
	        break;
	    default:
	        break;
	}
    /* Flash read data operation after setting 4 line mode */
	while(reg_FLASH_OPERATE_SW & 0x80000000);
	temp0 = reg_FLASH_OPERATE_SW;
	reg_FLASH_OPERATE_SW = ((0              << BIT_ADDRESS_SW)
							| (FLASH_OPCODE_READ << BIT_OP_TYPE_SW)
							| (0x1               << BIT_OP_SW)
							| (temp0          &  SET_WP_VALUE));
	while(reg_FLASH_OPERATE_SW & 0x80000000);
	temp0 = reg_FLASH_DATA_SW_FLASH;
	temp0 = reg_FLASH_DATA_SW_FLASH;
	temp0 = reg_FLASH_DATA_SW_FLASH;
	temp0 = reg_FLASH_DATA_SW_FLASH;
	temp0 = reg_FLASH_DATA_SW_FLASH;
	temp0 = reg_FLASH_DATA_SW_FLASH;
	temp0 = reg_FLASH_DATA_SW_FLASH;
	temp0 = reg_FLASH_DATA_SW_FLASH;
}

/**************************************************
FunctionName:
Describe:
InputParameter:
OutputParameter:
Notice:
**************************************************/
uint32 flash_read_sr(void)
{
	unsigned int temp0;
	uint32 flash_id;

	while(reg_FLASH_OPERATE_SW & 0x80000000);
	temp0 = reg_FLASH_OPERATE_SW;
	reg_FLASH_OPERATE_SW = ((temp0             &  SET_ADDRESS_SW)
							| (FLASH_OPCODE_RDSR << BIT_OP_TYPE_SW)
							| (0x1               << BIT_OP_SW)
							| (temp0             &  SET_WP_VALUE));
	while(reg_FLASH_OPERATE_SW & 0x80000000);

	flash_id = reg_FLASH_SR_DATA_CRC_CNT;

	return flash_id;
}
/**************************************************
FunctionName:
Describe:
InputParameter:
OutputParameter:
Notice:
**************************************************/
uint32 flash_read_sr2(void)
{
	unsigned int temp0;
    uint32 flash_id;

    while(reg_FLASH_OPERATE_SW & 0x80000000);
    temp0 = reg_FLASH_OPERATE_SW;
    reg_FLASH_OPERATE_SW = (  (temp0             &  SET_ADDRESS_SW)
                            | (FLASH_OPCODE_RDSR2 << BIT_OP_TYPE_SW)
                            | (0x1               << BIT_OP_SW)
                            | (temp0             &  SET_WP_VALUE));
    while(reg_FLASH_OPERATE_SW & 0x80000000);

    flash_id = reg_FLASH_SR_DATA_CRC_CNT;

    return flash_id;
}

/**************************************************
FunctionName:
Describe:
InputParameter:
OutputParameter:
Notice:
**************************************************/
void set_flash_ctrl_config(void)
{
	unsigned int temp0;
	unsigned int cfg = 0;
	cfg |= (flash_read_sr() & 0x000000ff);
	cfg |= ((flash_read_sr2() & 0x000000ff) << 8);
	temp0 = reg_FLASH_CONF;
	temp0 &= ~(0xffff << BIT_WRSR_DATA);  // set Flash Status Register = 0
	reg_FLASH_CONF = temp0 | (cfg << BIT_WRSR_DATA);
}
/**************************************************
FunctionName:
Describe:
InputParameter:
OutputParameter:
Notice:
**************************************************/
uint8 flash_read_mID(void) 
{
    unsigned int temp0;
    uint32 flash_id;

    while(reg_FLASH_OPERATE_SW & 0x80000000);
    temp0 = reg_FLASH_OPERATE_SW;
    reg_FLASH_OPERATE_SW = (  (temp0             &  SET_ADDRESS_SW)
                            | (FLASH_OPCODE_RDID << BIT_OP_TYPE_SW)
                            | (0x1               << BIT_OP_SW)
                            | (temp0             &  SET_WP_VALUE));
    while(reg_FLASH_OPERATE_SW & 0x80000000);

    flash_id = reg_FLASH_RDID_DATA_FLASH;

    return ( (flash_id >> 16) & 0xff);
}

uint32 flash_read_mID32(void) 
{
    unsigned int temp0;
    uint32 flash_id;

    while(reg_FLASH_OPERATE_SW & 0x80000000);
    temp0 = reg_FLASH_OPERATE_SW;
    reg_FLASH_OPERATE_SW = (  (temp0             &  SET_ADDRESS_SW)
                            | (FLASH_OPCODE_RDID << BIT_OP_TYPE_SW)
                            | (0x1               << BIT_OP_SW)
                            | (temp0             &  SET_WP_VALUE));
    while(reg_FLASH_OPERATE_SW & 0x80000000);

    flash_id = reg_FLASH_RDID_DATA_FLASH;

    return flash_id;
}
/**************************************************
FunctionName:
Describe:
InputParameter:
OutputParameter:
Notice:
**************************************************/
void flash_init(void) 
{
	uint32 tmp = 0;
	set_flash_clk(FLASH_CLK_32MHz);
//	  set_flash_clk(FLASH_CLK_64MHz);
	flash_set_line_mode(FLASH_LINE_1);
	tmp |= flash_read_sr();
	tmp |= (flash_read_sr2()<<8);
	
	flash_id = flash_read_mID32();
	if(flash_id == 0xC22314)   ////MXIC
	{
		reg_FLASH_SR_DATA_CRC_CNT = (0xa5 << 22);     
	}
	///printf("[L%d]flash sr:%x\r\n",__LINE__,get_flash_rdsr(1));
	flash_current_config = NULL;
	for(tmp=0;tmp<(sizeof(flash_config)/sizeof(flash_config_t));tmp++) {
		if(flash_config[tmp].flash_id == flash_id) {
			flash_current_config = &flash_config[tmp];
			break;
		}
	}
#if CFG_DEFAULT_FLASH_LINE_OPEN
	set_flash_qe();
#endif

#if ((CFG_DEFAULT_FLASH_PROTECT_MODE) == (FLASH_PROTECT_HALF))
	set_flash_protect(FLASH_PROTECT_HALF);
#elif ((CFG_DEFAULT_FLASH_PROTECT_MODE) == (FLASH_PROTECT_ALL))
	set_flash_protect(FLASH_PROTECT_ALL);
#elif ((CFG_DEFAULT_FLASH_PROTECT_MODE) == (FLASH_UNPROTECT_LAST_BLOCK))
	set_flash_protect(FLASH_UNPROTECT_LAST_BLOCK);
#elif ((CFG_DEFAULT_FLASH_PROTECT_MODE) == (FLASH_PROTECT_NONE))
	set_flash_protect(FLASH_PROTECT_NONE);
#else
	set_flash_protect(FLASH_PROTECT_ALL);
#endif
	if(flash_current_config == NULL){
		if(flash_id == 0xC22314) {
			flash_set_line_mode(FLASH_LINE_2);
		}
		return;
	}
	
	flash_set_line_mode(flash_current_config->line_mode);
	set_flash_clk(flash_current_config->clk);
	///printf("[L%d]flash sr:%x\r\n",__LINE__,get_flash_rdsr(1));
}

/**************************************************
FunctionName:
Describe:
InputParameter:
OutputParameter:
Notice:
**************************************************/
void flash_read_data(uint8 *buffer, uint32 address, uint32 len) 
{
    uint32 i, reg_value;
    uint32 addr = address&(~0x1f);
    uint32 buf[8];
    uint8 *pb = (uint8 *)&buf[0];

    if(len == 0)
			return;

    while(reg_FLASH_OPERATE_SW & 0x80000000);
    while(len) 
	{
		reg_value = reg_FLASH_OPERATE_SW;
		reg_FLASH_OPERATE_SW = ((addr              << BIT_ADDRESS_SW)
								| (FLASH_OPCODE_READ << BIT_OP_TYPE_SW)
								| (0x1               << BIT_OP_SW)
								| (reg_value         &  SET_WP_VALUE));
		while(reg_FLASH_OPERATE_SW & 0x80000000);
		addr += 32;

		for (i=0; i<8; i++)
			buf[i] = reg_FLASH_DATA_FLASH_SW;

		for (i=address%32; i<32; i++) 
		{
			*buffer++ = pb[i];
			address++;
			len--;
			if (len == 0)
				break;
		}
    }
}
/**************************************************
FunctionName:
Describe:
InputParameter:
OutputParameter:
Notice:
**************************************************/
void flash_write_data (uint8 *buffer, uint32 address, uint32 len) 
{
    uint32 i, reg_value;
    uint32 addr = address&(~0x1f);
    uint32 buf[8];
    uint8 *pb = (uint8 *)&buf[0];
    uint32 cpu_flags;
	
	cpu_flags = REG_APB3_ICU_IRQTYPE_EN;
	REG_APB3_ICU_IRQTYPE_EN &= (~(cpu_flags|0x03));

	if (address%32)
	flash_read_data(pb, addr, 32);
		set_flash_clk(FLASH_CLK_32MHz);
	flash_set_line_mode(FLASH_LINE_2);
	set_flash_protect(0);
	///printf("[L%d]flash sr:%x\r\n",__LINE__,get_flash_rdsr(1));
	while(reg_FLASH_OPERATE_SW & 0x80000000);
	while (len) 
	{
		for (i=address%32; i<32; i++) 
		{
			pb[i] = *buffer++;
			address++;
			len--;
			if (len == 0)
				break;
		}

		for (i=0; i<8; i++)
			reg_FLASH_DATA_SW_FLASH = buf[i];

		reg_value = reg_FLASH_OPERATE_SW;
		reg_FLASH_OPERATE_SW = (  (addr            << BIT_ADDRESS_SW)
								| (FLASH_OPCODE_PP << BIT_OP_TYPE_SW)
								| (0x1             << BIT_OP_SW)
								| (reg_value       &  SET_WP_VALUE));
		while(reg_FLASH_OPERATE_SW & 0x80000000);
		addr+=32;
		memset(pb, 0XFF, 32);
    }
    set_flash_protect(1);
	///printf("[L%d]flash sr:%x\r\n",__LINE__,get_flash_rdsr(1));

	flash_set_line_mode(FLASH_LINE_2);
    set_flash_clk(FLASH_CLK_SEL);

	REG_APB3_ICU_IRQTYPE_EN = cpu_flags;
}

/**************************************************
FunctionName:
Describe:
InputParameter:
OutputParameter:
Notice:
**************************************************/
void flash_crc_remove(uint32 address) {
	uint16 i, j, k;
	uint32 addrsrc = address&(~0x1f);
	uint32 addrstr = address&(~0x1f);
	uint8 buf[4352];
	uint8 *pbsrc = (uint8 *)&buf[0];
	uint8 *pbstr = (uint8 *)&buf[0];

	if (!(addrstr&0x0FFF))
	{
		flash_read_data(pbsrc, addrsrc, 4352);
		flash_erase_sector(addrstr);
	}
	for (i=0,j=0; j<4352; ) {
		for (k=0; k<32; k++,i++,j++)
			pbstr[i] = pbsrc[j];
		j+=2;
	}
	flash_write_data(pbstr, addrstr, 4096);
	addrsrc+=4352;
	addrstr+=4096;

	BK5863WDTClose();

	if (!(addrstr&0x0FFF))
	{
		flash_read_data(pbsrc, addrsrc, 4352);
		flash_erase_sector(addrstr);
	}
	for (i=0,j=0; j<4352; ) {
		for (k=0; k<32; k++,i++,j++)
			pbstr[i] = pbsrc[j];
		j+=2;
	}
	flash_write_data(pbstr, addrstr, 4096);
	BK5863WDTClose();
}

int get_flash_support_line_mode(FLASH_LINE_MODE linex)
{
	if((flash_current_config == NULL)&&(flash_id == 0x00c22314))
		return linex;
	if((flash_current_config != NULL)&&(flash_current_config->qe_bit_post == -1))
		return (linex == FLASH_LINE_4)? (-1):linex;
	
	return -1;
}

void flash_write_enable(void)
{
	UINT32 value;
	
	while(reg_FLASH_OPERATE_SW & 0x80000000);
	value = reg_FLASH_OPERATE_SW;
	reg_FLASH_OPERATE_SW = ((value 			    &  SET_ADDRESS_SW)
						   | (FLASH_OPCODE_WREN << BIT_OP_TYPE_SW)
						   | (0x1U				<< BIT_OP_SW)
						   | (0x1U              << BIT_WP_VALUE));
	while(reg_FLASH_OPERATE_SW & 0x80000000);
}

void flash_write_disable(void)
{
	UINT32 value;
	
	while(reg_FLASH_OPERATE_SW & 0x80000000);
	value = reg_FLASH_OPERATE_SW;
	reg_FLASH_OPERATE_SW = ((value 		    	&  SET_ADDRESS_SW)
						   | (FLASH_OPCODE_WRDI << BIT_OP_TYPE_SW)
						   | (0x1U				<< BIT_OP_SW)
						   | (0x1U              << BIT_WP_VALUE));
	while(reg_FLASH_OPERATE_SW & 0x80000000);
}


uint32 get_flash_rdsr(unsigned char size)
{
	unsigned int temp0;
	uint32 flash_id,rdsr;

	while(reg_FLASH_OPERATE_SW & 0x80000000);
	temp0 = reg_FLASH_OPERATE_SW;
	reg_FLASH_OPERATE_SW = ((temp0             &  SET_ADDRESS_SW)
							| (FLASH_OPCODE_RDSR << BIT_OP_TYPE_SW)
							| (0x1               << BIT_OP_SW)
							| (temp0             &  SET_WP_VALUE));
	while(reg_FLASH_OPERATE_SW & 0x80000000);

	flash_id = reg_FLASH_SR_DATA_CRC_CNT & 0x00FF;
	rdsr = flash_id&0xFF;
	
	if(size == 2)
	{
		while(reg_FLASH_OPERATE_SW & 0x80000000);
		temp0 = reg_FLASH_OPERATE_SW;
		reg_FLASH_OPERATE_SW = ((temp0             &  SET_ADDRESS_SW)
								| (FLASH_OPCODE_RDSR2 << BIT_OP_TYPE_SW)
								| (0x1               << BIT_OP_SW)
								| (temp0             &  SET_WP_VALUE));
		while(reg_FLASH_OPERATE_SW & 0x80000000);
		
		flash_id = reg_FLASH_SR_DATA_CRC_CNT & 0x00FF;
		rdsr = rdsr | (flash_id << 8);
	}
	
	return rdsr;
}

void set_flash_wrsr(unsigned char sr_width,  unsigned short val)
{
	uint32 value;

#if CFG_REDUCE_FLASH_WR_SR
	value = get_flash_rdsr(sr_width);
	if(value == val){
		return;
	}
#endif
	while(reg_FLASH_OPERATE_SW & 0x80000000UL);

	value = reg_FLASH_CONF;
	value &= ~(SET_WRSR_DATA);

	value |= (val << BIT_WRSR_DATA);
	value |= (0x1U      << BIT_CRC_EN);
	reg_FLASH_CONF = value;
	while(reg_FLASH_OPERATE_SW & 0x80000000UL);

	if(sr_width == 1)
	{
		while(reg_FLASH_OPERATE_SW & 0x80000000);
		value = reg_FLASH_OPERATE_SW;
		reg_FLASH_OPERATE_SW = ((value             &  SET_ADDRESS_SW)
								| (FLASH_OPCODE_WRSR << BIT_OP_TYPE_SW)
								| (0x1U              << BIT_OP_SW)
								| (0x1U              << BIT_WP_VALUE));
		while(reg_FLASH_OPERATE_SW & 0x80000000);
	}
	else if(sr_width == 2)
	{
		while(reg_FLASH_OPERATE_SW & 0x80000000);
		value = reg_FLASH_OPERATE_SW;
		reg_FLASH_OPERATE_SW = ((value             &  SET_ADDRESS_SW)
								| (FLASH_OPCODE_WRSR2 << BIT_OP_TYPE_SW)
								| (0x1U              << BIT_OP_SW)
								| (0x1U              << BIT_WP_VALUE));
		while(reg_FLASH_OPERATE_SW & 0x80000000);
	}
}

/**************************************************
FunctionName:
Describe:
InputParameter:
OutputParameter:
Notice:
**************************************************/
void flash_write_data2(uint8 *buffer, uint32 address, uint32 len) 
{
    uint32 i, reg_value;
    uint32 addr = address&(~0x1f);
    uint32 buf[8];
    uint8 *pb = (uint8 *)&buf[0];
    uint32 cpu_flags;
	
	cpu_flags = REG_APB3_ICU_IRQTYPE_EN;
	REG_APB3_ICU_IRQTYPE_EN &= (~(cpu_flags|0x03));

	if (address%32)
		flash_read_data(pb, addr, 32);
	set_flash_clk(FLASH_CLK_32MHz);
	flash_set_line_mode(FLASH_LINE_2);

	while(reg_FLASH_OPERATE_SW & 0x80000000);
	while (len) 
	{
		for (i=address%32; i<32; i++) 
		{
			pb[i] = *buffer++;
			address++;
			len--;
			if (len == 0)
				break;
		}

		for (i=0; i<8; i++)
			reg_FLASH_DATA_SW_FLASH = buf[i];

		reg_value = reg_FLASH_OPERATE_SW;
		reg_FLASH_OPERATE_SW = (  (addr            << BIT_ADDRESS_SW)
								| (FLASH_OPCODE_PP << BIT_OP_TYPE_SW)
								| (0x1             << BIT_OP_SW)
								| (reg_value       &  SET_WP_VALUE));
		while(reg_FLASH_OPERATE_SW & 0x80000000);
		addr+=32;
		memset(pb, 0XFF, 32);
    }

	flash_set_line_mode(FLASH_LINE_2);
    set_flash_clk(FLASH_CLK_SEL);

	REG_APB3_ICU_IRQTYPE_EN = cpu_flags;
}

/**************************************************
FunctionName:
Describe:
InputParameter:
OutputParameter:
Notice:
**************************************************/
void flash_erase_sector2(uint32 address) 
{
	unsigned int temp0;
	uint32 cpu_flags;

	cpu_flags = REG_APB3_ICU_IRQTYPE_EN;
	REG_APB3_ICU_IRQTYPE_EN &= (~(cpu_flags|0x03));

	set_flash_clk(FLASH_CLK_32MHz);
    flash_set_line_mode(FLASH_LINE_2);
	
    while(reg_FLASH_OPERATE_SW & 0x80000000);
    temp0 = reg_FLASH_OPERATE_SW;
    reg_FLASH_OPERATE_SW = (  (address         << BIT_ADDRESS_SW)
                            | (FLASH_OPCODE_SE << BIT_OP_TYPE_SW)
                            | (0x1             << BIT_OP_SW)
                            | (temp0           &  SET_WP_VALUE));
    while(reg_FLASH_OPERATE_SW & 0x80000000);
	
	flash_set_line_mode(FLASH_LINE_2);
	set_flash_clk(FLASH_CLK_SEL);
	
	REG_APB3_ICU_IRQTYPE_EN = cpu_flags;
}


/***********************  END OF FILES  ***********************/
