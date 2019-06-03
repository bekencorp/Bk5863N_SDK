#include "fake_timer.h"

static LIST_HEADER_T fake_timer_soft_list = LIST_HEAD_INIT(fake_timer_soft_list);
static volatile unsigned int fake_timer_count = 0;
static volatile unsigned int fake_timer_count_target = 0xFFFFFFFF;

__INLINE int fake_timer_find_node(struct fake_timer_msg_handler *timer)
{
	int level;
	struct fake_timer_msg_handler *Hdl;
	LIST_HEADER_T *pos, *n;
	level = fake_timer_hw_interrupt_disable();
	
	list_for_each_safe(pos, n, &fake_timer_soft_list)
	{
		Hdl = list_entry(pos, struct fake_timer_msg_handler, hd);
		if(Hdl == timer)
		{
			goto exit;
		}
	}

	fake_timer_hw_interrupt_enable(level);
	return FAKE_TIMER_ERROR;
exit:
	fake_timer_hw_interrupt_enable(level);
	return FAKE_TIMER_OK;
}

__INLINE int fake_timer_register(struct fake_timer_msg_handler *timer)
{
	int level;
	int first;
	struct fake_timer_msg_handler *Hdl;
	LIST_HEADER_T *pos, *n;
	
	level = fake_timer_hw_interrupt_disable();
	if(list_empty(&fake_timer_soft_list))
	{
		list_add_head(&timer->hd,&fake_timer_soft_list);
		timer->timeout_tick = timer->target_tick;
		fake_timer_count = 0;
		fake_timer_count_target = timer->timeout_tick;
	}
	else
	{
		first = 0;
		list_for_each_safe(pos, n, &fake_timer_soft_list)
		{
			Hdl = list_entry(pos, struct fake_timer_msg_handler, hd);
			if(Hdl == timer)
				continue;
			Hdl->timeout_tick = (Hdl->timeout_tick > fake_timer_count)?(Hdl->timeout_tick - fake_timer_count):0;
			if(Hdl->timeout_tick >= timer->target_tick)
			{
				if(first == 0)
				{
					first = 1;
					if(Hdl->timeout_tick == timer->target_tick)
						list_add_tail(&timer->hd,Hdl->hd.next);
					else
						list_add_head(&timer->hd,Hdl->hd.prev);
					timer->timeout_tick = timer->target_tick;
				}
			}
		}
		if(first == 0)
		{
				list_add_tail(&timer->hd,&fake_timer_soft_list);
				timer->timeout_tick = timer->target_tick;
		}
		Hdl = list_entry(fake_timer_soft_list.next, struct fake_timer_msg_handler, hd);
		fake_timer_count = 0;
		fake_timer_count_target = Hdl->timeout_tick;
	}
	
	fake_timer_hw_interrupt_enable(level);
	return FAKE_TIMER_OK;
}

__INLINE int fake_timer_unregister(struct fake_timer_msg_handler *timer)
{
	struct fake_timer_msg_handler *Hdl;
	LIST_HEADER_T *pos, *n;
	int level;
	
	level = fake_timer_hw_interrupt_disable();
	if(list_empty(&fake_timer_soft_list))
		goto exit;

	Hdl = list_entry(fake_timer_soft_list.next, struct fake_timer_msg_handler, hd);
	if(Hdl == timer)
	{
		Hdl = list_entry(fake_timer_soft_list.next, struct fake_timer_msg_handler, hd);
		Hdl->run = 0;
		
		list_del(&Hdl->hd);
		if(list_empty(&fake_timer_soft_list))
		{
			fake_timer_count = 0;
			fake_timer_count_target = FAKE_TIMER_TICK_MAX;
		}
		else
		{
			Hdl = list_entry(fake_timer_soft_list.next, struct fake_timer_msg_handler, hd);
			fake_timer_count_target = Hdl->target_tick;
		}
	}
	else
	{
		list_for_each_safe(pos, n, &fake_timer_soft_list)
		{
			Hdl = list_entry(pos, struct fake_timer_msg_handler, hd);
			if(Hdl == timer)
			{
				list_del(pos);
				Hdl->hd.next = &Hdl->hd;
				Hdl->hd.prev = &Hdl->hd;
				Hdl->run = 0;
				goto exit;
			}
		}
		goto exit2;
	}

exit:
	fake_timer_hw_interrupt_enable(level);
	return FAKE_TIMER_OK;
exit2:
	fake_timer_hw_interrupt_enable(level);
	return FAKE_TIMER_ERROR;
}

__INLINE int fake_timer_find_next_timer(void)
{
	int level;
	struct fake_timer_msg_handler *Hdl;
	
	level = fake_timer_hw_interrupt_disable();
	if(list_empty(&fake_timer_soft_list))
	{
		fake_timer_count = 0;
		fake_timer_count_target = FAKE_TIMER_TICK_MAX;
	}
	else
	{
		Hdl = list_entry(fake_timer_soft_list.next, struct fake_timer_msg_handler, hd);
		fake_timer_count = 0;
		fake_timer_count_target = Hdl->timeout_tick;
	}
	
	fake_timer_hw_interrupt_enable(level);
	return FAKE_TIMER_OK;
}


