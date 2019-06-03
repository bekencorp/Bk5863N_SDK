#ifndef _TYPES_H_
#define _TYPES_H_

#include <stdint.h>
//#define NULL		((void*)0)

typedef void (*TCallback)();
typedef int (*CCallback)();

//----------------------------------------------
typedef unsigned char				BYTE,int8u;
typedef signed   long				int32;      //  �з���32λ���ͱ���
typedef signed   short				int16,int16u;      //  �з���16λ���ͱ���
typedef signed   char				int8;       //  �з���8λ���ͱ���
typedef unsigned long				uint32,int32u,UINT32;     //  �޷���32λ���ͱ���
typedef unsigned short				uint16;     //  �޷���16λ���ͱ���
typedef unsigned char				uint8;      //  �޷���8λ���ͱ���
typedef float						fp32;       //  �����ȸ�������32λ���ȣ�
typedef double						fp64;       //  ˫���ȸ�������64λ���ȣ�
typedef __int64						int64,INT64,SI64;
typedef unsigned __int64			uint64,UINT64,UI64;
typedef long long					LONGLONG;
typedef unsigned long long			ULONGLONG;
typedef unsigned int        size_t;
typedef signed long					/*int32_t,*/s32,SI32;
typedef signed short				int16_t,s16,SI16;
typedef signed char					int8_t,s8,SI8;
typedef unsigned long				/*uint32_t,*/u32,UI32,DWORD,UNIT;
typedef unsigned short				uint16_t,u16,UI16,*PUI16,WORD;
typedef unsigned char				uint8_t,u8,UI8,*PUI8;

typedef volatile short int			vs16;
typedef volatile int			vs32;
typedef volatile unsigned short int			vu16;
typedef volatile unsigned int			vu32;

//typedef enum { false=0, true=!false}  bool;
 typedef enum { FALSE=0, TRUE = !FALSE }  bool;


//���Ͻṹ�嶨��
typedef union
{
    int16u word;    //һ����16λ
    struct
    {
        int8u lb;   //һ���ֵĵ�8λ
        int8u hb;   //һ���ֵĸ�8λ
    }
    byte;
}
unword;

typedef union
{
	int32u two_word;
	struct
	{
		int16u lw;
		int16u hw;
	}
	word;
}
unlong;
//----------------------------------------------

#ifndef MAX
#define MAX(a,b) ((a > b) ? a : b)
#endif

#ifndef MIN
#define MIN(a,b) ((a < b) ? a : b)
#endif

#define BIT0				0x01
#define BIT1				0x02
#define BIT2				0x04
#define BIT3				0x08
#define BIT4				0x10
#define BIT5				0x20
#define BIT6				0x40
#define BIT7				0x80

#define BIT(n) \
	(1<<(n))
#define _BIT(n) \
	(~ BIT(n))

#define BFD(val,bs,bl) \
    (((unsigned long)(val&(BIT(bl)-1)))<<(bs))

#define GET_BFD(val,bs,bl) \
	(((val)&((BIT(bl)-1)<<(bs)))>>(bs))

#define _BFD(val,bs,bl) \
	(~ BFD(val,bs,bl))

#define SET_BFD(tar,val,bs,bl) \
	{\
	tar &= _BFD((1<<(bl))-1,bs,bl);\
	tar |= BFD(val,bs,bl);\
	}

//BIG ENDIAN to LITTLE ENDIAN
#define _BYTE(n,v) \
    (((v)>>(8*n))&0xff)
//����2�ָ�ʽ�ı任
#define B2L_32(bv) \
    ((_BYTE(0, bv)<<24)|\
    (_BYTE(1, bv)<<16)|\
    (_BYTE(2, bv)<<8)|\
    (_BYTE(3, bv)))

#define B2L_16(bv) \
    ((_BYTE(0, bv)<<8)|\
    (_BYTE(1, bv)))

#define RW
	//�ɶ�д
#define RO
	//ֻ��
#define WO
	//ֻд
#define W1C
	//д1��0
#define RC
	//������0

#define FAST_CALL   __attribute__((section("fastcall")))
#define FAST_ENTRY0                     __attribute__((section("aentry0"))) //main
#define FAST_ENTRY1                     __attribute__((section("aentry1"))) //FIQ
#define FAST_ENTRY2                     __attribute__((section("aentry2"))) //irq
// #define FAST_CALL   
// #define FAST_ENTRY0 
// #define FAST_ENTRY1 
// #define FAST_ENTRY2 


#endif
