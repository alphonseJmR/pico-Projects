#ifndef STRUCT_DEFINITION_S_H
#define STRUCT_DEFINITION_S_H

#include "project_struct_s.h"
#include "../Periphereals/resources/pico_pin_enum.h"


input_types my_types = {

  .r_en = {
    .CLK = UNDEFINED,
    .DT = UNDEFINED,
    .REB = UNDEFINED,
    .max_rota_val = 500,
    .r_en_max = 2000
    },

  .t_buttons = {
    .b_zero = GPIO_SIXTEEN,
    .b_one = GPIO_SEVENTEEN
    
  },

  .button_value = 0

};


servo_motor my_servo = {

  .my_perv.servo_pin_one = GPIO_TWENTY,
  .my_perv.servo_pin_two = UNDEFINED,

  .my_serv.servo_wrap_one = 39062.f,
  .my_serv.servo_one_level = 0,

  .my_serv.servo_middle_ground = (146 * 10)

};


dc_motor my_dc = {

  .my_dc_pins.motor_one_forward = GPIO_EIGHTEEN,
  .my_dc_pins.motor_one_reverse = GPIO_NINETEEN,

  .my_dc_pins.motor_two_forward = UNDEFINED,
  .my_dc_pins.motor_two_reverse = UNDEFINED,

  .my_dc_pwm.dc_slice_one = 0,
  .my_dc_pwm.dc_chan_one = 0,
  .my_dc_pwm.dc_wrap_one = 0,
  .my_dc_pwm.dc_one_level = 0,

  .my_dc_pwm.dc_slice_two = 0,
  .my_dc_pwm.dc_chan_two = 0,
  .my_dc_pwm.dc_wrap_two = 0,
  .my_dc_pwm.dc_two_level = 0

};


c0_var my_core0 = {

  .fifo_out_ready = false

};


pre_calcs received_calcs = {
  
  .wrap_val_buffered = 0,
  .duty_cycle_buffed = 0,
  .servo_val_buffed = 0

};


c1_vars my_core = {
    
    .fifo_ready = false,
    .fifo_read = false,
    .fifo_read_in = 0,
    .wrapping_input = 0,
    .vertical_input = 0

};


pre_calcs my_pres ={

  .wrap_val_buffered = 0,
  .duty_cycle_buffed = 0,
  .servo_val_buffed = 0,
  .raw_adc_rec = 0

};


pay_size my_load_size = {

  .zero_size = 0,
  .one_size = 0,
  .two_size = 0,
  .three_size = 0

};


payload_data my_loads = {

  .ready_load.load_duty_data.upper_duty = 0,
  .ready_load.load_duty_data.lower_duty = 0,
  .ready_load.load_vert_data.vert_upper = 0,
  .ready_load.load_vert_data.vert_lower = 0,
  .ready_load.load_hori_data.hori_upper = 0,
  .ready_load.load_hori_data.hori_lower = 0,
  .ready_load.raw_data.raw_upper = 0,
  .ready_load.raw_data.raw_lower = 0,
  .ready_load.load_two = 0,
  .ready_load.load_three = 0,

  .payload_buffer.load_duty_buffer.duty_upper = 0,
  .payload_buffer.load_duty_buffer.duty_lower = 0,
  .payload_buffer.load_vert_buffer.vert_upper_buf = 0,
  .payload_buffer.load_vert_buffer.vert_lower_buf = 0,
  .payload_buffer.load_hori_buffer.hori_upper_buf = 0,
  .payload_buffer.load_hori_buffer.hori_lower_buf = 0,
  .payload_buffer.raw_buf_data.raw_buf_upper = 0,
  .payload_buffer.raw_buf_data.raw_buf_lower = 0,
  .payload_buffer.load_two_buffer = 0,
  .payload_buffer.load_three_buffer = 0

};


payload_data turret_load = {

  .ready_load.load_duty_data.upper_duty = 0,
  .ready_load.load_duty_data.lower_duty = 0,
  .ready_load.load_vert_data.vert_upper = 0,
  .ready_load.load_vert_data.vert_lower = 0,
  .ready_load.load_hori_data.hori_upper = 0,
  .ready_load.load_hori_data.hori_lower = 0,
  .ready_load.raw_data.raw_upper = 0,
  .ready_load.raw_data.raw_lower = 0,
  .ready_load.load_two = 0,
  .ready_load.load_three = 0,

  .payload_buffer.load_duty_buffer.duty_upper = 0,
  .payload_buffer.load_duty_buffer.duty_lower = 0,
  .payload_buffer.load_vert_buffer.vert_upper_buf = 0,
  .payload_buffer.load_vert_buffer.vert_lower_buf = 0,
  .payload_buffer.load_hori_buffer.hori_upper_buf = 0,
  .payload_buffer.load_hori_buffer.hori_lower_buf = 0,
  .payload_buffer.raw_buf_data.raw_buf_upper = 0,
  .payload_buffer.raw_buf_data.raw_buf_lower = 0,
  .payload_buffer.load_two_buffer = 0,
  .payload_buffer.load_three_buffer = 0

};


