#ifndef _FAKE_CLOCK_H_
#define _FAKE_CLOCK_H_


#define FCLK_SECOND    1000

void fclk_hdl(unsigned char step);
int fclk_update_tick(unsigned int tick);
unsigned int fclk_get_tick(void);
unsigned int fclk_get_second(void);
unsigned int fclk_from_sec_to_tick(unsigned int sec);
void fclk_reset_count(void);

#endif
