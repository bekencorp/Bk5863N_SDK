#ifndef _FAKE_TIMER_H_
#define _FAKE_TIMER_H_

#include "doubly_list.h"
#ifndef NULL
#define NULL   0U
#endif


#define FAKE_TIMER_OK               (0)
#define FAKE_TIMER_ERROR            (-1)

#define FAKE_TIMER_UINT8_MAX                    0xff            /**< Maxium number of UINT8 */
#define FAKE_TIMER_UINT15_MAX                   0x7fff          /**< Maxium number of UINT16 */
#define FAKE_TIMER_UINT16_MAX                   0xffff          /**< Maxium number of UINT16 */
#define FAKE_TIMER_UINT32_MAX                   0xffffffff      /**< Maxium number of UINT32 */
#define FAKE_TIMER_TICK_MAX                     FAKE_TIMER_UINT15_MAX   /**< Maxium number of tick */

enum{
	FAKE_TIMER_CTRL_GET_TIME,
	FAKE_TIMER_CTRL_SET_TIME,
	FAKE_TIMER_CTRL_SET_ONESHOT,
	FAKE_TIMER_CTRL_SET_PERIODIC,
};

typedef void (*fake_timer_func_t)(void *arg);

struct fake_timer_msg_handler{
	LIST_HEADER_T hd;
	unsigned repetition : 1;
	unsigned run : 1;
//	unsigned run_main : 1;     ////ÔÚmainÖÐÖ´ÐÐ
	unsigned target_tick : 15;
	unsigned timeout_tick : 15;
	
	fake_timer_func_t cb;
	void *arg;
};

typedef struct fake_timer_msg_handler *fake_timer_t;

__INLINE int fake_timer_hw_interrupt_disable(void)
{
	int level = 0;
	
	level = (*((volatile unsigned long *)   0x00a03084));
	(*((volatile unsigned long *)   0x00a03084)) = 0;
	return level;
}

__INLINE void fake_timer_hw_interrupt_enable(int level)
{
	(*((volatile unsigned long *)   0x00a03084)) = level;
}


void fake_timer_handler(unsigned char step);
int fake_timer_create(struct fake_timer_msg_handler *timer,
												void (*timeout)(void *parameter),
												void      *parameter,
												unsigned int  time,
												char flag);
int fake_timer_delete(struct fake_timer_msg_handler *timer);
int fake_timer_start(struct fake_timer_msg_handler *timer);
int fake_timer_stop(struct fake_timer_msg_handler *timer);
int fake_timer_init(struct fake_timer_msg_handler *timer,
												void (*timeout)(void *parameter),
												void      *parameter,
												unsigned int  time,
												char flag);
int fake_timer_deinit(struct fake_timer_msg_handler *timer);
int fake_timer_control(fake_timer_t timer, int cmd, void *arg);

#endif

/***********************  END OF FILES  ***********************/
