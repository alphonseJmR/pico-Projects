#ifndef SERVO_MOTOR_S_H
#define SERVO_MOTOR_S_H

#define pin uint8_t
#define position uint16_t


// #define _RC_CAR_SERVO_
// #define _SG90_SERVO_
// #define _NEMA_17_SERVO_
#define _GENERIC_SERVO_

#include <stdio.h>
#include <stdint.h>
#include "../resources/pico_pin_enum.h"
#include "../../hi_lvl_resources/project_struct_s.h"
#include "hardware/gpio.h"
#include "hardware/pwm.h"

#ifdef _RC_CAR_SERVO_

#define input_Test(a, b, c) ((bool)( ( ((uint16_t)a <= (uint16_t)c ) && ((uint16_t)b >= (uint16_t)c) )))
#define idle_return(a, b, c) (uint16_t)( (((uint16_t)a >= b) && ((uint16_t)a <= c) ? 505 : a))

uint16_t hori_testers[12] = {

  0x0000, 0x0038,
  0x0039, 0x00CA,
  0x00CB, 0x01DB,
  0x0213, 0x0296,
  0x0297, 0x0328,
  0x0329, 0x0400

};

uint16_t ms18_turn_degs[6] = {

0x0248,
0x036C,
0x03FE,
0x076A,
0x088E,
0x09B2

};

uint16_t servo_turn_table[6] = {

  0x0474,
  0x06AE,
  0x07CC,
  0x0B23,
  0x0E7A,
  0x10B5

};

void set_servo_position(uint8_t pin_selection, servo_motor *moto, uint16_t position){
  
   if(pin_selection == moto->my_perv.servo_pin_one){

    pwm_set_gpio_level(pin_selection, position);
  //    printf("Servo Motor Position: %.\n", position);

  }

  if(pin_selection == moto->my_perv.servo_pin_two){   

      pwm_set_gpio_level(pin_selection, position);  

  }

}

void new_servo_turn(uint8_t pin, uint16_t pos){

  if(pos != 2851){
    set_servo_position(pin, &my_servo, pos);
  }else {
    set_servo_position(pin, &my_servo, 2851);
  }

}

void set_servo_initial_position(servo_motor *config){

  if(config->my_perv.servo_pin_one != UNDEFINED){
      pwm_set_gpio_level(config->my_perv.servo_pin_one, 2851);
      printf("Servo Motor One Initialized to: %li.\n", 2851);
  }
  
  if(config->my_perv.servo_pin_two != UNDEFINED){
      pwm_set_gpio_level(config->my_perv.servo_pin_two, 2851);
      printf("Servo Motor Two Initialized to: %li.\n", 2851);
    }

}

