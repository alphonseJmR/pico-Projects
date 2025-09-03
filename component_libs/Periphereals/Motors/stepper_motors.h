#ifndef STEPPER_MOTOR_S_H
#define STEPPER_MOTOR_S_H

#include <stdio.h>
#include <stdint.h>
#include "pico/stdlib.h"
#include "pico/time.h"
#include "hardware/pwm.h"
#include "hardware/gpio.h"
#define gpin uint8_t

/*
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
*/

//  Select stepper_motor to use.
// #define _28bjy_48
#define _nema_17


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


typedef struct _42_bygh_23_stepper_s {

    gpin coil_a_pos;
    gpin coil_a_neg;
    gpin coil_b_pos;
    gpin coil_b_neg;

    bool position_reset;

    int16_t current_rotations;
    uint16_t const_delay_speed; //  Used to pass to sleep_ms() to set motor delay speed.
    int32_t current_num_steps; //  Keep track of the number of steps taken to guage again max_rotations or minimum.
    int16_t max_rotations;     //  For setting a maximum number of rotations in the defined system.

}bygh_s;


void nema_motor_init(bygh_s *nema){

        printf("Initializing NEMA Coil Pins.\n");
    gpio_init(nema->coil_a_pos);
    gpio_init(nema->coil_a_neg);
    gpio_init(nema->coil_b_pos);
    gpio_init(nema->coil_b_neg);
    
        printf("Setting Pin Direction: OUT.\n");
    gpio_set_dir(nema->coil_a_pos, GPIO_OUT);
    gpio_set_dir(nema->coil_a_neg, GPIO_OUT);
    gpio_set_dir(nema->coil_b_pos, GPIO_OUT);
    gpio_set_dir(nema->coil_b_neg, GPIO_OUT);

    printf("GPIO Pins Initialized.\n");
}


void set_coil_A(bygh_s *nema, bool pol, bool dir){
    if(pol){
        gpio_put(nema->coil_a_pos, dir);
    }else{
        gpio_put(nema->coil_a_neg, dir);
    }
}


void set_coil_B(bygh_s *nema, bool pol, bool dir){
    if(pol){
        gpio_put(nema->coil_b_pos, dir);
    }else{
        gpio_put(nema->coil_b_neg, dir);
    }
}


void test_dir_fire(bygh_s *nema){

    set_coil_A(nema, 1, 1);
    set_coil_B(nema, 1, 1);
        sleep_us(nema->const_delay_speed);
    set_coil_A(nema, 1, 0);
    set_coil_B(nema, 1, 0);
    nema->current_num_steps++;
}


void x_steps(bygh_s *nema, uint16_t steps){
    int16_t a = 0;
    if(nema->current_num_steps < 10000){
        while(a < steps){
            test_dir_fire(nema);
            a++;
        }
    }
}

/*
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

*/
#endif


#ifdef _nema_17

//  This is the minimum step time the motor can handle
#define minimum_step_uS 1.9
//  this is intended to by used by pwm clk div
// with a wrap of 20 and level of 5, this produces a 2.5us signal.
#define minimum_clk_div 60 
#define minimum_wrap_val 20
#define minimum_level_val 5


typedef struct nema_motor_driver_pin_s {
    
  uint8_t step;
  uint8_t direction;
  uint8_t fault;
  uint8_t fan1;
  uint8_t fan2;

}nema_pins;


typedef struct nema17_pwm_config_s {

    pwm_config motor;
    uint8_t slice;
    uint8_t channel;
    uint8_t init_clk_div;
    uint16_t init_wrap;
    uint16_t init_level;
    
}nema_pwm;


typedef struct nema17_info {

    //  holds the gpio pins
    nema_pins n_pins;
    // pwm configuration
    nema_pwm pwm_cfg;

    double motor_temp;
    double drv_temp;

    bool motor_direction;
    volatile bool fan1_status;
    volatile bool fan2_status;
    //  this is active low, i believe.
    bool fault_status;
    uint16_t max_motor_temp;
    uint16_t max_drv_temp;
    uint8_t max_clk_div;

}nema_info;


void initialize_nema17_pwm(nema_info *set){

    set->pwm_cfg.slice = pwm_gpio_to_slice_num(set->n_pins.step);
    set->pwm_cfg.channel = pwm_gpio_to_channel(set->n_pins.step);
    set->pwm_cfg.motor = pwm_get_default_config();
    pwm_config_set_clkdiv_int(&set->pwm_cfg.motor, set->pwm_cfg.init_clk_div);
    pwm_config_set_wrap(&set->pwm_cfg.motor, set->pwm_cfg.init_wrap);
    pwm_init(set->pwm_cfg.slice, &set->pwm_cfg.motor, true);
    pwm_set_chan_level(set->pwm_cfg.slice, set->pwm_cfg.channel, set->pwm_cfg.init_level);
    pwm_set_enabled(set->pwm_cfg.slice, true);

}

