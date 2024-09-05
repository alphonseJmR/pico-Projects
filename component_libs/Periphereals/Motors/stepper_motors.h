#ifndef STEPPER_MOTOR_S_H
#define STEPPER_MOTOR_S_H

#include <stdio.h>
#include <stdint.h>
#include "pico/stdlib.h"
#include "pico/time.h"
#include "pico/multicore.h"
#include "hardware/gpio.h"
#define gpin uint8_t


volatile uint32_t fifo_buffer;

bool fifo_pop(void){
    volatile bool out;
    out = false;
        printf("Popping Fifo.\n");
    fifo_buffer = multicore_fifo_pop_blocking();
    //  printf("Fifo Value: %i.\n", fifo_buffer);
    out = (bool)(fifo_buffer & 0x01);
    printf("Out Bool: %i.\n", (out) ? 1 : 0);
    return out;
}


void fifo_res(void){
  multicore_fifo_push_blocking(0x00);
}


//  Select stepper_motor to use.
#define _28bjy_48


#ifdef _28bjy_48
//  Definition of # of steps required for one full rotation.
#define full_rotation 2048
#define half_rotation 1024

const uint16_t _30_deg_rotation[12] = {
    171,
    342,
    513,
    684,
    855,
    1026,
    1197,
    1368,
    1539,
    1710,
    1881,
    2048
};
const uint16_t _60_deg_rotation[7] = {
    342,
    684,
    1026,
    1368,
    5130, // previous value 1710 (8/10/24) known working value
    2048
};
const uint16_t _90_deg_rotation[5] = {
    513,
    1026,
    1539,
    2048
};

//  Defined pattern for directional rotation.
/*
        Easier to see the pattern this way.
        Clockwise going down the pattern.
        Counter Clockwise going up the pattern
        -------------------------------------
        Coil_A1 | Coil_A2 | Coil_B1 | Coil_B2
        -------------------------------------
 Step 1 |  On       Off       On       Off
 Step 2 |  Off      On        On       Off
 Step 3 |  Off      On        Off      On
 Step 4 |  On       Off       Off      On
        -------------------------------------
*/
//  0xA659
//  0x965A

// #define clockwise 0x96C3
// #define counter_clockwise 0x3C69

//  This defined variable is 4, as sending clockwise or counter clockwise pattern is technically 4 steps.
//  We don't want to send full rotation, 4 times.  So full rotation technically requires 1/4 of the defined number.
#define pattern_correction 8        

typedef struct _28bjy_48_stepper {

    gpin coil_a;
    gpin coil_b;
    gpin coil_c;
    gpin coil_d;

    bool position_reset;
    bool last_direction;

    bool cw_edge;
    bool ccw_edge;
    uint8_t current_rotations;
    uint8_t pat_slice[4];       //  Variable to store the pattern into usable bytes.
    uint16_t const_delay_speed; //  Used to pass to sleep_ms() to set motor delay speed.
    int32_t current_num_steps; //  Keep track of the number of steps taken to guage again max_rotations or minimum.
    uint32_t max_rotations;     //  For setting a maximum number of rotations in the defined system.

}bjy48;

void motor_initilization(bjy48 *pins){

        printf("Initializing Coil Pins.\n");
    gpio_init(pins->coil_a);
    gpio_init(pins->coil_b);
    gpio_init(pins->coil_c);
    gpio_init(pins->coil_d);
    
        printf("Setting Pin Direction: OUT.\n");
    gpio_set_dir(pins->coil_a, GPIO_OUT);
    gpio_set_dir(pins->coil_b, GPIO_OUT);
    gpio_set_dir(pins->coil_c, GPIO_OUT);
    gpio_set_dir(pins->coil_d, GPIO_OUT);

    printf("GPIO Pins Initialized.\n");
}

void pattern_slice(bjy48 *set, bool direction){

    (direction) ? printf("Clockwise Direction.\n") : printf("Counter Clockwise Direction.\n");
    if(direction){
        set->pat_slice[0] = 0x07;
        set->pat_slice[1] = 0x1C;
        set->pat_slice[2] = 0x70;
        set->pat_slice[3] = 0xC1;
    }else {
        set->pat_slice[0] = 0xC1;
        set->pat_slice[1] = 0x70;
        set->pat_slice[2] = 0x1C;
        set->pat_slice[3] = 0x07;
    }

}