void servo_initialization(servo_motor *configuration){

  printf("\n////\tServo Motor Initialisation\t////\n");

  pwm_config config = pwm_get_default_config();

  if(configuration->my_perv.servo_pin_one != UNDEFINED){

    gpio_set_function(configuration->my_perv.servo_pin_one, GPIO_FUNC_PWM);
      printf("Initializing Servo Pin: %i.\n", configuration->my_perv.servo_pin_one);
    configuration->my_serv.servo_slice_one = pwm_gpio_to_slice_num(configuration->my_perv.servo_pin_one);
      printf("Servo Slice: %i.\n", configuration->my_serv.servo_slice_one);
    pwm_config_set_clkdiv(&config, 64.f);
    pwm_config_set_wrap(&config, configuration->my_serv.servo_wrap_one);
    pwm_init(configuration->my_serv.servo_slice_one, &config, true);
    
    printf("////\tServo One: Initialised\t////\n");
    
  }else {
    printf("Servo Motor One:\tNot Initialized.\n");
  }

  if(configuration->my_perv.servo_pin_two != UNDEFINED){

    gpio_set_function(configuration->my_perv.servo_pin_two, GPIO_FUNC_PWM);
    configuration->my_serv.servo_slice_two = pwm_gpio_to_slice_num(configuration->my_perv.servo_pin_two);
    pwm_config_set_clkdiv(&config, 64.f);
    pwm_config_set_wrap(&config, configuration->my_serv.servo_wrap_two);
    pwm_init(configuration->my_perv.servo_pin_two, &config, true);

    printf("////\tServo Two: Initialised\t////\n");
  }else {
    printf("Servo Motor Two:\tNot Initialized.\n");
  }

/*
  if(configuration->servo_three_pin != -1){

    gpio_set_function(configuration->servo_three_pin, GPIO_FUNC_PWM);
    configuration->servo_three_slice = pwm_gpio_to_slice_num(configuration->servo_three_pin);
    pwm_config_set_clkdiv(&config, configuration->stt_clkdiv);
    pwm_config_set_wrap(&config, configuration->stt_wrap);
    pwm_init(configuration->servo_three_pin, &config, true);
  }else {
    printf("Servo Motor Three:\tNot Initialized.\n");
  }
  if(configuration->servo_four_pin != -1){

    gpio_set_function(configuration->servo_four_pin, GPIO_FUNC_PWM);
    configuration->servo_four_slice = pwm_gpio_to_slice_num(configuration->servo_four_pin);
    pwm_config_set_clkdiv(&config, configuration->sf_clkdiv);
    pwm_config_set_wrap(&config, configuration->sf_wrap);
    pwm_init(configuration->servo_four_pin, &config, true);
  }else {
    printf("Servo Motor Four:\tNot Initialized.\n");
  }

*/

  set_servo_initial_position(configuration);
}

uint16_t servo_pre_calc(uint16_t hori_data, uint16_t *tabb){

  bool test_status;
    test_status = false;
  uint16_t buffered_degree;
    buffered_degree = 0;

  size_t check_size = ( (sizeof(hori_testers) / sizeof(uint16_t)) );
//  printf("Size of Hori_checker: %i.\n", check_size);

//    printf("\nRaw Horizontal Data: %i.\n", hori_data);

  test_status = input_Test(480, 525, hori_data);

    if(test_status){
    
      buffered_degree = 2851;
      test_status = false;

    }else{

    for(int i = 0; i < check_size; i += 2){

      test_status = input_Test(hori_testers[i], hori_testers[i+1], hori_data);
        if(test_status){

          buffered_degree = tabb[(i/2)];
          printf("Current Servo Output: %i\n", buffered_degree);
          i = check_size;

        }else{

          printf("Unmatched.\n");


        }
    }
  }

  return buffered_degree;

}

uint16_t set_servo_turn(servo_motor *servo, volatile uint16_t horizontal_data, uint16_t prev_pos){

//  printf("\n////\tSet Servo Turn\t////\n");
  printf("///\tPrevious Position: %i.\t///\n", prev_pos);

  bool idle_turn;
  idle_turn = false;
    //  Check if input is idle value.
  idle_turn = input_Test(1400, 1500, horizontal_data);
  
  if(!idle_turn){
    (!idle_turn) ? printf("Servo Turning.\n") :  printf("Servo Turn Idle\n");

  set_servo_position(10, servo, horizontal_data);
//    printf("Re-Enabling Servo PWM");
  pwm_set_enabled(servo->my_serv.servo_slice_one, true);
      sleep_us(50);

  }else{

        set_servo_position(10, servo, 2851);
        //  pwm_set_counter(my_servo.my_serv.servo_slice_one, 0);
        sleep_us(50);
        pwm_set_enabled(servo->my_serv.servo_slice_one, false);

  }

  return prev_pos;
}

#endif


#ifdef _SG90_SERVO_

//  Pulse sizes as defined by datasheet.
//  90DEG  = ~2ms pulse   ~~10% duty cycle
//  0DEG   = 1.5ms pulse  ~~7.5% duty cylce
//  -90DEG = ~1ms pulse   ~~5% duty cycle

//  50Hz PWM Period.


#define not_max(a,b) (bool)(a < b)
#define not_min(a,b) (bool)(a > b)
#define testing(a,b,c) (bool)((a >= b) && (a <= c))

uint16_t ctp;

