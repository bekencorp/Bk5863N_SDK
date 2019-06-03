#ifndef _GERNEL_H_
#define _GERNEL_H_

#ifndef __INLINE 
#define  __INLINE static inline
#endif

#ifndef GLOBAL_INT_DECLARATION
#define GLOBAL_INT_DECLARATION()   unsigned int fiq_tmp, irq_tmp
#define GLOBAL_INT_DISABLE()       do{\
										                  fiq_tmp = __disable_fiq();\
										                  irq_tmp = __disable_irq();\
									                 }while(0)


#define GLOBAL_INT_RESTORE()       do{                         \
                                        if(!fiq_tmp)           \
                                        {                      \
                                            __enable_fiq();    \
                                        }                      \
                                        if(!irq_tmp)           \
                                        {                      \
                                            __enable_irq();    \
                                        }                      \
                                   }while(0)
#endif
																	 
																	 
//#undef  ASSERT
//#ifndef ASSERT
#define as_printf (printf("%s:%d\r\n",__FUNCTION__,__LINE__))
#define ASSERT(exp)                                 \
{                                                   \
    if ( !(exp) )                                   \
    {                                               \
        as_printf;							     	\
        while(1);                                   \
    }                                               \
} 
//#endif

#define ASSERT_ERR(cond)                  ASSERT(cond)

#endif
