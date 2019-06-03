#ifndef _BK5863_REG_H_
#define _BK5863_REG_H_

//memory layout
#define ROM_START_ADDR                 0x00000000
#define RAM_START_ADDR                 0x00400000

#define AHB0_MFC_BASE                  0x00800000
#define AHB1_CDRDER_BASE               0x00900000
#define AHB2_AHB2APB_BASE              0x00a00000
#define APB1_ADC_BASE                  0x00a01000
#define APB2_DAC_BASE                  0x00a02000
#define APB3_ICU_BASE                  0x00a03000
#define APB4_DES_BASE                  0x00a04000
#define APB5_MODEMFIFO_BASE            0x00a05000
#define APB6_WDT_BASE                  0x00a06000
#define APB7_GPIO_BASE                 0x00a07000
#define APB8_UART1_BASE                0x00a08000
#define APB9_UART2_BASE                0x00a09000
#define APBA_SPI1_BASE                 0x00a0A000
#define APBB_SPI2_BASE                 0x00a0B000
#define APBC_SPI3_BASE                 0x00a0C000
#define APBD_SPI4_BASE                 0x00a0D000
#define APBE_I2C1_BASE                 0x00a0E000
#define APBF_I2C2_BASE                 0x00a0F000
#define APB10_PWM_BASE                 0x00a10000

#define p_AHB0_MFC_BASE                (*((volatile unsigned long *) 0x00800000))
#define p_AHB1_CDRDER_BASE             (*((volatile unsigned long *) 0x00900000))
#define p_AHB2_AHB2APB_BASE            (*((volatile unsigned long *) 0x00a00000))
#define p_APB1_ADC_BASE                (*((volatile unsigned long *) 0x00a01000))
#define p_APB2_DAC_BASE                (*((volatile unsigned long *) 0x00a02000))
#define p_APB3_ICU_BASE                (*((volatile unsigned long *) 0x00a03000))
#define p_APB4_DES_BASE                (*((volatile unsigned long *) 0x00a04000))
#define P_APB5_MODEMFIFO_BASE          (*((volatile unsigned long *) 0x00a05000))
#define P_APB6_WDT_BASE                (*((volatile unsigned long *) 0x00a06000))
#define P_APB7_GPIO_BASE               (*((volatile unsigned long *) 0x00a07000))
#define P_APB8_UART1_BASE              (*((volatile unsigned long *) 0x00a08000))
#define P_APB9_UART2_BASE              (*((volatile unsigned long *) 0x00a09000))
#define P_APBA_SPI1_BASE               (*((volatile unsigned long *) 0x00a0A000))
#define P_APBB_SPI2_BASE               (*((volatile unsigned long *) 0x00a0B000))
#define P_APBC_SPI3_BASE               (*((volatile unsigned long *) 0x00a0C000))
#define P_APBD_SPI4_BASE               (*((volatile unsigned long *) 0x00a0D000))
#define P_APBE_I2C1_BASE               (*((volatile unsigned long *) 0x00a0E000))
#define P_APBF_I2C2_BASE               (*((volatile unsigned long *) 0x00a0F000))
#define p_APB10_PWM_BASE               (*((volatile unsigned long *) 0x00a10000))

//////////////////////////////////
//register definition
//////////////////////////////////

//MFC
#define REG_AHB0_MFC_KEYWORD           (*((volatile unsigned long *)   0x00800000))
#define REG_AHB0_MFC_CONTROL           (*((volatile unsigned long *)   0x00800004))
#define REG_AHB0_MFC_ADDR              (*((volatile unsigned long *)   0x00800008))
#define REG_AHB0_MFC_DATA              (*((volatile unsigned long *)   0x0080000C))

#define MFC_CONTROL_READ               (0x00)
#define MFC_CONTROL_WRITE              (0x01<<2)
#define MFC_CONTROL_SECT_ERASE         (0x02<<2)
#define MFC_CONTROL_BLOCK_ERASE        (0x03<<2)
#define MFC_CONTROL_CHIP_ERASE         (0x07<<2)
#define MFC_CONTROL_MAIN_SPACE         (0x00<<5)
#define MFC_CONTROL_NVR_SPACE          (0x01<<5)