/*
  These values are defined by using input values of 400-2000 (100 increments)
  Inputted into ((x/20000)*39062)
*/
const uint16_t turn_values[17] = {
781,  // 1
976,
1171, // 2
1367,
1562, //  min turret position
1757,
1953, //4
2148,
2343, //  center position
2539,
2734, //6
2929,
3124, //  max turret position
3320,
3515, //8 
3710,
3906
};

const uint16_t turret_tilt_angle[9] = {

1562, //  min turret position // -60deg
1757, //  -45deg
1953, //  -30deg
2148, //  -15deg
2343, //  center position
2539, //  15deg
2734, //  30deg
2929, //  45deg
3124, //  max turret position // 60deg

};

//  Turret Tilt Angle
const uint16_t tta[9] = {
1300, //  Min Position
1400,
1500,
1600,
1700,
1800,
1900,
2000,
2100 // Max position?
};

typedef struct sg90_servo_s {

pin signal;  // This is the output pin.
const position center;  //  going with 2343?
position min; //  1562
position max; //  3124

float clk_div;        //  For SG90 we'll set this as 64.f
uint16_t period_div;  //  This will be set to 39062

uint8_t m_slice;

}sg90;

void center_servo(sg90 *servo){

  printf("\tCentering Servo Motor.\n");

  pwm_set_gpio_level(servo->signal, servo->center);

  printf("\tServo Centered.\n");
}

/*! \brief
  Initialise SG90 Servo Motor.

  *\param setup Struct containing servo variables
  
  \note 
    Sets up:
  \note
    Pin Function
  \note
    Slice Num
  \note
    Clk Div
  \note
    Wrap
*/
void initialize_servo(sg90 *setup){
  pwm_config config = pwm_get_default_config();

    printf("Initializing Servo with Pin: %i.\n", setup->signal);
  gpio_set_function(setup->signal, GPIO_FUNC_PWM);
  setup->m_slice = pwm_gpio_to_slice_num(setup->signal);
  pwm_config_set_clkdiv(&config, setup->clk_div);
  pwm_config_set_wrap(&config, setup->period_div);

  pwm_init(setup->m_slice, &config, true);
    center_servo(setup);
  printf("SG90 Servo Motor Initialized.\n");
}

void servo_test(sg90 *test){

  printf("Incrementing Direction.\n");
  for(uint8_t a = 0; a < 17; a++){
      printf("Setting Servo Duty Cycle: %i.\n", turn_values[a]);
    pwm_set_gpio_level(test->signal, turn_values[a]);
      sleep_ms(1000);
  }

  printf("Decrementing Direction.\n");
  for(uint8_t a = 16; a > 0; a--){
      printf("Setting Servo Duty Cycle: %i.\n", turn_values[a]);
    pwm_set_gpio_level(test->signal, turn_values[a]);
      sleep_ms(1000);
  }
}

void set_servo_position(sg90 *motor, uint16_t positional){

  pwm_set_gpio_level(motor->signal, positional);

}

//  For turret program.

/*! \brief
  Used to find turret angle from analog input value.
 * \param input_value 16bit recieved mapped ADC input.
 * \param t_side Bool to set turret servos for opposite values.
 * 
 * \return Num value to set turret_tilt_angle
*/
uint8_t find_turret_angle(uint16_t input_value){

    printf("\nInput Position Value: 0x%04x.\n", input_value);

  if(testing(input_value, 0x79, 0x86)){
      printf("IDLE SETTING FOUND.\n");
    return 4;

  }else{

    if(testing(input_value, 0, 30)){
      return 0;
    }else if(testing(input_value, 31, 60)){
      return 1;
    }else if(testing(input_value, 61, 92)){
      return 2;
    }else if(testing(input_value, 93, 120)){
      return 3;
    }else if(testing(input_value, 135, 162)){
      return 5;
    }else if(testing(input_value, 163, 193)){
      return 6;
    }else if(testing(input_value, 194, 224)){
      return 7;
    }else if(testing(input_value, 225, 255)){
      return 8;
    }
  }
}

