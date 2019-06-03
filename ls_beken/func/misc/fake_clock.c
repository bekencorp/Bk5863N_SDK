#include "fake_clock.h"

static volatile unsigned int current_clock = 0;
static volatile unsigned int current_seconds = 0;
static short second_countdown = FCLK_SECOND;

void fclk_hdl(unsigned char step)
{
    current_clock += step;
		second_countdown -= step;
    if (second_countdown <= 0)
    {
        current_seconds ++;
        second_countdown = FCLK_SECOND;
    }
}


int fclk_update_tick(unsigned int tick)
{
    current_clock += tick;

    if(second_countdown <= tick)
    {
        current_seconds ++;
        second_countdown = FCLK_SECOND - (tick - second_countdown);
    }
    else
    {
        second_countdown -= tick;
    }

    return 0;
}

unsigned int fclk_get_tick(void)
{
    return current_clock;
}

unsigned int fclk_get_second(void)
{
    return current_seconds;
}

unsigned int fclk_from_sec_to_tick(unsigned int sec)
{
    return sec * FCLK_SECOND;
}

void fclk_reset_count(void)
{
    current_clock = 0;
    current_seconds = 0;
}



void fclk_init(void)
{

}

// eof