//ADC
#define ADC_SLEEP_MODE					0
#define ADC_STEP_MODE					1
#define ADC_SOFTCTRL_MODE				2
#define ADC_SEQUENT_MODE				3

#define ADC_CHANNEL_SEL_BIT				2
#define ADC_FIFO_EMPTY					6
#define ADC_SAMPLE_RATE_BIT				8
#define ADC_SETTLING_BIT				10
#define ADC_CLK_RATE_BIT				16

#define ADC_CLK_DIV_4					0
#define ADC_CLK_DIV_8					1
#define ADC_CLK_DIV_16					2
#define ADC_CLK_DIV_32					3

#define REG_APB1_ADC_CFG               (*((volatile unsigned long *)   0x00a01000))
#define REG_APB1_ADC_DAT               (*((volatile unsigned long *)   0x00a01004))

//DAC  0x00870000
#define REG_APB2_SDDAC_CLKDIVID        (*((volatile unsigned long *)   0x00a02000))
#define REG_APB2_SDDAC_FIFOCONF        (*((volatile unsigned long *)   0x00a02004))
#define REG_APB2_SDDAC_TESTCONF        (*((volatile unsigned long *)   0x00a02008))
#define REG_APB2_SDDAC_ENABLE          (*((volatile unsigned long *)   0x00a0200c))
#define REG_APB2_SDDAC_DACRFIFO        (*((volatile unsigned long *)   0x00a02020))
#define REG_APB2_SDDAC_DACLFIFO        (*((volatile unsigned long *)   0x00a02024))
#define REG_APB2_SDDAC_ADCRFIFO        (*((volatile unsigned long *)   0x00a02028))
#define REG_APB2_SDDAC_ADCLFIFO        (*((volatile unsigned long *)   0x00a0202c))
#define REG_APB2_SDDAC_FIFOSTAT        (*((volatile unsigned long *)   0x00a02030))

#define FIFO_DACR_INT_bit              (0x01<<16)
#define FIFO_DACL_INT_bit              (0x01<<17)
#define FIFO_ADCR_INT_bit              (0x01<<18)
#define FIFO_ADCL_INT_bit              (0x01<<19)
#define FIFO_DACR_FULL_bit      	   (0x01<<8)
#define FIFO_DACL_FULL_bit             (0x01<<9)
#define FIFO_DACR_EMPTY_bit            (0x01<<12)
#define FIFO_DACL_EMPTY_bit            (0x01<<13)