void angle_turret(sg90 *angle, uint16_t analog){

    uint8_t set_it = 0;
      printf("Setting Turret Angle.\n");

    set_it = find_turret_angle(analog);
      printf("Currently Set at: %i.\n", tta[set_it]);
  set_servo_position(angle, tta[set_it]);

}

void swivel_turret(sg90 *swivel, uint16_t input){

  switch(input){ //  Checking for left turn input value.
    case 1:
      if(not_min(ctp, turn_values[0])){ //  Checking we're not at minimum value.
        set_servo_position(swivel, ctp);
        ctp -= 5;
          printf("Current CTP Value: %i.\n", ctp);
      }else{
        printf("At Minimum rotational value.\n");
          ctp = (turn_values[0] + 5);
      }
    break;

    case 2:
      if(not_max(ctp, turn_values[16])){
        set_servo_position(swivel, ctp);
        ctp += 5;
          printf("Current CTP Value: %i.\n", ctp);
      }else{
        printf("At Maximum rotational value.\n");
          ctp = (turn_values[16] - 5);
      }
    break;

    default:
      break;
  }
}


#endif


#ifdef _NEMA_17_SERVO_

typedef struct nema17_pin_s {

  uint8_t step;
  uint8_t dir;
  uint8_t fault;

}nema_pins;

//  Struct Insts

// This is my base driver (for name simplicity sake)
nema_pins driver;
//  To check drv8825 fault line
repeating_timer_t fault_check;


nema_pins set_driver_pins(nema_pins *in, uint8_t step, uint8_t direction, uint8_t fault){

  in->step = step;
  in->dir = direction;
  in->fault = fault;

  return *in;

}

#endif


#ifdef _GENERIC_SERVO_

typedef struct generic_servo_s {
  uint8_t signal_pin;
  uint8_t pwm_slice;
  uint8_t pwm_chan;
  uint16_t center_pos;
  uint16_t min_pos;
  uint16_t max_pos;
  float clk_div;
  float duty_cycle;
}gen_servo;

//  Helper function to set motor back to center pos
void center_servo(gen_servo *motor){

    printf("\n\tCentering Servo Motor.\n");
  pwm_set_gpio_level(motor->signal_pin, motor->center_pos);
    printf("\tServo Centered.\n\n");
}

//  Helper function to set servo motor.
void set_servo_pos(gen_servo *motor, uint16_t pos){
    
  if((pos <= motor->max_pos) && (pos >= motor->min_pos)){
      printf("\n\tSetting Servo at %i.\n", pos);
    pwm_set_gpio_level(motor->signal_pin, pos);
      printf("\tServo at %i.\n\n", pos);
  }else{
      printf("\n\tServo input out of range.  Centering.\n");
    center_servo(motor);
      printf("\tServo at %i.\n\n", motor->center_pos);
  }
}


/*! \brief
  Initialise SG90 Servo Motor.

  *\param setup Struct containing servo variables
  
  \note 
    Sets up:
  \note
    Pin Function
  \note
    Slice Num
  \note
    Clk Div
  \note
    Wrap
*/
void initialize_servo(gen_servo *setup){
  //  default config, then we adjust
  pwm_config config = pwm_get_default_config();

    printf("Initializing Servo Pin: %i.\n", setup->signal_pin);
  gpio_set_function(setup->signal_pin, GPIO_FUNC_PWM);
  setup->pwm_slice = pwm_gpio_to_slice_num(setup->signal_pin);
  pwm_config_set_clkdiv(&config, setup->clk_div);
  pwm_config_set_wrap(&config, setup->duty_cycle);

  pwm_init(setup->pwm_slice, &config, true);
    center_servo(setup);
  printf("SG90 Servo Motor Initialized.\n");
}


//  Helper function to remember to initialize the servo pos.
void init_servo_pos(gen_servo *motor, uint16_t initial_pos){
  set_servo_pos(motor, initial_pos);
}

/*
// generic setup
  gen_servo generic_setup = {
  .signal_pin = 6,
  .center_pos = 2343,
  .min_pos = 1560,
  .max_pos = 3150,
  .clk_div = 64.f,
  .period_div = 39062
  }
*/

#endif

#endif
