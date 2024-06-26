#ifndef INTERRUPT_STRUCT_S_H
#define INTERRUPT_STRUCT_S_H

#include <stdint.h>
#include <stdbool.h>
#define pin uint8_t
#define timed uint32_t

typedef struct input_type_s {

    struct r_en {
    const pin CLK;      //  Clk of the rotary encoder
    const pin DT;       //  Dt of the rotary encoder
    const pin REB;      //  Button of the rotary encoder

    volatile bool pre_clk;
    volatile bool pre_dt;
    volatile bool cur_clk;
    volatile bool cur_dt;

    volatile uint16_t current_value;

    const uint16_t max_rota_val;    //  The max rotation value to be set by user.

    timed r_en_initial;             //  Initial interruption time.
    timed r_en_last;                //  Last interruption time.
    timed r_en_curr;	 	    //  Current Interrupt time.
    
    timed r_en_max;		    //	The debounce value.

    }r_en;

    struct t_buttons {

    const pin b_zero;
    const pin b_one;
    const pin b_two;
    const pin b_three;
    const pin b_four;
    const pin b_five;

    volatile bool b_ze_status;
    volatile bool b_on_status;
    volatile bool b_tw_status;
    volatile bool b_th_status;
    volatile bool b_fo_status;
    volatile bool b_fi_status;
    volatile bool reb_status;

    timed initial_button;
    timed last_button;
    timed curr_button;
    
    timed max_button_interrupt;

    }t_buttons;

    uint16_t button_value;
}input_types;

#endif