//ICU register definitions
#define REG_APB3_ICU_CLOCKSOURCE       (*((volatile unsigned long *)   0x00a03000))
#define REG_APB3_ICU_CPUCLKCON         (*((volatile unsigned long *)   0x00a03004))
#define REG_APB3_ICU_CDRDERCLKCON      (*((volatile unsigned long *)   0x00a0300c))
#define REG_APB3_ICU_ADCCLKCON         (*((volatile unsigned long *)   0x00a03010))
#define REG_APB3_ICU_DACCLKCON         (*((volatile unsigned long *)   0x00a03014))
#define REG_APB3_ICU_DESCLKCON         (*((volatile unsigned long *)   0x00a03018))
#define REG_APB3_ICU_MFIFOCLKCON       (*((volatile unsigned long *)   0x00a0301c))
#define REG_APB3_ICU_WDTCLKCON         (*((volatile unsigned long *)   0x00a03020))
#define REG_APB3_ICU_UART1CLKCON       (*((volatile unsigned long *)   0x00a03024))
#define REG_APB3_ICU_UART2CLKCON       (*((volatile unsigned long *)   0x00a03028))
#define REG_APB3_ICU_SPI1CLKCON        (*((volatile unsigned long *)   0x00a0302c))
#define REG_APB3_ICU_SPI2CLKCON        (*((volatile unsigned long *)   0x00a03030))
#define REG_APB3_ICU_SPI3CLKCON        (*((volatile unsigned long *)   0x00a03034))
#define REG_APB3_ICU_SPI4CLKCON        (*((volatile unsigned long *)   0x00a03038))
#define REG_APB3_ICU_I2C1CLKCON        (*((volatile unsigned long *)   0x00a0303C))
#define REG_APB3_ICU_I2C2CLKCON        (*((volatile unsigned long *)   0x00a03040))
#define REG_APB3_ICU_PWMCLKCON         (*((volatile unsigned long *)   0x00a03044))
#define REG_APB3_ICU_GPIOB7CLKCON      (*((volatile unsigned long *)   0x00a03048))

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
//#define BK5863_ICU_REG20_INTEN_CONFIG			(*((volatile unsigned long *)(0x00a03080)))
#define REG_APB3_ICU_INT_ENABLE        (*((volatile unsigned long *)   0x00a03080))
#define REG_APB3_ICU_IRQTYPE_EN        (*((volatile unsigned long *)   0x00a03084))
#define REG_APB3_ICU_INT_WAKENA        (*((volatile unsigned long *)   0x00a03088))
#define REG_APB3_ICU_INT_CLEAR         (*((volatile unsigned long *)   0x00a0308C))
#define REG_APB3_ICU_INT_FLAG          (*((volatile unsigned long *)   0x00a03090))
#define REG_APB3_ICU_ANA1_CTRL         (*((volatile unsigned long *)   0x00a030C0))
#define REG_APB3_ICU_ANA2_CTRL         (*((volatile unsigned long *)   0x00a030C4))
#define REG_APB3_ICU_ANA3_CTRL         (*((volatile unsigned long *)   0x00a030C8))
#define REG_APB3_ICU_ANA4_CTRL         (*((volatile unsigned long *)   0x00a030CC))
#define REG_APG3_ICU_DEEPSLEEP		     (*((volatile unsigned long *)   0x00a030D0)) 
#define REG_APG3_ICU_CHIPID		         (*((volatile unsigned long *)   0x00a030D4)) 
#define REG_APG3_ICU_ARM_WR_FLASH	     (*((volatile unsigned long *)   0x00a030D8))
#define REG_APB3_ICU_FIQ_ENABLE        (*((volatile unsigned long *)   0x00a030DC))
#define REG_APB3_ICU_FIQ_WAKENA        (*((volatile unsigned long *)   0x00a030E0))
#define REG_APB3_ICU_FIQ_CLEAR         (*((volatile unsigned long *)   0x00a030E4))
#define REG_APB3_ICU_FIQ_FLAG          (*((volatile unsigned long *)   0x00a030E8))

#define SYSCLK_HFSRC_32K				0
#define SYSCLK_HFSRC_32M				1
#define SYSCLK_HFSRC_64M				2
#define SYSCLK_HFSRC_30M_DCO		    3

//FIQ
#define INT_STATUS_LOW_VOLT_bit        (0x01<< 0)
#define INT_STATUS_HIGH_VOLT_bit       (0x01<< 1)
                                                                      
//IRQ
#define INT_STATUS_Detoutao_bit      	 (0x01<<20)
#define INT_STATUS_Detoutbo_bit        (0x01<<19)
#define INT_STATUS_PWM4_bit            (0x01<<18)
#define INT_STATUS_PWM3_bit            (0x01<<17)
#define INT_STATUS_PWM2_bit            (0x01<<16)
#define INT_STATUS_PWM1_bit            (0x01<<15)
#define INT_STATUS_PWM0_bit            (0x01<<14)
#define INT_STATUS_I2C2_bit            (0x01<<13)
#define INT_STATUS_I2C1_bit            (0x01<<12)
#define INT_STATUS_SPI4_bit            (0x01<<11)
#define INT_STATUS_SPI3_bit            (0x01<<10)
#define INT_STATUS_SPI2_bit            (0x01<< 9)
#define INT_STATUS_SPI1_bit            (0x01<< 8)
#define INT_STATUS_UART2_bit           (0x01<< 7)
#define INT_STATUS_UART1_bit           (0x01<< 6)
#define INT_STATUS_GPIO_bit            (0x01<< 5)
#define INT_STATUS_MFIFO_bit           (0x01<< 4)
#define INT_STATUS_DES_bit             (0x01<< 3)
#define INT_STATUS_DAC_bit             (0x01<< 2)
#define INT_STATUS_ADC_bit             (0x01<< 1)
#define INT_STATUS_CDRDER_bit          (0x01<< 0)

