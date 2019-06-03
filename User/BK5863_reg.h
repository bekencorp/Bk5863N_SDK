#ifndef _BK5863_CHIP_H_
#define _BK5863_CHIP_H_


/////////////////////////////////////////////////////////////////////////////
//memory layout
/////////////////////////////////////////////////////////////////////////////
#define ROM_START_ADDR					0x00000000
#define RAM_START_ADDR					0x00400000

#define BK5863_MFC_BASE_ADDR			0x00800000
#define BK5863_READER_BASE_ADDR			0x00900000
#define BK5863_XVR_BASE_ADDR			0x00a00000
#define BK5863_ADC_BASE_ADDR			0x00a01000
#define BK5863_DAC_BASE_ADDR			0x00a02000
#define BK5863_ICU_REG_BASE_ADDR		0x00a03000
#define BK5863_TDES_BASE_ADDR			0x00a04000
#define BK5863_MODEM_BASE_ADDR			0x00a05000
#define BK5863_WDT_BASE_ADDR			0x00a06000
#define BK5863_GPIO_BASE_ADDR			0x00a07000
#define BK5863_UART1_BASE_ADDR			0x00a08000
#define BK5863_UART2_BASE_ADDR			0x00a09000
#define BK5863_SPI1_BASE_ADDR			0x00a0a000 /////
#define BK5863_SPI2_BASE_ADDR			0x00a0b000
#define BK5863_SPI3_BASE_ADDR			0x00a0c000
#define BK5863_SPI4_BASE_ADDR			0x00a0d000
#define BK5863_I2C1_BASE_ADDR			0x00a0e000
#define BK5863_I2C2_BASE_ADDR			0x00a0f000
#define BK5863_TIME_BASE_ADDR			0x00a10000

//ADC
#define ADC_CFG         							(*((volatile unsigned long *)(BK5863_ADC_BASE_ADDR+0x00)))
#define ADC_DAT         							(*((volatile unsigned long *)(BK5863_ADC_BASE_ADDR+0x04)))


///ICU register
#define BK5863_ICU_REG0_SYSCLK_CONFIG				(*((volatile unsigned long *)(BK5863_ICU_REG_BASE_ADDR+0x00)))
#define BK5863_ICU_REG1_CPUCLK_CONFIG				(*((volatile unsigned long *)(BK5863_ICU_REG_BASE_ADDR+0x04)))

#define ADAPTER_CLK_ACTIVE							0
#define ADAPTER_CLK_INACTIVE						1
//hop 4 bytes
#define BK5863_ICU_REG3_READERCLK_CONFIG			(*((volatile unsigned long *)(BK5863_ICU_REG_BASE_ADDR+0x0c)))
#define BK5863_ICU_REG4_ADCLK_CONFIG				(*((volatile unsigned long *)(BK5863_ICU_REG_BASE_ADDR+0x10)))
#define BK5863_ICU_REG5_DACCLK_CONFIG				(*((volatile unsigned long *)(BK5863_ICU_REG_BASE_ADDR+0x14)))
#define BK5863_ICU_REG6_3DESCLK_CONFIG				(*((volatile unsigned long *)(BK5863_ICU_REG_BASE_ADDR+0x18)))
#define BK5863_ICU_REG7_MODEMCLK_CONFIG				(*((volatile unsigned long *)(BK5863_ICU_REG_BASE_ADDR+0x1c)))
#define BK5863_ICU_REG8_WDTCLK_CONFIG				(*((volatile unsigned long *)(BK5863_ICU_REG_BASE_ADDR+0x20)))
#define BK5863_ICU_REG9_UART1CLK_CONFIG				(*((volatile unsigned long *)(BK5863_ICU_REG_BASE_ADDR+0x24)))
#define BK5863_ICU_REGA_UART2CLK_CONFIG				(*((volatile unsigned long *)(BK5863_ICU_REG_BASE_ADDR+0x28)))
#define BK5863_ICU_REGB_SPI1CLK_CONFIG				(*((volatile unsigned long *)(BK5863_ICU_REG_BASE_ADDR+0x2c)))
#define BK5863_ICU_REGC_SPI2CLK_CONFIG				(*((volatile unsigned long *)(BK5863_ICU_REG_BASE_ADDR+0x30)))
#define BK5863_ICU_REGD_SPI3CLK_CONFIG				(*((volatile unsigned long *)(BK5863_ICU_REG_BASE_ADDR+0x34)))
#define BK5863_ICU_REGE_SPI4CLK_CONFIG				(*((volatile unsigned long *)(BK5863_ICU_REG_BASE_ADDR+0x38)))
#define BK5863_ICU_REGF_I2C1CLK_CONFIG				(*((volatile unsigned long *)(BK5863_ICU_REG_BASE_ADDR+0x3c)))
#define BK5863_ICU_REG10_I2C2CLK_CONFIG				(*((volatile unsigned long *)(BK5863_ICU_REG_BASE_ADDR+0x40)))
#define BK5863_ICU_REG11_PWMCLK_CONFIG				(*((volatile unsigned long *)(BK5863_ICU_REG_BASE_ADDR+0x44)))
#define BK5863_ICU_REG12_GPIO7CLK_CONFIG			(*((volatile unsigned long *)(BK5863_ICU_REG_BASE_ADDR+0x48)))

