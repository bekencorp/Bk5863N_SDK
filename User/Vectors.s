;/****************************************Copyright (c)**************************************************
;**                               ��ͨ���ɵ�·���Ϻ����ɷ����޹�˾
;**                                     ��  ��  ��
;**
;**                                 http://www.bekencorp.com/
;**
;**--------------�ļ���Ϣ--------------------------------------------------------------------------------
;**��   ��   ��: vectors.s
;**��   ��   ��: wei
;**����޸�����: 2018��4��20��
;**��        ��: bk5863n�쳣������ڼ��쳣������c���Դ���Ľӿڣ�������ʼ����ջ�Ĵ���
;**              ÿ������Ӧ���ж���������ļ��Ŀ�������������Ӧ���޸�  
;**--------------��ʷ�汾��Ϣ----------------------------------------------------------------------------
;** ������: 
;** ��  ��: v0.1
;** �ա���: 2017��5��30��
;** �衡��: ԭʼ�汾
;** 
;**--------------��ʷ�汾��Ϣ----------------------------------------------------------------------------
;** �޸���: caiwei
;** ��  ��: v1.0
;** �ա���: 2017��5��30��
;** �衡��: ԭʼ�汾
;**
;**--------------��ǰ�汾�޶�----------------------------------------------------------------------------
;** �޸���: 
;** �ա���:
;** �衡��:
;**
;**------------------------------------------------------------------------------------------------------
;********************************************************************************************************/

;�����ջ�Ĵ�С
USR_STACK_LEGTH     EQU         512
SVC_STACK_LEGTH     EQU         0
FIQ_STACK_LEGTH     EQU         64
IRQ_STACK_LEGTH     EQU         256
ABT_STACK_LEGTH     EQU         0
UND_STACK_LEGTH     EQU         16


;������ⲿ�����������
    IMPORT  FIQ_Exception                   ;�����ж��쳣�������
    IMPORT  IRQ_Exception
	IMPORT  Undefined_Exception
    IMPORT  Reset

;���ⲿʹ�õı����������
    EXPORT  InitStack
    EXPORT  Vectors


    CODE32
	PRESERVE8
    AREA    Startup,CODE,READONLY

;�ж�������
Vectors
        LDR     PC, ResetAddr
        LDR     PC, UndefinedAddr
        LDR     PC, SWI_Addr
        LDR     PC, PrefetchAddr
        LDR     PC, DataAbortAddr
        DCD     0xb9205f80
        LDR     PC, IRQ_Addr
        LDR     PC, FIQ_Addr

ResetAddr           DCD     Reset
UndefinedAddr       DCD     Undefined
SWI_Addr            DCD     SoftwareInterrupt
PrefetchAddr        DCD     PrefetchAbort  ;ȡָ��
DataAbortAddr       DCD     DataAbort  ;ȡ���ݴ�����ָ���ַ������
Nouse               DCD     0
IRQ_Addr            DCD     IRQ_Handler
FIQ_Addr            DCD     FIQ_Handler

;δ����ָ��
Undefined
		STMFD   SP!, {R0-R12, LR}
        BL      Undefined_Exception
        LDMFD   SP!, {R0-R12, LR}
        SUBS    PC,  LR,  #4

;���ж�
SoftwareInterrupt
        B       SoftwareInterrupt

PrefetchAbort
        B       PrefetchAbort

;ȡ������ֹ
DataAbort
        B       DataAbort

;�����ж�
IRQ_Handler
        STMFD   SP!, {R0-R12, LR}  ;
        BL      IRQ_Exception
        LDMFD   SP!, {R0-R12, LR}  ;
        SUBS    PC,  LR,  #4


;�����ж�
FIQ_Handler
        STMFD   SP!, {R0-R12, LR}
        BL      FIQ_Exception
        LDMFD   SP!, {R0-R12, LR}
        SUBS    PC,  LR,  #4

;/*********************************************************************************************************
;** ��������: InitStack
;** ��������: ��ʼ����ջ
;** �䡡��:   ��
;** �䡡�� :  ��
;** ȫ�ֱ���: ��
;** ����ģ��: ��
;** 
;** ������: 
;** �ա���: 
;**-------------------------------------------------------------------------------------------------------
;** �ޡ���: 
;** �ա���: 
;**-------------------------------------------------------------------------------------------------------
;********************************************************************************************************/
InitStack    
        MOV     R0,  LR

;���ù���ģʽ��ջ
        MSR     CPSR_c, #0xd3
        LDR     SP, StackSvc
;�����ж�ģʽ��ջ
        MSR     CPSR_c, #0xd2
        LDR     SP, StackIrq
;���ÿ����ж�ģʽ��ջ
        MSR     CPSR_c, #0xd1
        LDR     SP, StackFiq
;������ֹģʽ��ջ
        MSR     CPSR_c, #0xd7
        LDR     SP, StackAbt
;����δ����ģʽ��ջ
        MSR     CPSR_c, #0xdb
        LDR     SP, StackUnd
;����ϵͳģʽ��ջ
        MSR     CPSR_c, #0xdf
        LDR     SP, StackUsr

		BX       R0
StackUsr           DCD     UsrStackSpace + (USR_STACK_LEGTH - 1) * 4
StackSvc           DCD     SvcStackSpace + (SVC_STACK_LEGTH - 1)* 4
StackIrq           DCD     IrqStackSpace + (IRQ_STACK_LEGTH - 1)* 4
StackFiq           DCD     FiqStackSpace + (FIQ_STACK_LEGTH - 1)* 4
StackAbt           DCD     AbtStackSpace + (ABT_STACK_LEGTH - 1)* 4
StackUnd           DCD     UndtStackSpace + (UND_STACK_LEGTH - 1)* 4


;/* �����ջ�ռ� */
        AREA    MyStacks, DATA, NOINIT, ALIGN=2
UsrStackSpace      SPACE   USR_STACK_LEGTH * 4  ;�û���ϵͳ��ģʽ��ջ�ռ�
SvcStackSpace      SPACE   SVC_STACK_LEGTH * 4  ;����ģʽ��ջ�ռ�
IrqStackSpace      SPACE   IRQ_STACK_LEGTH * 4  ;�ж�ģʽ��ջ�ռ�
FiqStackSpace      SPACE   FIQ_STACK_LEGTH * 4  ;�����ж�ģʽ��ջ�ռ�
AbtStackSpace      SPACE   ABT_STACK_LEGTH * 4  ;��ֹ��ģʽ��ջ�ռ�
UndtStackSpace     SPACE   UND_STACK_LEGTH * 4  ;δ����ģʽ��ջ

    END
;/*********************************************************************************************************
;**                            End Of File
;********************************************************************************************************/