//TDES
#define DES_MODULE_RST_BIT				0
#define DES_OUT_RST_BIT					1
#define DES_ADDR_RST_BIT				2
#define DES_MAC_BIT						4
#define DES_ENC_BIT						5
#define DES_TDES_BIT					6
#define DES_EN_BIT						7

#define REG_APB4_DES_CTL               (*((volatile unsigned long *)   0x00a04004))
#define REG_APB4_DES_INTF              (*((volatile unsigned long *)   0x00a04008))
#define REG_APB4_DES_KEY1              (*((volatile unsigned long *)   0x00a0400c))
#define REG_APB4_DES_KEY2              (*((volatile unsigned long *)   0x00a04010))
#define REG_APB4_DES_KEY3              (*((volatile unsigned long *)   0x00a04014))
#define REG_APB4_DES_IN                (*((volatile unsigned long *)   0x00a04018))
#define REG_APB4_DES_OUT               (*((volatile unsigned long *)   0x00a0401c))

//MODEM_FIFO
#define REG_APB5_MODEM_FIFO_CFG        (*((volatile unsigned long *)   0x00a05000))
#define REG_APB5_MODEM_FIFO_ADDR       (*((volatile unsigned long *)   0x00a05004))
#define REG_APB5_MODEM_FIFO_INTCLR     (*((volatile unsigned long *)   0x00a05008))

//WDT
#define REG_APB6_WDT_CFG               (*((volatile unsigned long *)   0x00a06000))

//GPIO               
#define REG_APB7_GPIO_Basic            0x00a07000
#define REG_APB7_GPIOA_CFG             (*((volatile unsigned long *)   0x00a07000))  
#define REG_APB7_GPIOA_DATA            (*((volatile unsigned long *)   0x00a07004))  
#define REG_APB7_GPIOB_CFG             (*((volatile unsigned long *)   0x00a07008))  
#define REG_APB7_GPIOB_DATA            (*((volatile unsigned long *)   0x00a0700C))  
#define REG_APB7_GPIOC_CFG             (*((volatile unsigned long *)   0x00a07010))  
#define REG_APB7_GPIOC_DATA            (*((volatile unsigned long *)   0x00a07014))  
#define REG_APB7_GPIOD_CFG             (*((volatile unsigned long *)   0x00a07018))  
#define REG_APB7_GPIOD_DATA            (*((volatile unsigned long *)   0x00a0701C))  
#define REG_APB7_GPIOE_CFG             (*((volatile unsigned long *)   0x00a07020))  
#define REG_APB7_GPIOE_DATA            (*((volatile unsigned long *)   0x00a07024))  
#define REG_APB7_GPIOF_CFG             (*((volatile unsigned long *)   0x00a07028))  
#define REG_APB7_GPIOF_DATA            (*((volatile unsigned long *)   0x00a0702C))  
#define REG_APB7_GPIO_INT_LEVEL        (*((volatile unsigned long *)   0x00a07038))  
#define REG_APB7_GPIO_INT_ENABLE       (*((volatile unsigned long *)   0x00a0703C))  
#define REG_APB7_GPIO_INT_STATUS       (*((volatile unsigned long *)   0x00a07040))  
#define REG_APB7_GPIO_INT2_LEVEL       (*((volatile unsigned long *)   0x00a07044))  
#define REG_APB7_GPIO_INT2_ENABLE      (*((volatile unsigned long *)   0x00a07048))  
#define REG_APB7_GPIO_INT2_STATUS      (*((volatile unsigned long *)   0x00a0704C)) 
#define REG_APB7_GPIO_INT_PULSE        (*((volatile unsigned long *)   (0x00a07000+(0x14*4)))) 
#define REG_APB7_GPIO_INT_PULSE2       (*((volatile unsigned long *)   (0x00a07000+(0x15*4)))) 
#define REG_APB7_GPIO_INT_TRIG_MODE    (*((volatile unsigned long *)   (0x00a0704C+12))) 
#define REG_APB7_GPIO_INT_TRIG_MODE2   (*((volatile unsigned long *)   (0x00a0704C+16))) 
#define REG_APB7_GPIO_INT_TRIG_dpulse  (*((volatile unsigned long *)  (REG_APB7_GPIO_Basic+(0x18*4)))) 
#define REG_APB7_GPIO_INT_TRIG_dpulse2 (*((volatile unsigned long *)  (REG_APB7_GPIO_Basic+(0x19*4)))) 
#define REG_APB7_GPIO_CFG(exc)         (*((volatile unsigned long *)  (0x00a07000+(8*exc))))
#define REG_APB7_GPIO_DATA(exc)        (*((volatile unsigned long *)  (0x00a07004+(8*exc))))
//UART1
#define UART_TX_EN_BIT						0
#define UART_RX_EN_BIT						1
#define UART_IRDA_MODE_EN_BIT				2
#define UART_DATA_LEN_BIT					3
#define UART_PAR_EN_BIT						5
#define UART_PAR_MODE_BIT					6
#define UART_STOP_LEN_BIT					7
#define UART_CLK_DIV_BIT					8
#define UART_TX_FIFO_RESET_BIT				21
#define UART_RX_FIFO_RESET_BIT				22