//hop some bytes
#define READER_INT_bit									0
#define ADC_INT_bit										1
#define DAC_INT_bit										2
#define DES_INT_bit										3
#define MODEM_INT_bit									4
#define GPIO_INT_bit									5
#define UART1_INT_bit									6
#define UART2_INT_bit									7
#define SPI1_INT_bit									8
#define SPI2_INT_bit									9
#define SPI3_INT_bit									10
#define SPI4_INT_bit									11
#define I2C1_INT_bit									12
#define I2C2_INT_bit									13
#define PWM0_INT_bit									14
#define PWM1_INT_bit									15
#define PWM2_INT_bit									16
#define PWM3_INT_bit									17
#define PWM4_INT_bit									18
#define DETOUTBO_INT_bit								19
#define DETOUTAO_INT_bit								20
#define BK5863_ICU_REG20_INTEN_CONFIG					(*((volatile unsigned long *)(BK5863_ICU_REG_BASE_ADDR+0x80)))

#define FIQ_INT_ENABLE_bit								1
#define IRQ_INT_ENABLE_bit								0
#define BK5863_ICU_REG21_FIRQEN_CONFIG					(*((volatile unsigned long *)(BK5863_ICU_REG_BASE_ADDR+0x84)))
#define BK5863_ICU_REG22_INTWAKE_CONFIG					(*((volatile unsigned long *)(BK5863_ICU_REG_BASE_ADDR+0x88)))
#define BK5863_ICU_REG23_INT_CLEAR						(*((volatile unsigned long *)(BK5863_ICU_REG_BASE_ADDR+0x8c)))
#define BK5863_ICU_REG24_INT_STATUS						(*((volatile unsigned long *)(BK5863_ICU_REG_BASE_ADDR+0x90)))
#define BK5863_ICU_REG30_CTRL_ANALOG0					(*((volatile unsigned long *)(BK5863_ICU_REG_BASE_ADDR+0xc0)))
#define BK5863_ICU_REG31_CTRL_ANALOG1					(*((volatile unsigned long *)(BK5863_ICU_REG_BASE_ADDR+0xc4)))

#define PWD_CARDREAD											BIT(18)		//Card reader power down
#define PWD_CR_SPI_RX											BIT(17)		//Card reader spi rx power down
#define PWD_CR_SPI_TX											BIT(16)		//Card reader spi tx power down
#define BK5863_ICU_REG32_CTRL_ANALOG2					(*((volatile unsigned long *)(BK5863_ICU_REG_BASE_ADDR+0xc8)))
#define BK5863_ICU_REG33_CTRL_ANALOG3					(*((volatile unsigned long *)(BK5863_ICU_REG_BASE_ADDR+0xcc)))
#define BK5863_ICU_REG34_FLASH_CONFIG					(*((volatile unsigned long *)(BK5863_ICU_REG_BASE_ADDR+0xd0)))


