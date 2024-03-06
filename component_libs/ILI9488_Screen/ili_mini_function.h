#ifndef ILI9488_MINI_FUNCTIONS
#define ILI9488_MINI_FUCNTIONS

#include <stdio.h>

// Create tick counter, in ms.
volatile uint32_t tick_count;

void tick_inc(void){
    tick_count++;
}

void s_wait(volatile uint32_t time_ms){
    uint32_t t_start;
    uint32_t t_current;

    t_start = time_ms;
        do {
            t_current = time_ms;
        }while (t_current - t_start < time_ms){
            tick_inc();
        }
}

#endif