#define UART_FRAME_LEN_5_BIT				0
#define UART_FRAME_LEN_6_BIT				1
#define UART_FRAME_LEN_7_BIT				2
#define UART_FRAME_LEN_8_BIT				3

#define UART_TX_INT_EN	 REG_APB8_UART1_INT_ENABLE |= 0x01
#define UART_RX_INT_EN	 REG_APB8_UART1_INT_ENABLE |= ((0x01 << 1)|(0x1 << 6))
#define UART_TX_INT_DISENABLE	 REG_APB8_UART1_INT_ENABLE &= ~0x01
#define UART_RX_INT_DISENABLE	 REG_APB8_UART1_INT_ENABLE &= ~((0x01 << 1)|(0x1 << 6))
#define UART2_TX_INT_EN	 REG_APB9_UART2_INT_ENABLE |= 0x01
#define UART2_RX_INT_EN	 REG_APB9_UART2_INT_ENABLE |= 0x02
#define UART2_TX_INT_DISENABLE  REG_APB9_UART2_INT_ENABLE &= ~0x01
#define UART2_RX_INT_DISENABLE  REG_APB9_UART2_INT_ENABLE &= ~0x02

#define UART_TX_INT_BIT						0
#define UART_RX_INT_BIT						1

#define REG_APB8_UART1_CFG             (*((volatile unsigned long *)   0x00a08000))
#define REG_APB8_UART1_FIFO_THRESHOLD  (*((volatile unsigned long *)   0x00a08004))
#define REG_APB8_UART1_FIFO_COUNT      (*((volatile unsigned long *)   0x00a08008))
#define REG_APB8_UART1_DATA            (*((volatile unsigned long *)   0x00a0800C))
#define REG_APB8_UART1_INT_ENABLE      (*((volatile unsigned long *)   0x00a08010))
#define REG_APB8_UART1_INT_FLAG        (*((volatile unsigned long *)   0x00a08014))
#define REG_APB8_UART1_FLOW_CTRL       (*((volatile unsigned long *)   0x00a08018))
#define REG_APB8_UART1_WAKE_CTRL       (*((volatile unsigned long *)   0x00a0801c))

//#define 	UART1_CFG							(*((volatile unsigned long *)(APB8_UART1_BASE+0x00)))
//#define 	UART1_FIFO_THRESHOLD				(*((volatile unsigned long *)(APB8_UART1_BASE+0x04)))
//#define 	UART1_FIFO_COUNT					(*((volatile unsigned long *)(APB8_UART1_BASE+0x08)))
//#define 	UART1_DATA							(*((volatile unsigned long *)(APB8_UART1_BASE+0x0C)))
//#define 	UART1_INT_EN						(*((volatile unsigned long *)(APB8_UART1_BASE+0x10)))
//#define 	UART1_INT_FLAG						(*((volatile unsigned long *)(APB8_UART1_BASE+0x14)))
//#define 	UART1_WAKE_CTRL					(*((volatile unsigned long *)(APB8_UART1_BASE+0x1C)))



