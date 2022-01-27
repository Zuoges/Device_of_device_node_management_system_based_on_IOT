#ifndef _GPIO_H_
#define _GPIO_H_

extern int relay_state;

void gpio_init();
void relay_on();
void relay_off();

#endif