//  cwccw = Directional.  True == clockwise, False = Counter Clockwise.
//  Step function has pattern correction intergrated.  Full rotation is 2048 steps.  "One" step is equal to 8/2048
//  rotation_speed integrates into sleep_ms().  Minimum is 10 (maybe 11)
void step(bjy48 *motor, uint16_t steps, bool cwccw, uint16_t rotation_speed){
    
    pattern_slice(motor, cwccw);
        printf("Stepping %i times.\n", (steps / pattern_correction));

    for(uint16_t a = 0; a <= (steps / pattern_correction); a++){
    //    printf("Stepping %i times.\n", ((steps / pattern_correction) - a));

        for(int8_t b = 8; b >= 0; b--){

        //    printf("Coil A: 0x%02x.\n", (motor->pat_slice[0] >> b));
        gpio_put(motor->coil_a, (motor->pat_slice[0] >> b) & 0x01);
        //    printf("Coil B: 0x%02x.\n", (motor->pat_slice[1] >> b));
        gpio_put(motor->coil_b, (motor->pat_slice[1] >> b) & 0x01);
        //    printf("Coil C: 0x%02x.\n", (motor->pat_slice[2] >> b));
        gpio_put(motor->coil_c, (motor->pat_slice[2] >> b) & 0x01);
        //    printf("Coil D: 0x%02x.\n", (motor->pat_slice[3] >> b));
        gpio_put(motor->coil_d, (motor->pat_slice[3] >> b) & 0x01);
            sleep_ms(rotation_speed);
            if(cwccw){
                motor->current_num_steps += 1;
            }else{
                motor->current_num_steps -= 1;
            }

        //    printf("Steps Taken: %i.\n", motor->current_num_steps);
        }
       }
      }

bool turret_step(bjy48 *motor, uint16_t steps, bool cwccw, uint16_t rotation_speed, bool val_rx){
    
    bool popped;
    bool set_return;
    pattern_slice(motor, cwccw);
        printf("Stepping %i times.\n", (steps / pattern_correction));
    for(uint16_t a = 0; a <= (steps / pattern_correction); a++){

    if( val_rx ){
        set_return = true;
        break;
    }else{
      for(int8_t b = 8; b >= 0; b--){

          gpio_put(motor->coil_a, (motor->pat_slice[0] >> b) & 0x01);
          gpio_put(motor->coil_b, (motor->pat_slice[1] >> b) & 0x01);
          gpio_put(motor->coil_c, (motor->pat_slice[2] >> b) & 0x01);
          gpio_put(motor->coil_d, (motor->pat_slice[3] >> b) & 0x01);
            sleep_ms(rotation_speed);
          if(cwccw){
            motor->current_num_steps += 1;
          }else{
            motor->current_num_steps -= 1;
          }
         } 
    }
       }
    return set_return;
      }

void turret_position_reset(bjy48 *motor, uint16_t steps, bool cwccw, uint16_t rotation_speed){
    
    pattern_slice(motor, cwccw);
        printf("Stepping %i times.\n", (steps / pattern_correction));
    for(uint16_t a = 0; a <= (steps / pattern_correction); a++){
      for(int8_t b = 8; b >= 0; b--){

          gpio_put(motor->coil_a, (motor->pat_slice[0] >> b) & 0x01);
          gpio_put(motor->coil_b, (motor->pat_slice[1] >> b) & 0x01);
          gpio_put(motor->coil_c, (motor->pat_slice[2] >> b) & 0x01);
          gpio_put(motor->coil_d, (motor->pat_slice[3] >> b) & 0x01);
            sleep_ms(rotation_speed);
          if(cwccw){
            motor->current_num_steps += 1;
          }else{
            motor->current_num_steps -= 1;
          }
        } 
    }
}

//  Angle Size is 0-2 (30,60,90), and degs_to_turn available sizes change.
//  30_degs = (0-11), 60_degs = (0-6), 90_degs = (0-4)
void step_by_angle(bjy48 *motor, uint8_t angle_size, uint8_t degs_to_turn, bool dir){

    switch(angle_size){
        case 0:
            printf("Stepping By 30 degree increments.\n");
            step(motor, _30_deg_rotation[degs_to_turn], dir, motor->const_delay_speed);
        break;

        case 1:
            printf("Stepping By 60 degree increments.\n");
            step(motor, _60_deg_rotation[degs_to_turn], dir, motor->const_delay_speed);
        break;

        case 2:
            printf("Stepping By 90 degree increments.\n");
            step(motor, _90_deg_rotation[degs_to_turn], dir, motor->const_delay_speed);
        break;

        default:
            printf("Valid values are currently 0-2.\n");
        break;
    }

}

bool turret_stepping(bjy48 *motor, bool core_rx){
        printf("Turrent Stepping.\n");
    bool func_test;
    func_test = false;

while(!motor->position_reset){
    printf("Resetting From Last Position.\n");

   if(motor->last_direction){
    turret_position_reset(motor, motor->current_num_steps, false, 3);
    motor->position_reset = true;
   }else{
    turret_position_reset(motor, (4096 - motor->current_num_steps), true, 3);
    motor->position_reset = true;
   }
    fifo_res();
}
while(!func_test){
        printf("Normal Stepping.\n");
    motor->last_direction = true;
    func_test = turret_step(motor, 4096, true, 3, core_rx);
        if(func_test){
            printf("Breaking Positive Turning\n");
        motor->position_reset = false;
            break;
        }
    motor->last_direction = false;
    func_test = (motor, 4096, false, 3, core_rx);
        if(func_test){
            printf("Breaking Negative Turning\n");
        motor->position_reset = false;
            break;
        }
  }

  return func_test;
}
#endif

#endif