//  Helper function to set new nema wrap value
void set_new_nema_wrap(nema_info *set, uint16_t n_wrap){
    pwm_set_wrap(set->pwm_cfg.slice, n_wrap);
}

// Helper function to set new nema levels
void set_new_nema_level(nema_info *set, uint16_t n_level){
    pwm_set_chan_level(set->pwm_cfg.slice, set->pwm_cfg.channel, n_level);
}

//  Helper function to set new nema clkdiv
void set_new_nema_clkdiv(nema_info *set, uint8_t n_clkdiv){
    pwm_set_enabled(set->pwm_cfg.slice, false);
        pwm_set_clkdiv(set->pwm_cfg.slice, n_clkdiv);
    pwm_set_enabled(set->pwm_cfg.slice, true);
}


void initialize_nema17_pins(nema_info *set){

    gpio_init(set->n_pins.step);
    gpio_init(set->n_pins.direction);
    gpio_init(set->n_pins.fault);
    gpio_init(set->n_pins.fan1);
    gpio_init(set->n_pins.fan2);

    gpio_set_function(set->n_pins.step, GPIO_FUNC_PWM);
    gpio_set_function(set->n_pins.direction, GPIO_OUT);
    gpio_set_function(set->n_pins.fault, GPIO_IN);
    gpio_pull_up(set->n_pins.fault);
    gpio_set_function(set->n_pins.fan1, GPIO_OUT);
    gpio_set_function(set->n_pins.fan2, GPIO_OUT);

    // test fan2 pin
    gpio_put(set->n_pins.fan2, true);

    initialize_nema17_pwm(set);

}


// checks if current motor temp is below max motor temp, if not, disable pwm until it is within range.
void motor_temp_check(nema_info *set){
    // percent motor temp of max
    set->motor_temp = ((set->motor_temp / set->max_motor_temp) * 100);
    printf("Motor Temp of Max: %.1f%c\n\n", set->motor_temp, '%');

    if(set->motor_temp > 25 && set->motor_temp < 50){
        printf("\tMotor Temp Surpassing 25%c Heat Rating.\n\n", '%');
    }else if(set->motor_temp > 50.00 && set->motor_temp < 75){
        printf("\tMotor Temp Surpassing 50%c Heat Rating.\n\n", '%');
    }else if(set->motor_temp > 75.00 && set->motor_temp < 85){
        printf("\tMotor Temp Surpassing 75%c Heat Rating.\n\n", '%');
    }else if(set->motor_temp > 87.5){
        printf("\t\tMotor Temp Reached Unsafe Operating Temps, cutting to prevent damage.\n");
        pwm_set_enabled(set->pwm_cfg.slice, false);
    }else{
    //    printf("Motor Operating within nominal temps.\n\n");
    }

}


void drv_temp_check(nema_info *set){
    // percent motor temp of max
    set->drv_temp = ((set->drv_temp / set->max_drv_temp) * 100);
        printf("DRV Temp of Max: %.1f%c\n\n", set->drv_temp, '%');
    if(set->drv_temp > 25.00 && set->drv_temp < 55.00){
        set->fan1_status = true;
    //    printf("Fan 1: Active\n");
    }else{
        set->fan1_status = false;
    }

    if(set->drv_temp > 50.00 && set->drv_temp < 75.00){
        set->fan2_status = true;
    //    printf("Fan 2: Active.\n");
    }else{
        set->fan2_status = false;
    }


    if(set->drv_temp > 83.3){
        printf("DRV8825 Temp Reached Unsafe Operating Temps, Cut Power to prevent damage.\n");
    }

}


void poll_nema_setup_temps(nema_info *set){
    motor_temp_check(set);
    drv_temp_check(set);
}


void update_fan_status(nema_info *set){
    poll_nema_setup_temps(set);
    gpio_put(set->n_pins.fan1, set->fan1_status);
    gpio_put(set->n_pins.fan2, set->fan2_status);
}


//  When we go to use this in out project, we want this to be an interrupt.
//  This pin is active low when set
void update_fault_status(nema_info *set){
    if(gpio_get(set->n_pins.fault)){
        set->fault_status = true;
    }else{
        set->fault_status = false;
    }
}


void update_motor_direction(nema_info *set, bool dir){
    set->motor_direction = dir;
    gpio_put(set->n_pins.direction, dir);
}

#endif

#endif