#define REG_APB1_XVR_IDX_00H     (*((volatile unsigned long *)   (BK5863_XVR_BASE_ADDR+0x00)))
#define REG_APB1_XVR_IDX_01H     (*((volatile unsigned long *)   (BK5863_XVR_BASE_ADDR+0x04)))
#define REG_APB1_XVR_IDX_02H     (*((volatile unsigned long *)   (BK5863_XVR_BASE_ADDR+0x08)))
#define REG_APB1_XVR_IDX_03H     (*((volatile unsigned long *)   (BK5863_XVR_BASE_ADDR+0x0C)))
#define REG_APB1_XVR_IDX_04H     (*((volatile unsigned long *)   (BK5863_XVR_BASE_ADDR+0x10)))
#define REG_APB1_XVR_IDX_05H     (*((volatile unsigned long *)   (BK5863_XVR_BASE_ADDR+0x14)))
#define REG_APB1_XVR_IDX_06H     (*((volatile unsigned long *)   (BK5863_XVR_BASE_ADDR+0x18)))
#define REG_APB1_XVR_IDX_07H     (*((volatile unsigned long *)   (BK5863_XVR_BASE_ADDR+0x1C)))
#define REG_APB1_XVR_IDX_08H     (*((volatile unsigned long *)   (BK5863_XVR_BASE_ADDR+0x20)))
#define REG_APB1_XVR_IDX_09H     (*((volatile unsigned long *)   (BK5863_XVR_BASE_ADDR+0x24)))
#define REG_APB1_XVR_IDX_0AH     (*((volatile unsigned long *)   (BK5863_XVR_BASE_ADDR+0x28)))
#define REG_APB1_XVR_IDX_0BH     (*((volatile unsigned long *)   (BK5863_XVR_BASE_ADDR+0x2C)))
#define REG_APB1_XVR_IDX_0CH     (*((volatile unsigned long *)   (BK5863_XVR_BASE_ADDR+0x30)))
#define REG_APB1_XVR_IDX_0DH     (*((volatile unsigned long *)   (BK5863_XVR_BASE_ADDR+0x34)))
#define REG_APB1_XVR_IDX_0EH     (*((volatile unsigned long *)   (BK5863_XVR_BASE_ADDR+0x38)))
#define REG_APB1_XVR_IDX_0FH     (*((volatile unsigned long *)   (BK5863_XVR_BASE_ADDR+0x3C)))
#define REG_APB1_XVR_IDX_10H     (*((volatile unsigned long *)   (BK5863_XVR_BASE_ADDR+0x40)))
#define REG_APB1_XVR_IDX_11H     (*((volatile unsigned long *)   (BK5863_XVR_BASE_ADDR+0x44)))
#define REG_APB1_XVR_IDX_12H     (*((volatile unsigned long *)   (BK5863_XVR_BASE_ADDR+0x48)))
#define REG_APB1_XVR_IDX_13H     (*((volatile unsigned long *)   (BK5863_XVR_BASE_ADDR+0x4C)))
#define REG_APB1_XVR_IDX_14H     (*((volatile unsigned long *)   (BK5863_XVR_BASE_ADDR+0x50)))
#define REG_APB1_XVR_IDX_15H     (*((volatile unsigned long *)   (BK5863_XVR_BASE_ADDR+0x54)))
#define REG_APB1_XVR_IDX_16H     (*((volatile unsigned long *)   (BK5863_XVR_BASE_ADDR+0x58)))
#define REG_APB1_XVR_IDX_17H     (*((volatile unsigned long *)   (BK5863_XVR_BASE_ADDR+0x5C)))
#define REG_APB1_XVR_IDX_18H     (*((volatile unsigned long *)   (BK5863_XVR_BASE_ADDR+0x60)))
#define REG_APB1_XVR_IDX_19H     (*((volatile unsigned long *)   (BK5863_XVR_BASE_ADDR+0x64)))
#define REG_APB1_XVR_IDX_1AH     (*((volatile unsigned long *)   (BK5863_XVR_BASE_ADDR+0x68)))
#define REG_APB1_XVR_IDX_1BH     (*((volatile unsigned long *)   (BK5863_XVR_BASE_ADDR+0x6C)))
#define REG_APB1_XVR_IDX_1CH     (*((volatile unsigned long *)   (BK5863_XVR_BASE_ADDR+0x70)))
#define REG_APB1_XVR_IDX_1DH     (*((volatile unsigned long *)   (BK5863_XVR_BASE_ADDR+0x74)))
#define REG_APB1_XVR_IDX_1EH     (*((volatile unsigned long *)   (BK5863_XVR_BASE_ADDR+0x78)))
#define REG_APB1_XVR_IDX_1FH     (*((volatile unsigned long *)   (BK5863_XVR_BASE_ADDR+0x7C)))
#define REG_APB1_XVR_REG_24H     (*((volatile unsigned long *)   (BK5863_XVR_BASE_ADDR+0x90)))
#define REG_APB1_XVR_REG_25H     (*((volatile unsigned long *)   (BK5863_XVR_BASE_ADDR+0x94)))
#define REG_APB1_XVR_REG_2AH     (*((volatile unsigned long *)   (BK5863_XVR_BASE_ADDR+0xA8)))
#define REG_APB1_XVR_REG_3BH     (*((volatile unsigned long *)   (BK5863_XVR_BASE_ADDR+0xEC)))