//UART2  
#define REG_APB9_UART2_CFG             (*((volatile unsigned long *)   0x00a09000))
#define REG_APB9_UART2_FIFO_THRESHOLD  (*((volatile unsigned long *)   0x00a09004))
#define REG_APB9_UART2_FIFO_COUNT      (*((volatile unsigned long *)   0x00a09008))
#define REG_APB9_UART2_DATA            (*((volatile unsigned long *)   0x00a0900C))
#define REG_APB9_UART2_INT_ENABLE      (*((volatile unsigned long *)   0x00a09010))
#define REG_APB9_UART2_INT_FLAG        (*((volatile unsigned long *)   0x00a09014))
#define REG_APB9_UART2_FLOW_CTRL       (*((volatile unsigned long *)   0x00a09018))
#define REG_APB9_UART2_WAKE_CTRL       (*((volatile unsigned long *)   0x00a0901c))

//#define 	UART2_CFG							(*((volatile unsigned long *)(APB9_UART2_BASE+0x00)))
//#define 	UART2_FIFO_THRESHOLD	(*((volatile unsigned long *)(APB9_UART2_BASE+0x04)))
//#define 	UART2_FIFO_COUNT			(*((volatile unsigned long *)(APB9_UART2_BASE+0x08)))
//#define 	UART2_DATA						(*((volatile unsigned long *)(APB9_UART2_BASE+0x0C)))
//#define 	UART2_INT_EN					(*((volatile unsigned long *)(APB9_UART2_BASE+0x10)))
//#define 	UART2_INT_FLAG				(*((volatile unsigned long *)(APB9_UART2_BASE+0x14)))
//#define 	UART2_WAKE_CTRL				(*((volatile unsigned long *)(APB9_UART2_BASE+0x1C)))


//SPI1
#define REG_APBA_SPI1_CTRL             (*((volatile unsigned long *)   0x00a0a000))
#define REG_APBA_SPI1_STAT             (*((volatile unsigned long *)   0x00a0a004))
#define REG_APBA_SPI1_DAT              (*((volatile unsigned long *)   0x00a0a008))

#define SPI_EN_BIT                     (1 << 23)
#define SPI_MAST_BIT                   (1 << 22)
#define SPI_NSSMD_BIT                  (3 << 16)
#define SPI_TXINT_BIT                  (1 << 8)
#define SPI_RXINT_BIT                  (1 << 9)
#define SPI_RXEMPTY_BIT                (1 << 2)

#define	 SPI_BUSY									BIT(15)
#define	 SPI_RX_OVER							BIT(12)
#define  SPI_TX_OVER							BIT(11)
#define  SPI_INT_RX								BIT(9)
#define  SPI_INT_TX								BIT(8)


//SPI2
#define REG_APBB_SPI2_CTRL             (*((volatile unsigned long *)   0x00a0b000))
#define REG_APBB_SPI2_STAT             (*((volatile unsigned long *)   0x00a0b004))
#define REG_APBB_SPI2_DAT              (*((volatile unsigned long *)   0x00a0b008))

//SPI3
#define REG_APBC_SPI3_CTRL             (*((volatile unsigned long *)   0x00a0c000))
#define REG_APBC_SPI3_STAT             (*((volatile unsigned long *)   0x00a0c004))
#define REG_APBC_SPI3_DAT              (*((volatile unsigned long *)   0x00a0c008))

//SPI4
#define REG_APBD_SPI4_CTRL             (*((volatile unsigned long *)   0x00a0d000))
#define REG_APBD_SPI4_STAT             (*((volatile unsigned long *)   0x00a0d004))
#define REG_APBD_SPI4_DAT              (*((volatile unsigned long *)   0x00a0d008))

//I2C1
#define REG_APBE_I2C1_CN               (*((volatile unsigned long *)   0x00a0e000))
#define REG_APBE_I2C1_STAT             (*((volatile unsigned long *)   0x00a0e004))
#define REG_APBE_I2C1_DAT              (*((volatile unsigned long *)   0x00a0e008))