ADC ADC_Default_Config = {
  .ADC_1 = {
    .adc_instance = 0,
    .pinum = GPIO_TWENTY_SIX,
    .raw = 0,
    .mapped = 0,
    .miin = 0,
    .main = 4096,
    .miout = 0,
    .maout = 1024
  },
  .ADC_2 = {
    .adc_instance = 1,
    .pinum = GPIO_TWENTY_SEVEN,
    .raw = 0,
    .mapped = 0,
    .miin = 0,
    .main = 4096,
    .miout = 0,
    .maout = 1024
  },
  .ADC_3 = {
    .adc_instance = 2,
    .pinum = GPIO_TWENTY_EIGHT,
    .raw = 0,
    .mapped = 0,
    .miin = 0,
    .main = 4096,
    .miout = 0,
    .maout = 1024
  }
};
/*
adc_port_values my_adc = {
    .adc0_pin = GPIO_TWENTY_SIX,
    .adc1_pin = GPIO_TWENTY_SEVEN,
    .adc2_pin = GPIO_TWENTY_EIGHT,

    .adc0_min_in_map_value = 0,       //  ADC0 Port will be the battery voltage detection.
    .adc0_max_in_map_value = 4096,    //  4096 is the max resolution we can expect.

    .adc1_min_in_map_value = 0,       //  ADC1 Port will route the vertical channel of the joystick into a mapped value we can use.
    .adc1_max_in_map_value = 4096,    //  4096 is the max resolution we can expect.

    .adc2_min_in_map_value = 0,       //  ADC2 Port will route the horizontal channel of the joystick into a mapped value we can use.
    .adc2_max_in_map_value = 4096,    //  4096 is the max resolution we can expect.

    .adc0_min_out_map_value = 0,      //  Usually a minimum of zero, but for say a servo, keeping it center may require non zero.  Also, the pico can't do negatives?
    .adc0_max_out_map_value = 10,     //  For the Battery voltage, we can try an arbirtraily low number, like 10.

    .adc1_min_out_map_value = 0,      //  Joystick input minimum of zero in our case.
    .adc1_max_out_map_value = 1024,   //  For joystick input lets set the cap at 1024

    .adc2_min_out_map_value = 0,      //  Joystick input minimum of zero in our case.
    .adc2_max_out_map_value = 1024,      //  For joystick input lets set the cap at 1024.
    
    .adc0_mapped_value = 0,
    .adc1_mapped_value = 0,
    .adc2_mapped_value = 0,
    .adc0_mapped_float = 0,
    .adc1_mapped_float = 0,
    .adc2_mapped_float = 0
};


adc_port_values turret_adc = {
  .adc0_pin = UNDEFINED,
  .adc1_pin = GPIO_TWENTY_SEVEN,
  .adc2_pin = GPIO_TWENTY_EIGHT,
  .adc0_raw_read = 0,
  .adc1_raw_read = 0,
  .adc2_raw_read = 0,
  .adc0_min_in_map_value = 0,
  .adc0_max_in_map_value = 0,
  .adc1_min_in_map_value = 0,
  .adc1_max_in_map_value = 4096,
  .adc2_min_in_map_value = 0,
  .adc2_max_in_map_value = 4096,
  .adc0_min_out_map_value = 0,
  .adc0_max_out_map_value = 0,
  .adc1_min_out_map_value = 0,
  .adc1_max_out_map_value = 255,
  .adc2_min_out_map_value = 0,
  .adc2_max_out_map_value = 255,
  .adc0_mapped_value = 0,
  .adc1_mapped_value = 0,
  .adc2_mapped_value = 0,
  
};


adc_port_values pivot_adc = {
  .adc0_pin = UNDEFINED,
  .adc1_pin = GPIO_TWENTY_SEVEN,
  .adc2_pin = GPIO_TWENTY_EIGHT,
  .adc0_raw_read = 0,
  .adc1_raw_read = 0,
  .adc2_raw_read = 0,
  .adc0_min_in_map_value = 0,
  .adc0_max_in_map_value = 0,
  .adc1_min_in_map_value = 0,
  .adc1_max_in_map_value = 4096,
  .adc2_min_in_map_value = 0,
  .adc2_max_in_map_value = 4096,
  .adc0_min_out_map_value = 0,
  .adc0_max_out_map_value = 0,
  .adc1_min_out_map_value = 0,
  .adc1_max_out_map_value = 255,
  .adc2_min_out_map_value = 0,
  .adc2_max_out_map_value = 255,
  .adc0_mapped_value = 0,
  .adc1_mapped_value = 0,
  .adc2_mapped_value = 0,
  
};

*/

input_types turret_buttons = {

  .t_buttons = {
    .b_zero = GPIO_SIXTEEN,
    .b_one = GPIO_SEVENTEEN
  }

};


#endif