//GPIO register
#define GPIOA_CONFIG						(*((volatile unsigned long *)(BK5863_GPIO_BASE_ADDR+0x00)))
#define GPIOA_DATA							(*((volatile unsigned long *)(BK5863_GPIO_BASE_ADDR+0x04)))
#define GPIOB_CONFIG						(*((volatile unsigned long *)(BK5863_GPIO_BASE_ADDR+0x08)))
#define GPIOB_DATA							(*((volatile unsigned long *)(BK5863_GPIO_BASE_ADDR+0x0C)))
#define GPIOC_CONFIG						(*((volatile unsigned long *)(BK5863_GPIO_BASE_ADDR+0x10)))
#define GPIOC_DATA							(*((volatile unsigned long *)(BK5863_GPIO_BASE_ADDR+0x14)))
#define GPIOD_CONFIG						(*((volatile unsigned long *)(BK5863_GPIO_BASE_ADDR+0x18)))
#define GPIOD_DATA							(*((volatile unsigned long *)(BK5863_GPIO_BASE_ADDR+0x1C)))
#define GPIOE_CONFIG						(*((volatile unsigned long *)(BK5863_GPIO_BASE_ADDR+0x20)))
#define GPIOE_DATA							(*((volatile unsigned long *)(BK5863_GPIO_BASE_ADDR+0x24)))
#define GPIOF_CONFIG						(*((volatile unsigned long *)(BK5863_GPIO_BASE_ADDR+0x28)))
#define GPIOF_DATA							(*((volatile unsigned long *)(BK5863_GPIO_BASE_ADDR+0x2C)))
// hop  8 bytes 
#define GPIO_INT_LEVEL_CONFIG				(*((volatile unsigned long *)(BK5863_GPIO_BASE_ADDR+0x38)))
#define GPIO_INT_ENABLE_CONFIG				(*((volatile unsigned long *)(BK5863_GPIO_BASE_ADDR+0x3C)))
#define GPIO_INT_STATUS						(*((volatile unsigned long *)(BK5863_GPIO_BASE_ADDR+0x40)))
#define GPIO_INT_LEVEL_2_CONFIG				(*((volatile unsigned long *)(BK5863_GPIO_BASE_ADDR+0x44)))
#define GPIO_INT_ENABLE_2_CONFIG			(*((volatile unsigned long *)(BK5863_GPIO_BASE_ADDR+0x48)))
#define GPIO_INT_STATUS_2					(*((volatile unsigned long *)(BK5863_GPIO_BASE_ADDR+0x4C)))

#define GPIOD_UART1_TX_BIT					4
#define GPIOD_UART1_RX_BIT					5


