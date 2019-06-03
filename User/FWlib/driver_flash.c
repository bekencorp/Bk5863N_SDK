#include <string.h>
#include "Includes.h"

static uint32 flash_mid = 0;
static uint32 flash_crc_idx = 0;

void set_flash_clk(unsigned char clk_conf) {
    unsigned int temp0;
    temp0 = reg_FLASH_CONF;
    reg_FLASH_CONF = (  (clk_conf << BIT_FLASH_CLK_CONF)
                      | (temp0    &  SET_MODE_SEL)
                      | (temp0    &  SET_FWREN_FLASH_CPU)
                      | (temp0    &  SET_WRSR_DATA)
                      | (temp0    &  SET_CRC_EN));

	/* Flash read data operation after setting clock */
	while(reg_FLASH_OPERATE_SW & 0x80000000);
    temp0 = reg_FLASH_OPERATE_SW;
    reg_FLASH_OPERATE_SW = (  (0              << BIT_ADDRESS_SW)
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

void set_flash_qe(void) {
    unsigned int temp0;

    while(reg_FLASH_OPERATE_SW & 0x80000000){}
    //WRSR QE=1
    temp0 = reg_FLASH_CONF; //����WRSR Status data
    if(flash_mid == 0xC22314)   ////MXIC
    {
         reg_FLASH_SR_DATA_CRC_CNT = (0xa5 << 22);            
    }
    if(READ_CHIPID == 0x326283) // chipid = '3262S'
    {
        reg_FLASH_CONF = (  (temp0 &  SET_FLASH_CLK_CONF)
                  | (temp0 &  SET_MODE_SEL)
                  | (temp0 &  SET_FWREN_FLASH_CPU)
#if (FLASH_4MBIT == 1)
                  | (0x0800 << BIT_WRSR_DATA) // SET QE=1  don't protect
#else
                  | (0x087c << BIT_WRSR_DATA) // SET QE=1, set flash protect all
#endif
                  | (temp0 &  SET_CRC_EN));

    }
    else
    {
    reg_FLASH_CONF = (  (temp0 &  SET_FLASH_CLK_CONF)
                      | (temp0 &  SET_MODE_SEL)
                      | (temp0 &  SET_FWREN_FLASH_CPU)
#if (FLASH_4MBIT == 1)
                      | (0x200 << BIT_WRSR_DATA) // SET QE=1  don't protect
#else
                      | (0x234 << BIT_WRSR_DATA) // SET QE=1, set flash protect all
#endif
                      | (temp0 &  SET_CRC_EN));


    }
    //Start WRSR
    temp0 = reg_FLASH_OPERATE_SW;
    reg_FLASH_OPERATE_SW = (  (temp0              &  SET_ADDRESS_SW)
                            | (FLASH_OPCODE_WRSR2 << BIT_OP_TYPE_SW)
                            | (0x1                << BIT_OP_SW)
                            | (0x1                << BIT_WP_VALUE)); // make WP equal 1 not protect SRP
    while(reg_FLASH_OPERATE_SW & 0x80000000);
}
/* protect part or whole */
void set_flash_protect(uint8 all) {
    unsigned int temp0;
   
    uint8 bit_QE = 0;
#if(DEFAULT_LINE_MODE == FLASH_LINE_4)
    bit_QE = 1;
#endif
    while(reg_FLASH_OPERATE_SW & 0x80000000){}
   
    temp0 = reg_FLASH_CONF; //����WRSR Status data
    temp0 &= 0xfffe0fff;  // set [BP4:BP0] = 0
    if(((flash_mid & 0x00ff0000) == 0x00BA0000) || flash_mid == 0xC22014 ) // ZETTA
    {// ZETTA 
        if(all == 1)
        {
            reg_FLASH_CONF = (  (temp0 &  SET_FLASH_CLK_CONF)
            					| (0x1003c << BIT_WRSR_DATA));// protect all sectors for ZETTA
        }
        else
        {
            reg_FLASH_CONF = (  (temp0 &  SET_FLASH_CLK_CONF)
            					| (0x10028 << BIT_WRSR_DATA));//protect 0 -251 sectors
        }
        	//Start WRSR
    	temp0 = reg_FLASH_OPERATE_SW;
    	reg_FLASH_OPERATE_SW = (  (temp0   &  SET_ADDRESS_SW)
    								| (FLASH_OPCODE_WRSR << BIT_OP_TYPE_SW)
                            		| (0x1               << BIT_OP_SW)
                            		| (0x1               << BIT_WP_VALUE)); // make WP equal 1 not protect SRP
        while(reg_FLASH_OPERATE_SW & 0x80000000);
    }
    else if(flash_mid == 0xC22314) 
    {//MXIC 
    	/* Attention: TB(top/bottom) bit of MXIC flash is OTP !!! */
        if(READ_CHIPID == 0x326283)  //chipid = '3262S'
        {
            reg_FLASH_SR_DATA_CRC_CNT = (0xa5 << 22);            
            if(all == 1)
            {
                reg_FLASH_CONF = (  (temp0 &  SET_FLASH_CLK_CONF)
                                                 |(bit_QE  << 16)// QE(Quad Enable)
            					       | (0x1003c << BIT_WRSR_DATA));//protect 0 -251 sectors
            }
            else
            {
                reg_FLASH_CONF = (  (temp0 &  SET_FLASH_CLK_CONF)
                                                 |(bit_QE  << 16)// QE(Quad Enable)
            					       | (0x10000 << BIT_WRSR_DATA)); // only protect 0th-7th blocks for MXIC,so 8th-15th blks may be written;0x10850 QE = enable
            }
        }    
        else
        {
            if(all == 1)
            {
                reg_FLASH_CONF = (  (temp0 &  SET_FLASH_CLK_CONF)
            					    | (0x1003c << BIT_WRSR_DATA));////protect all
            }
            else
            {
                reg_FLASH_CONF = (  (temp0 &  SET_FLASH_CLK_CONF)
            					    | (0x10000 << BIT_WRSR_DATA));//protect none 
            } 
        }
       //Start WRSR
    	temp0 = reg_FLASH_OPERATE_SW;
    	reg_FLASH_OPERATE_SW = (  (temp0    &  SET_ADDRESS_SW)
    								| (FLASH_OPCODE_WRSR << BIT_OP_TYPE_SW)
                            		| (0x1               << BIT_OP_SW)
                            		| (0x1               << BIT_WP_VALUE)); // make WP equal 1 not protect SRP
        while(reg_FLASH_OPERATE_SW & 0x80000000);
    }
    else if(flash_mid == 0x5e4014) //xtx 8M flash
    {
        if(all == 1)
        {
            reg_FLASH_CONF = (  (temp0 &  SET_FLASH_CLK_CONF)
            					| (0x1001c << BIT_WRSR_DATA));
        }
    	 else
        {
            reg_FLASH_CONF = (  (temp0 &  SET_FLASH_CLK_CONF)
            					| (0x10000 << BIT_WRSR_DATA));
        }
        //Start WRSR
        temp0 = reg_FLASH_OPERATE_SW;
        reg_FLASH_OPERATE_SW = (  (temp0 &  SET_ADDRESS_SW)
                                         | (FLASH_OPCODE_WRSR << BIT_OP_TYPE_SW)
                                         | (0x1                << BIT_OP_SW)
                                         | (0x1                << BIT_WP_VALUE)); // make WP equal 1 not protect SRP
        while(reg_FLASH_OPERATE_SW & 0x80000000);    
    }
#if (FLASH_4MBIT == 1)
    else if(flash_mid == 0x1C3113) //xtx 4M flash
    {
        if(all == 1)
        {
            reg_FLASH_CONF = (  (temp0 &  SET_FLASH_CLK_CONF)
            					| (0x1003c << BIT_WRSR_DATA));
        }
    	 else
        {
            reg_FLASH_CONF = (  (temp0 &  SET_FLASH_CLK_CONF)
            					| (0x10034 << BIT_WRSR_DATA));//protect Lower 448KB
        }
        //Start WRSR
        temp0 = reg_FLASH_OPERATE_SW;
        reg_FLASH_OPERATE_SW = (  (temp0 &  SET_ADDRESS_SW)
                                         | (FLASH_OPCODE_WRSR << BIT_OP_TYPE_SW)
                                         | (0x1                << BIT_OP_SW)
                                         | (0x1                << BIT_WP_VALUE)); // make WP equal 1 not protect SRP
        while(reg_FLASH_OPERATE_SW & 0x80000000);    
    }
#endif
    else
    {//other flash, such as BOYA, Paragon, BG etc.
    	if(all == 1)
       {
            reg_FLASH_CONF = (  (temp0 &  SET_FLASH_CLK_CONF)
        					   |(bit_QE  << 19)// QE(Quad Enable)
            					   | (0x14000 << BIT_WRSR_DATA));//protect all sectors &
    	}
       else
       {
            reg_FLASH_CONF = (  (temp0 &  SET_FLASH_CLK_CONF)
        					   |(bit_QE  << 19)// QE(Quad Enable)
            					   | (0x1404c << BIT_WRSR_DATA));//protect address:0x00000-0xfbfff
       }
        //Start WRSR
        temp0 = reg_FLASH_OPERATE_SW;
        reg_FLASH_OPERATE_SW = (  (temp0 &  SET_ADDRESS_SW)
                                         | (FLASH_OPCODE_WRSR2 << BIT_OP_TYPE_SW)
                                         | (0x1                << BIT_OP_SW)
                                         | (0x1                << BIT_WP_VALUE)); // make WP equal 1 not protect SRP
        while(reg_FLASH_OPERATE_SW & 0x80000000);
    }

    while(reg_FLASH_OPERATE_SW & 0x80000000);	
}


#if(DEFAULT_LINE_MODE == FLASH_LINE_4)
static void set_flash_qwfr(void) {
    unsigned int temp0;
    //QWFR
    temp0 = reg_FLASH_CONF;
    reg_FLASH_CONF = (  (temp0 &  SET_FLASH_CLK_CONF)
                      | (0x2   << BIT_MODE_SEL)  //QWFR��������MODE use QWFR to fetch data  //for BG flash
                      | (temp0 &  SET_FWREN_FLASH_CPU)
                      | (temp0 &  SET_WRSR_DATA)
                      | (temp0 &  SET_CRC_EN));
}
#endif

void clr_flash_qwfr(void) {
    unsigned int temp0;
    //CRMR �ر�continuous���� �����mode_sel�еĵ���λģʽѡ��
    temp0 = reg_FLASH_CONF;
    reg_FLASH_CONF = (  (temp0 &  SET_FLASH_CLK_CONF)
                      | (0x1   << BIT_MODE_SEL)
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

void flash_erase_sector(uint32 address) {
    unsigned int temp0;
    u_int32 cpu_flags, mask;
    SYSirq_Disable_Interrupts_Save_Flags(&cpu_flags, &mask);
    set_flash_clk(FLASH_CLK_26mHz);
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
	flash_config();
	set_spr( SPR_VICTR(0), 0x00000000 );

    SYSirq_Interrupts_Restore_Flags(cpu_flags, mask);
}
#if 0
void flash_self_destroyed(void)
{
    unsigned int temp0;
    u_int32 cpu_flags, mask;
    SYSirq_Disable_Interrupts_Save_Flags(&cpu_flags, &mask);
	set_flash_clk(FLASH_CLK_26mHz);
    flash_set_line_mode(FLASH_LINE_2);

    /* Flash protect disable */
    while(reg_FLASH_OPERATE_SW & 0x80000000);
    temp0 = reg_FLASH_CONF; 
    temp0 &= 0xfffe0fff;    // set [BP4:BP0] = 0
    reg_FLASH_CONF = (  (temp0 &  SET_FLASH_CLK_CONF)
                               |(0x10000 << BIT_WRSR_DATA));     
    temp0 = reg_FLASH_OPERATE_SW;
    reg_FLASH_OPERATE_SW = (  (temp0 &  SET_ADDRESS_SW)
                                     | (FLASH_OPCODE_WRSR << BIT_OP_TYPE_SW)
                                     | (0x1                << BIT_OP_SW)
                                     | (0x1                << BIT_WP_VALUE)); 
    while(reg_FLASH_OPERATE_SW & 0x80000000); 
  
    /* Flash erase 0 sector */
    while(reg_FLASH_OPERATE_SW & 0x80000000);
    temp0 = reg_FLASH_OPERATE_SW;
    reg_FLASH_OPERATE_SW = (  (0         << BIT_ADDRESS_SW)
                            | (FLASH_OPCODE_SE << BIT_OP_TYPE_SW)
                            | (0x1             << BIT_OP_SW)
                            | (temp0           &  SET_WP_VALUE));
    while(reg_FLASH_OPERATE_SW & 0x80000000);

	set_spr( SPR_VICTR(0), 0x00000000 );

    SYSirq_Interrupts_Restore_Flags(cpu_flags, mask);
  
}
#endif
void flash_set_line_mode(uint8 mode) {
	uint32 temp0 = 0;
    switch(mode) {
    case FLASH_LINE_1:
        clr_flash_qwfr();
        break;
    case FLASH_LINE_2:
        clr_flash_qwfr();
        reg_FLASH_CONF &= (~(7<<BIT_MODE_SEL));
        reg_FLASH_CONF |= (1<<BIT_MODE_SEL);
        break;
    case FLASH_LINE_4:
#if(DEFAULT_LINE_MODE == FLASH_LINE_4)
        if(((flash_mid&0x00ff0000) == 0x00C20000)||((flash_mid&0x00ff0000) == 0x00BA0000)) //MXIC or ZETTA
        {
            if(READ_CHIPID == 0x326283) //chipid = '3262S'
            {   
                set_flash_qwfr();                  /**< 4�ߴ� */        
            }
            else
            {
                clr_flash_qwfr();
                reg_FLASH_CONF &= (~(7<<BIT_MODE_SEL));
                reg_FLASH_CONF |= (1<<BIT_MODE_SEL);
            }
        }
	 else
        {
            set_flash_qwfr();                  /**< 4�ߴ� */
        }
#else
        clr_flash_qwfr();
        reg_FLASH_CONF &= (~(7<<BIT_MODE_SEL));
        reg_FLASH_CONF |= (1<<BIT_MODE_SEL);
#endif
        break;
    default:
        break;
    }
    /* Flash read data operation after setting 4 line mode */
	while(reg_FLASH_OPERATE_SW & 0x80000000);
    temp0 = reg_FLASH_OPERATE_SW;
    reg_FLASH_OPERATE_SW = (  (0              << BIT_ADDRESS_SW)
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
uint32 flash_read_sr(void)
{
	unsigned int temp0;
    uint32 flash_id;

    while(reg_FLASH_OPERATE_SW & 0x80000000);
    temp0 = reg_FLASH_OPERATE_SW;
    reg_FLASH_OPERATE_SW = (  (temp0             &  SET_ADDRESS_SW)
                            | (FLASH_OPCODE_RDSR << BIT_OP_TYPE_SW)
                            | (0x1               << BIT_OP_SW)
                            | (temp0             &  SET_WP_VALUE));
    while(reg_FLASH_OPERATE_SW & 0x80000000);

    flash_id = reg_FLASH_SR_DATA_CRC_CNT;

    return (flash_id & 0x00ff);
}
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

    return (flash_id & 0x00ff);
}
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

uint32 flash_read_mID(void) {
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

    return (flash_id & 0xffffff);
}

void flash_init(void) 
{
    uint32 id1;
    flash_mid = flash_read_mID();
    Delay(50);
    id1 = flash_read_mID();
	
    if(flash_mid != id1)
    {
        BK3000_start_wdt(0xfff);
        while(1);
    }
    os_printf("FLASH mID: 0x%x\r\n", flash_mid);
    set_flash_protect(1);
    flash_config();
}

/**/
void flash_config(void)
{
    if(((flash_mid&0x00ff0000) == 0x00C20000)||((flash_mid&0x00ff0000) == 0x00BA0000)) //MXIC or ZETTA
    {
        if(READ_CHIPID == 0x326283) //chipid = '3262S'
        {   
            flash_set_line_mode(FLASH_LINE_4);
        }
        else
            flash_set_line_mode(FLASH_LINE_2);
    }
    else
        flash_set_line_mode(FLASH_LINE_4);
	
   set_flash_clk(FLASH_CLK_SEL);                /**< �ָ�flashʱ�� */
}
void flash_memcpy(uint8 *dst,uint8 *src,uint32 len,boolean first)
{
	uint32 i, reg_value;
    uint32 addr = (uint32 )src&(~0x1f);
    uint32 buf[8];
    uint8 *pb = (uint8 *)&buf[0];
    if (len == 0)
        return;
    if(first)
        flash_crc_idx = ((uint32)src) % 34;
	
    while(reg_FLASH_OPERATE_SW & 0x80000000);
    while (len) 
    {
        reg_value = reg_FLASH_OPERATE_SW;
        reg_FLASH_OPERATE_SW = (  (addr              << BIT_ADDRESS_SW)
                                | (FLASH_OPCODE_READ << BIT_OP_TYPE_SW)
                                | (0x1               << BIT_OP_SW)
                                | (reg_value         &  SET_WP_VALUE));
        while(reg_FLASH_OPERATE_SW & 0x80000000);
        addr+=32;

        for (i=0; i<8; i++)
            buf[i] = reg_FLASH_DATA_FLASH_SW;

        for (i=(uint32)src&0x1f; i<32; i++)
		{
			if(flash_crc_idx < 32)
			{
            	*dst++ = pb[i];
			}
			flash_crc_idx++;
			if(flash_crc_idx == 34)
				flash_crc_idx = 0;
            src++;
            len--;
            if (len == 0)
                break;
        }
    }
	
}
void flash_read_data (uint8 *buffer, uint32 address, uint32 len) {
    uint32 i, reg_value;
    uint32 addr = address&(~0x1f);
    uint32 buf[8];
    uint8 *pb = (uint8 *)&buf[0];

    if (len == 0)
        return;

    while(reg_FLASH_OPERATE_SW & 0x80000000);
    while (len) {
        reg_value = reg_FLASH_OPERATE_SW;
        reg_FLASH_OPERATE_SW = (  (addr              << BIT_ADDRESS_SW)
                                | (FLASH_OPCODE_READ << BIT_OP_TYPE_SW)
                                | (0x1               << BIT_OP_SW)
                                | (reg_value         &  SET_WP_VALUE));
        while(reg_FLASH_OPERATE_SW & 0x80000000);
        addr+=32;

        for (i=0; i<8; i++)
            buf[i] = reg_FLASH_DATA_FLASH_SW;

        for (i=(address&0x1f); i<32; i++) {
            *buffer++ = pb[i];
            address++;
            len--;
            if (len == 0)
                break;
        }
    }
}

void flash_write_data (uint8 *buffer, uint32 address, uint32 len) {
    uint32 i, reg_value;
    uint32 addr = address&(~0x1f);
    uint32 buf[8];
    uint8 *pb = (uint8 *)&buf[0];
    u_int32 cpu_flags, mask;
    SYSirq_Disable_Interrupts_Save_Flags(&cpu_flags, &mask);

    if (address&0x1f)
        flash_read_data(pb, addr, 32);
	set_flash_clk(FLASH_CLK_26mHz);
    flash_set_line_mode(FLASH_LINE_2);
    set_flash_protect(0);
	
    while(reg_FLASH_OPERATE_SW & 0x80000000);
    while (len) {
        for (i=(address&0x1f); i<32; i++) {
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
    flash_config();
	
    set_spr( SPR_VICTR(0), 0x00000000 );
    SYSirq_Interrupts_Restore_Flags(cpu_flags, mask);
}

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
    CLEAR_WDT;
    CLEAR_SLEEP_TICK;
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

    CLEAR_WDT;
    CLEAR_SLEEP_TICK;
}

#ifdef MEMORIZE_INTO_FLASH

void save_volume_task(void *arg)
{
 //   uint8 vol_type = ((uint32)arg) & 0xFF;

 //   app_write_info(vol_type, NULL, NULL, NULL);
}

#endif