void fake_timer_handler(unsigned char step)
{
	int level;
	struct fake_timer_msg_handler *Hdl;
	if(fake_timer_count_target != FAKE_TIMER_TICK_MAX)
	{
		fake_timer_count += step;
	re:
		if(fake_timer_count >= fake_timer_count_target)
		{
			if(!list_empty(&fake_timer_soft_list))
			{
				level = fake_timer_hw_interrupt_disable();
				Hdl = list_entry(fake_timer_soft_list.next,struct fake_timer_msg_handler, hd);
				fake_timer_hw_interrupt_enable(level);
				if(Hdl->cb)
				{
					Hdl->timeout_tick = Hdl->target_tick;
					Hdl->cb(Hdl->arg);
				}
				level = fake_timer_hw_interrupt_disable();
				list_del(&Hdl->hd);
				Hdl->hd.next = &Hdl->hd;
				Hdl->hd.prev = &Hdl->hd;
				fake_timer_hw_interrupt_enable(level);
				if((Hdl->repetition)&&(Hdl->run))
				{
					fake_timer_register(Hdl);
				}
				else
				{
					fake_timer_find_next_timer();
				}
				goto re;
			}
			else
			{
				fake_timer_count_target = FAKE_TIMER_TICK_MAX;
				fake_timer_count = 0;
			}
		}
	}
}


int fake_timer_create(struct fake_timer_msg_handler *timer,
												void (*timeout)(void *parameter),
												void      *parameter,
												unsigned int  time,
												char flag)
{
	if(timer == NULL)
		return FAKE_TIMER_ERROR;
	
	timer->arg = parameter;
	timer->target_tick = time;
	timer->timeout_tick = 0;
	timer->cb = timeout;
	timer->hd.next = &timer->hd;
	timer->hd.prev = &timer->hd;
	timer->run = 0;
	if(flag)
		timer->repetition = 1;
	else
		timer->repetition = 0;
	
	return FAKE_TIMER_OK;
}

int fake_timer_delete(struct fake_timer_msg_handler *timer)
{
	int ret;
	if(timer == NULL)
		return FAKE_TIMER_ERROR;
	
	if((timer->hd.next == &timer->hd) || (timer->hd.prev == &timer->hd))
	{
		return FAKE_TIMER_OK;
	}
	else
	{
		ret = fake_timer_unregister(timer);
		if(ret != FAKE_TIMER_OK)
		{
			return FAKE_TIMER_ERROR;
		}
		timer->hd.next = &timer->hd;
		timer->hd.prev = &timer->hd;
	}
	
	return FAKE_TIMER_OK;
}

int fake_timer_start(struct fake_timer_msg_handler *timer)
{
	if(timer == NULL)
		return FAKE_TIMER_ERROR;

	if(fake_timer_find_node(timer) == FAKE_TIMER_ERROR)
	{
		timer->hd.next = &timer->hd;
		timer->hd.prev = &timer->hd;
		timer->run = 1;
		fake_timer_register(timer);
	}

	return FAKE_TIMER_OK;
}

int fake_timer_stop(struct fake_timer_msg_handler *timer)
{
	if(timer == NULL)
		return FAKE_TIMER_ERROR;
	
	if((timer->hd.next == &timer->hd) || (timer->hd.prev == &timer->hd))
	{
		return FAKE_TIMER_OK;
	}
	else
	{
		if(fake_timer_find_node(timer) == FAKE_TIMER_OK)
			fake_timer_unregister(timer);
	}
	
	return FAKE_TIMER_OK;
}

int fake_timer_init(struct fake_timer_msg_handler *timer,
												void (*timeout)(void *parameter),
												void      *parameter,
												unsigned int  time,
												char flag)
{
	return fake_timer_create(timer,timeout,parameter,time,flag);
}

int fake_timer_deinit(struct fake_timer_msg_handler *timer)
{
	return fake_timer_delete(timer);
}

int fake_timer_control(fake_timer_t timer, int cmd, void *arg)
{
    /* timer check */
	if(timer == NULL)
	{
		return FAKE_TIMER_ERROR;
	}

	switch (cmd)
	{
		case FAKE_TIMER_CTRL_GET_TIME:
			*(int *)arg = timer->target_tick;
			break;

		case FAKE_TIMER_CTRL_SET_TIME:
			timer->target_tick = (*(int *)arg) & FAKE_TIMER_TICK_MAX;
			break;

		case FAKE_TIMER_CTRL_SET_ONESHOT:
			timer->repetition = 0;
			break;

		case FAKE_TIMER_CTRL_SET_PERIODIC:
			timer->repetition = 1;
			break;
	}

	return FAKE_TIMER_OK;
}



/***********************  END OF FILES  ***********************/