//UART register
#define UART1_CFG							(*((volatile unsigned long *)(BK5863_UART1_BASE_ADDR+0x00)))
#define UART1_FIFO_THRESHOLD				(*((volatile unsigned long *)(BK5863_UART1_BASE_ADDR+0x04)))
#define UART1_FIFO_COUNT					(*((volatile unsigned long *)(BK5863_UART1_BASE_ADDR+0x08)))
#define UART1_DATA							(*((volatile unsigned long *)(BK5863_UART1_BASE_ADDR+0x0C)))
#define UART1_INT_EN						(*((volatile unsigned long *)(BK5863_UART1_BASE_ADDR+0x10)))
#define UART1_INT_FLAG						(*((volatile unsigned long *)(BK5863_UART1_BASE_ADDR+0x14)))
#define UART1_WAKE_CTRL					(*((volatile unsigned long *)(BK5863_UART1_BASE_ADDR+0x1C)))

#define UART2_CFG							(*((volatile unsigned long *)(BK5863_UART2_BASE_ADDR+0x00)))
#define UART2_FIFO_THRESHOLD				(*((volatile unsigned long *)(BK5863_UART2_BASE_ADDR+0x04)))
#define UART2_FIFO_COUNT					(*((volatile unsigned long *)(BK5863_UART2_BASE_ADDR+0x08)))
#define UART2_DATA							(*((volatile unsigned long *)(BK5863_UART2_BASE_ADDR+0x0C)))
#define UART2_INT_EN						(*((volatile unsigned long *)(BK5863_UART2_BASE_ADDR+0x10)))
#define UART2_INT_FLAG						(*((volatile unsigned long *)(BK5863_UART2_BASE_ADDR+0x14)))
#define UART2_WAKE_CTRL					(*((volatile unsigned long *)(BK5863_UART2_BASE_ADDR+0x1C)))


#define UART_TX_EN_BIT						0
#define UART_RX_EN_BIT						1
#define UART_IRDA_MODE_EN_BIT			2
#define UART_DATA_LEN_BIT					3
#define UART_PAR_EN_BIT						5
#define UART_PAR_MODE_BIT					6
#define UART_STOP_LEN_BIT					7
#define UART_CLK_DIV_BIT					8
#define UART_TX_FIFO_RESET_BIT				21
#define UART_RX_FIFO_RESET_BIT				22

//#define UART_TX_INT_EN			 		UART1_INT_EN |= 0x01
//#define UART_RX_INT_EN			 		UART1_INT_EN |= 0x02
//#define UART_TX_INT_DISENABLE	 		UART1_INT_EN &= ~0x01
//#define UART_RX_INT_DISENABLE	 		UART1_INT_EN &= ~0x02

#define UART_TX_INT_BIT						0
#define UART_RX_INT_BIT						1

//SPI1
#define  SPI1_CTRL                                          (*((volatile unsigned long *)(BK5863_SPI1_BASE_ADDR+0x00)))
#define	 SPI_BUSY									BIT(15)
#define	 SPI_RX_OVER							BIT(12)
#define  SPI_TX_OVER							BIT(11)
#define  SPI_INT_RX								BIT(9)
#define  SPI_INT_TX								BIT(8)
#define  SPI1_STATUS                                     (*((volatile unsigned long *)(BK5863_SPI1_BASE_ADDR+0x04)))
#define  SPI1_DATA                                          (*((volatile unsigned long *)(BK5863_SPI1_BASE_ADDR+0x08)))




//#define   BK5863_WRITE_REG(reg_addr,reg_v)		(*((volatile unsigned long *)(reg_addr)))=reg_v
//#define   BK5863_READ_REG(reg_addr)				(*((volatile unsigned long *)   (reg_addr)))
#define REG_READ(addr)          *((volatile UINT32 *)(addr))
#define REG_WRITE(addr, _data) 	(*((volatile UINT32 *)(addr)) = (_data))


//CARD READER
#define PCD_BASE_ADDR 0x00900000
//#define WriteRawRC(reg_addr,reg_v)    (*((volatile unsigned long *)   (PCD_BASE_ADDR + reg_addr*4))) = (reg_v)
//#define ReadRawRC(reg_addr)    *((volatile unsigned long *)(PCD_BASE_ADDR + reg_addr*4))

#endif
