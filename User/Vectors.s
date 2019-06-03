;/****************************************Copyright (c)**************************************************
;**                               博通集成电路（上海）股份有限公司
;**                                     软  件  部
;**
;**                                 http://www.bekencorp.com/
;**
;**--------------文件信息--------------------------------------------------------------------------------
;**文   件   名: vectors.s
;**创   建   人: wei
;**最后修改日期: 2018年4月20日
;**描        述: bk5863n异常向量入口及异常向量与c语言代码的接口，包括初始化堆栈的代码
;**              每个工程应当有独立的这个文件的拷贝，并进行相应的修改  
;**--------------历史版本信息----------------------------------------------------------------------------
;** 创建人: 
;** 版  本: v0.1
;** 日　期: 2017年5月30日
;** 描　述: 原始版本
;** 
;**--------------历史版本信息----------------------------------------------------------------------------
;** 修改人: caiwei
;** 版  本: v1.0
;** 日　期: 2017年5月30日
;** 描　述: 原始版本
;**
;**--------------当前版本修订----------------------------------------------------------------------------
;** 修改人: 
;** 日　期:
;** 描　述:
;**
;**------------------------------------------------------------------------------------------------------
;********************************************************************************************************/

;定义堆栈的大小
USR_STACK_LEGTH     EQU         512
SVC_STACK_LEGTH     EQU         0
FIQ_STACK_LEGTH     EQU         64
IRQ_STACK_LEGTH     EQU         256
ABT_STACK_LEGTH     EQU         0
UND_STACK_LEGTH     EQU         16


;引入的外部标号在这声明
    IMPORT  FIQ_Exception                   ;快速中断异常处理程序
    IMPORT  IRQ_Exception
	IMPORT  Undefined_Exception
    IMPORT  Reset

;给外部使用的标号在这声明
    EXPORT  InitStack
    EXPORT  Vectors


    CODE32
	PRESERVE8
    AREA    Startup,CODE,READONLY

;中断向量表
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
PrefetchAddr        DCD     PrefetchAbort  ;取指令
DataAbortAddr       DCD     DataAbort  ;取数据错误，如指针地址不对齐
Nouse               DCD     0
IRQ_Addr            DCD     IRQ_Handler
FIQ_Addr            DCD     FIQ_Handler

;未定义指令
Undefined
		STMFD   SP!, {R0-R12, LR}
        BL      Undefined_Exception
        LDMFD   SP!, {R0-R12, LR}
        SUBS    PC,  LR,  #4

;软中断
SoftwareInterrupt
        B       SoftwareInterrupt

PrefetchAbort
        B       PrefetchAbort

;取数据中止
DataAbort
        B       DataAbort

;慢速中断
IRQ_Handler
        STMFD   SP!, {R0-R12, LR}  ;
        BL      IRQ_Exception
        LDMFD   SP!, {R0-R12, LR}  ;
        SUBS    PC,  LR,  #4


;快速中断
FIQ_Handler
        STMFD   SP!, {R0-R12, LR}
        BL      FIQ_Exception
        LDMFD   SP!, {R0-R12, LR}
        SUBS    PC,  LR,  #4

;/*********************************************************************************************************
;** 函数名称: InitStack
;** 功能描述: 初始化堆栈
;** 输　入:   无
;** 输　出 :  无
;** 全局变量: 无
;** 调用模块: 无
;** 
;** 作　者: 
;** 日　期: 
;**-------------------------------------------------------------------------------------------------------
;** 修　改: 
;** 日　期: 
;**-------------------------------------------------------------------------------------------------------
;********************************************************************************************************/
InitStack    
        MOV     R0,  LR

;设置管理模式堆栈
        MSR     CPSR_c, #0xd3
        LDR     SP, StackSvc
;设置中断模式堆栈
        MSR     CPSR_c, #0xd2
        LDR     SP, StackIrq
;设置快速中断模式堆栈
        MSR     CPSR_c, #0xd1
        LDR     SP, StackFiq
;设置中止模式堆栈
        MSR     CPSR_c, #0xd7
        LDR     SP, StackAbt
;设置未定义模式堆栈
        MSR     CPSR_c, #0xdb
        LDR     SP, StackUnd
;设置系统模式堆栈
        MSR     CPSR_c, #0xdf
        LDR     SP, StackUsr

		BX       R0
StackUsr           DCD     UsrStackSpace + (USR_STACK_LEGTH - 1) * 4
StackSvc           DCD     SvcStackSpace + (SVC_STACK_LEGTH - 1)* 4
StackIrq           DCD     IrqStackSpace + (IRQ_STACK_LEGTH - 1)* 4
StackFiq           DCD     FiqStackSpace + (FIQ_STACK_LEGTH - 1)* 4
StackAbt           DCD     AbtStackSpace + (ABT_STACK_LEGTH - 1)* 4
StackUnd           DCD     UndtStackSpace + (UND_STACK_LEGTH - 1)* 4


;/* 分配堆栈空间 */
        AREA    MyStacks, DATA, NOINIT, ALIGN=2
UsrStackSpace      SPACE   USR_STACK_LEGTH * 4  ;用户（系统）模式堆栈空间
SvcStackSpace      SPACE   SVC_STACK_LEGTH * 4  ;管理模式堆栈空间
IrqStackSpace      SPACE   IRQ_STACK_LEGTH * 4  ;中断模式堆栈空间
FiqStackSpace      SPACE   FIQ_STACK_LEGTH * 4  ;快速中断模式堆栈空间
AbtStackSpace      SPACE   ABT_STACK_LEGTH * 4  ;中止义模式堆栈空间
UndtStackSpace     SPACE   UND_STACK_LEGTH * 4  ;未定义模式堆栈

    END
;/*********************************************************************************************************
;**                            End Of File
;********************************************************************************************************/