#define I2C_EN_BIT                     (0X80000000) //(0x1 << 31)
#define I2C_MAST_BIT                   (1 << 14)
#define I2C_TXMODE_BIT                 (1 << 13)
#define I2C_ACKRQ_BIT                  (1 << 12)
#define I2C_ADDRMATCH_BIT              (1 << 11)
#define I2C_STA_BIT                    (1 << 10)
#define I2C_STO_BIT                    (1 << 9)
#define I2C_ACK_BIT                    (1 << 8)
#define I2C_INTMODE_BIT                (3 << 6)
#define I2C_RXFIFO_EMPTY_BIT           (1 << 4)
#define I2C_INT_BIT                    (1 << 0)

//I2C2
#define REG_APBF_I2C2_CN               (*((volatile unsigned long *)   0x00a0f000))
#define REG_APBF_I2C2_STAT             (*((volatile unsigned long *)   0x00a0f004))
#define REG_APBF_I2C2_DAT              (*((volatile unsigned long *)   0x00a0f008))


//PWM
#define REG_APB10_PWM_0                (*((volatile unsigned long *)   0x00a10000))  
#define REG_APB10_PWM_1                (*((volatile unsigned long *)   0x00a10004))  
#define REG_APB10_PWM_2                (*((volatile unsigned long *)   0x00a10008))  
#define REG_APB10_PWM_3                (*((volatile unsigned long *)   0x00a1000c))  
#define REG_APB10_PWM_4                (*((volatile unsigned long *)   0x00a10010))  

#define	PT3_CLOCK_SELECT		8
#define REG_APB10_PWM_CFG              (*((volatile unsigned long *)   0x00a10014))  
#define REG_APB10_PWM_CLK              (*((volatile unsigned long *)   0x00a10018))  

#define REG_RTC_CFG                    (*((volatile unsigned long *)   0x00a1001C))

#define REG_APB10_PT0_CNT              (*((volatile unsigned long *)   0x00a10020))  
#define REG_APB10_PT1_CNT              (*((volatile unsigned long *)   0x00a10024))  
#define REG_APB10_PT2_CNT              (*((volatile unsigned long *)   0x00a10028))  
#define REG_APB10_PT3_CNT              (*((volatile unsigned long *)   0x00a1002c))  
#define REG_APB10_PT4_CNT              (*((volatile unsigned long *)   0x00a10030))  



#define PT0_MODE_BIT				0
#define PT1_MODE_BIT				1
#define PT2_MODE_BIT				2
#define PT3_MODE_BIT				3
#define PT4_MODE_BIT				4
#define PT_ENABLE_BIT				7
#define PT_CLK_DIV_BIT				8
#define PT0_INT_FLAG				(1<<16)
#define PT1_INT_FLAG				(1<<17)
#define PT2_INT_FLAG				(1<<18)
#define PT3_INT_FLAG				(1<<19)
#define PT4_INT_FLAG				(1<<20)

//LBD
#define REG_APB11_LBD_CLK32K_PWD       (*((volatile unsigned long *)   0x00a11000)) 
#define REG_APB11_LBD_LBD_INT_FLAG     (*((volatile unsigned long *)   0x00a11004))  


//FLASH space
#define FLASH_ADDR0                    (*((volatile unsigned long *)   0x0000F000))
#define FLASH_ADDR1                    (*((volatile unsigned long *)   0x0000F100))
#define FLASH_ADDR2                    (*((volatile unsigned long *)   0x0000F200))
#define FLASH_ADDR3                    (*((volatile unsigned long *)   0x0000F300))

//CARD READER
#define ADAPTER_CLK_ACTIVE							0
#define ADAPTER_CLK_INACTIVE						1


#define WriteRawRC(reg_addr,reg_v)     (*((volatile unsigned long *)(AHB1_CDRDER_BASE + reg_addr*4))) = (reg_v)
#define ReadRawRC(reg_addr)            (unsigned char)(*((volatile unsigned long *)(AHB1_CDRDER_BASE + reg_addr*4)))

#define BK5863_WRITE_REG(reg_addr,reg_v)		((*((volatile unsigned long *)(reg_addr))) = reg_v)
#define BK5863_READ_REG(reg_addr)				(*((volatile unsigned long *)   (reg_addr)))

#endif



/***********************  END OF FILES  ***********************/
