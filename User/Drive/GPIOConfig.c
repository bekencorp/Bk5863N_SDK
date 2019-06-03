#include "GPIOConfig.h"
#include "Includes.h"

/**************************************************
FunctionName:GPIOTestConfig
Describe:
InputParameter:
OutputParameter:
Notice:
**************************************************/
void GPIOTestConfig(void)
{
	GPIOConfig_T GIPOConfig;
	
	 /* GPIOF_5 */
	/* �����ʵ��Ӳ������,�ṹ��ֵ���������ȱʡ */
	GIPOConfig.GPIO_PERI = GPIO_General_Function;  //��ͨ����
	GIPOConfig.GPIO_OE_N_INTERN = Disable;  //���ʹ��
	GIPOConfig.GPIO_I_IE = Enable;  //����ʹ��
	GIPOConfig.GPIO_PULLUP = Disable;  //����ʹ��
	GIPOConfig.GPIO_PULLDOWN = Enable;  //����ʹ��
	
	GIPOConfig.Int.isINT = Enable;    //�����ж�
	GIPOConfig.Int.En_Dis_able = Enable;  //GPIO_Pin�ж�
	#if 0  //���ش��������ش��� �����ش���
		GIPOConfig.Int.INTMode = edge_Trigging; //���ش���
		GIPOConfig.Int.dpulse = _SPULSE;  //���ش���
		GIPOConfig.Int.INTtrigger = Rising_edge_Trigging;
	#endif 
	#if 0 //���ش��������ش���  �����ش���
		GIPOConfig.Int.INTMode = edge_Trigging; //���ش���
		GIPOConfig.Int.dpulse = _SPULSE;  //���ش���
		GIPOConfig.Int.INTtrigger = falling_edge_Tringging;
	#endif
	#if 1 //���ش����� ˫���ش���  
		GIPOConfig.Int.INTMode = edge_Trigging; //���ش���
		GIPOConfig.Int.dpulse = _DPULSE;  //˫�ش���
//		GIPOConfig.Int.INTtrigger = falling_edge_Tringging;
	#endif
	#if 0 //��ƽ����, �ߵ�ƽ����
		GIPOConfig.Int.INTMode = level_Tringging;  //��ƽ����
		GIPOConfig.Int.INTtrigger = High_level_Tringging;
	#endif
	#if 0 //��ƽ����, �͵�ƽ����
		GIPOConfig.Int.INTMode = level_Tringging;  //��ƽ����
		GIPOConfig.Int.INTtrigger = Low_level_Trigging;
	#endif
	GIPOConfig.GPIO_Pin = GPIO_Pin_5;  //ѡ��GPIO_Pin
	
	GPIO_Config(GPIOF,&GIPOConfig);
	SetGPIOPin(GPIOF,GPIO_Pin_5);
	
	/* GPIO�жϻ��� */
	SetGPIOINT_WAKENA(Enable);
	/* ʹ��GPIO�ж�Դ */
	SetGPIOIntCmd(Enable);
}
/**************************************************
FunctionName: LEDConfig
Describe: ָʾ������
InputParameter:
OutputParameter:
Notice:  GPIOB1;GPIOB0
**************************************************/
void LEDConfig(void)
{
	GPIOConfig_T GIPOConfig;

	GIPOConfig.GPIO_PERI = GPIO_General_Function;  //��ͨ����
	GIPOConfig.GPIO_OE_N_INTERN = Enable;  //���ʹ��
	GIPOConfig.GPIO_PULLUP = Disable;//Enable;  //����ʹ��
	GIPOConfig.GPIO_PULLDOWN = Disable;  //����ʹ��
	GIPOConfig.GPIO_I_IE = Disable;//Enable;  //����ʹ��
	GIPOConfig.Int.isINT = Disable;    //�����ж�
	GIPOConfig.GPIO_Pin = GPIO_Pin_1|GPIO_Pin_0|GPIO_Pin_6;
	/* Config */
	GPIO_Config(GPIOB,&GIPOConfig);
	GPIO_Config(GPIOF,&GIPOConfig);
	/* Set High Level */
	SetGPIOPin(GPIOB,GPIO_Pin_1|GPIO_Pin_0|GPIO_Pin_6);
	ResetGPIOPin(GPIOF,GPIO_Pin_1|GPIO_Pin_0);
	
#ifdef BK5870
//	GPIOConfig_T GIPOConfig;
	
	GIPOConfig.GPIO_PERI = GPIO_General_Function;  //ÆÕÍ¨¹¦ÄÜ
	GIPOConfig.GPIO_OE_N_INTERN = Enable;  //Êä³öÊ¹ÄÜ
	GIPOConfig.GPIO_PULLUP = Disable;//Enable;  //ÉÏÀ­Ê¹ÄÜ
	GIPOConfig.GPIO_PULLDOWN = Disable;  //ÏÂÀ­Ê¹ÄÜ
	GIPOConfig.GPIO_I_IE = Disable;//Enable;  //ÊäÈëÊ¹ÄÜ
	GIPOConfig.Int.isINT = Disable;    //ÅäÖÃÖÐ¶Ï
	GIPOConfig.GPIO_Pin = GPIO_Pin_5|GPIO_Pin_6;
	/* Config */
	GPIO_Config(GPIOE,&GIPOConfig);
	/* Set High Level */
	SetGPIOPin(GPIOE,GPIO_Pin_5|GPIO_Pin_6);
#endif
}
/**************************************************
FunctionName:
Describe:
InputParameter:
OutputParameter:
Notice: GPIOB1;GPIOB0
**************************************************/
void FlashLED(void)
{
	unsigned char i= 5;
	for(;i>0;i--)
	{
		delay_ms(20);
	#ifdef BK5870
		ResetGPIOPin(GPIOE,GPIO_Pin_6);
		SetGPIOPin(GPIOE,GPIO_Pin_5);
	#endif
		ResetGPIOPin(GPIOB,GPIO_Pin_1);
		SetGPIOPin(GPIOB,GPIO_Pin_0);
		delay_ms(20);
		ResetGPIOPin(GPIOB,GPIO_Pin_0);
		SetGPIOPin(GPIOB,GPIO_Pin_1);
	#ifdef BK5870
		SetGPIOPin(GPIOE,GPIO_Pin_6);
		ResetGPIOPin(GPIOE,GPIO_Pin_5);
	#endif
		delay_ms(20);
		SetGPIOPin(GPIOB,GPIO_Pin_1|GPIO_Pin_0);
	#ifdef BK5870
		SetGPIOPin(GPIOE,GPIO_Pin_5|GPIO_Pin_6);
	#endif
		delay_ms(40);
		ResetGPIOPin(GPIOB,GPIO_Pin_1|GPIO_Pin_0);
	#ifdef BK5870
		ResetGPIOPin(GPIOE,GPIO_Pin_5|GPIO_Pin_6);
	#endif
		delay_ms(40);
		SetGPIOPin(GPIOB,GPIO_Pin_1|GPIO_Pin_0);
	#ifdef BK5870
		SetGPIOPin(GPIOE,GPIO_Pin_5|GPIO_Pin_6);
	#endif
		delay_ms(40);
	#ifdef BK5870
		ResetGPIOPin(GPIOE,GPIO_Pin_5|GPIO_Pin_6);
	#endif
		ResetGPIOPin(GPIOB,GPIO_Pin_1|GPIO_Pin_0);
	}
}
/**************************************************
FunctionName:
Describe:
InputParameter:
OutputParameter:
Notice: GPIOB1;GPIOB0
**************************************************/
void LEDOpen(uint32_t ledx)
{
#ifdef BK5863N
	if((ledx&0x01) == 0x1)
		ResetGPIOPin(GPIOB,GPIO_Pin_0);
	if((ledx&0x02) == 0x02)
		ResetGPIOPin(GPIOB,GPIO_Pin_1);
#endif
#ifdef BK5870
	if((ledx&0x01) == 0x1)
		ResetGPIOPin(GPIOE,GPIO_Pin_5);
	if((ledx&0x02) == 0x02)
		ResetGPIOPin(GPIOE,GPIO_Pin_6);
#endif
}
/**************************************************
FunctionName:
Describe:
InputParameter:
OutputParameter:
Notice: GPIOB1;GPIOB0
**************************************************/
void LEDClose(uint32_t ledx)
{
#ifdef BK5863N
	if((ledx&0x01) == 0x1)
		SetGPIOPin(GPIOB,GPIO_Pin_0);
	if((ledx&0x02) == 0x02)
		SetGPIOPin(GPIOB,GPIO_Pin_1);
#endif
#ifdef BK5870
	if((ledx&0x01) == 0x1)
		SetGPIOPin(GPIOE,GPIO_Pin_5);
	if((ledx&0x02) == 0x02)
		SetGPIOPin(GPIOE,GPIO_Pin_6);
#endif
}


/***********************  END OF FILES  ***********************/
