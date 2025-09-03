#ifndef ALL_PROJECT_STRUCT_S_H
#define ALL_PROJECT_STRUCT_S_H

#include <stdio.h>
#include <stdint.h>
#include "pico/stdlib.h"
//#include "hardware/i2c.h"
#include "hardware/adc.h"
#include "hardware/dma.h"
#include "hardware/pwm.h"
//#include "hardware/spi.h"

#define pin uint8_t
#define timed uint32_t


const uint32_t *adc0_initial_value[] = {
  0x00000000, 0x00000000, 0x00000000, 0x00000000,
  0x00000000, 0x00000000, 0x00000000, 0x00000000,
};
const uint16_t *mult_adc_initial_value[] = {
  0x00000000, 0x00000000, 0x00000000, 0x00000000,
  0x00000000, 0x00000000, 0x00000000, 0x00000000,
};

/*
//  Define I2C struct here
typedef struct i2c_communication_s {
    // "Required" pins first
    uint CSX;
    uint RWR;
    uint SDA;
    uint SCK;

    //  Required communications needs.
    uint16_t device_id;
    uint32_t baud_rate;
    i2c_inst_t *inst;
    bool moss;      //  True for master, false for slave.

    //  Uint8_t buffers for holding transmit & receive.
    uint8_t tx_buffer[8];
    uint8_t rx_buffer[8];

    //  Uint8_t value for checking function returns
    uint8_t f_stat;

}i2c_com;

typedef struct i2c_slave_unit_s {

    volatile bool wake_state;
    volatile bool read_write_state;
    uint8_t tx_buffer[8];
    uint8_t sel_tx_buffer[4];   //  Used to hold the values of style,beer,and sense.

    uint8_t rx_buffer[8];
    uint8_t sel_rx_buffer[4];


}i2c_sb;

//  Define SPI struct here
typedef struct spi_communication_s {
    
    // Required pins first
    uint CSX;
    uint MOSI;
    uint MISO;
    uint SCK;

    //  Required communications needs.
    uint16_t device_id; //  Not required for SPI, but why not?
    uint32_t baud_rate;
    spi_inst_t *inst;

    //  Uint8_t buffers for holding transmit & receive.
    uint8_t tx_buffer[8];
    uint8_t rx_buffer[8];

    //  Uint8_t value for checking function returns
    uint8_t f_stat;

}spi_com;
*/

typedef struct rotary_input_s {

  //  rot_X_inc are the values we set for each encoders increment value.
  int8_t rot_a_inc;
  int8_t rot_b_inc;
  int8_t rot_c_inc;
  // shortened name: Fifo return
  volatile uint32_t fr[1];
  volatile uint32_t frr;
  volatile uint8_t bux_state;
  volatile int16_t pr1;
  volatile int16_t pr2;
  volatile int16_t pr3;
  volatile int16_t r1;
  volatile int16_t r2;
  volatile int16_t r3;
  volatile bool r1_b;
  volatile bool r2_b;
  volatile bool r3_b;

}pio_rot;

typedef struct payload_data_s {

  //    ready_load is the current line 45, payloads_t, rewritten.
  struct ready_load {
        
        struct load_duty_data {
          uint8_t upper_duty;
          uint8_t lower_duty;
        }load_duty_data;

        struct load_vert_data {
          uint8_t vert_upper;
          uint8_t vert_lower;
        }load_vert_data;

        struct load_hori_data {
          uint8_t hori_upper;
          uint8_t hori_lower;
        }load_hori_data;

        struct raw_data {
          uint8_t raw_upper;
          uint8_t raw_lower;
        }raw_data;

        uint16_t load_two;
        uint16_t load_three;
  }ready_load;
  
  struct payload_buffer {
      
      struct load_duty_buffer {
        uint8_t duty_upper;
        uint8_t duty_lower;
      }load_duty_buffer;

      struct load_vert_buffer {
        uint8_t vert_upper_buf;
        uint8_t vert_lower_buf;
      }load_vert_buffer;

      struct load_hori_buffer {
        uint8_t hori_upper_buf;
        uint8_t hori_lower_buf;
      }load_hori_buffer;

      struct raw_buf_data {
        uint8_t raw_buf_upper;
        uint8_t raw_buf_lower;
      }raw_buf_data;

        uint8_t load_two_buffer;
        uint16_t load_three_buffer;
  }payload_buffer;

}payload_data;

/*
typedef struct split_payload_parity_s {

uint16_t v_par_one;
uint16_t v_par_two;
uint16_t v_par_three;

uint16_t h_par_one;
uint16_t h_par_two;
uint16_t h_par_three;

}load_parity;
*/

typedef struct rc_pwm_port_s {

const uint8_t pwm_driveit;
const uint8_t pwm_backit;
const uint8_t pwm_turnit;

}rc_pwms;


typedef struct {
 const uint8_t register_one_data;
 const uint8_t register_one_latch;
 const uint8_t register_one_enable;
 const uint8_t register_two_data;
 const uint8_t register_two_latch;
 const uint8_t register_two_enable;
 const uint8_t register_clk_line;
} register_pins;
register_pins reg_pins;


typedef struct lcd_register_pin_s {
  
  const uint8_t lcd_reg_data;
  const uint8_t lcd_reg_latch;
  const uint8_t lcd_reg_enable;
  const uint8_t lcd_reg_clk;
  const uint8_t lcd_rs;
  const uint8_t lcd_e;

}register_lcd;


typedef struct {
  uint8_t register_value_zero;
  uint8_t register_value_one;
  uint8_t register_value_two;
  uint8_t register_value_three;
  uint8_t register_value_four;
  uint8_t register_value_five;
  uint8_t register_value_six;
  uint8_t register_value_seven;
} uint8_variables;
uint8_variables u_vars;


typedef struct turret_laser_s {
    
    uint8_t fire_pattern;
    uint16_t fire_speed;
    bool rotation_direction;

}t_laser;


typedef struct register_595_pin_s {

    const uint8_t DATA_LINE_ONE;
    const uint8_t LATCH_LINE_ONE;
    const uint8_t ENABLE_LINE_ONE;

    const uint8_t DATA_LINE_TWO;
    const uint8_t LATCH_LINE_TWO;
    const uint8_t ENABLE_LINE_TWO;

    const uint8_t REGISTERS_CLK_LINE;

} registered_pins;


typedef struct lcd_line_data
{

  char line_one[16];
  char line_two[16];
  char line_three[20];
  char line_four[20];

} lcd_lines;


typedef struct hc_sr04_t {

  uint8_t trigger_pin;
  uint8_t echo_pin;
  uint32_t time_start;
  uint32_t time_end;
  uint32_t pulse_duration;
  double distance;
  bool detection;

}hcsr04;


typedef struct {
  uint trigger_one;
  uint echo_one;
  uint echo_two;
  uint32_t usonic_one_start;
  uint32_t usonic_two_start;
  uint32_t usonic_one_end;
  uint32_t usonic_two_end;
  uint32_t pulse_duration_one;
  uint32_t pulse_duration_two;
  double distance_one;
  double distance_two;
  uint16_t returned_distance_one;
  uint16_t returned_distance_two;

}ultra_sonic_data_t;


//  Holds only two settings for two motors, forward, and reverse.
typedef struct dc_motor_pin_s {

    uint8_t motor_one_forward;
    uint8_t motor_one_reverse;
    
    uint8_t motor_two_forward;
    uint8_t motor_two_reverse;

}dc_pins;


typedef struct dc_motor_pwm_configuration_s {

    uint8_t dc_slice_one;
    uint8_t dc_chan_one;
    uint16_t dc_wrap_one;
    uint16_t dc_one_level;

    uint8_t dc_slice_two;
    uint8_t dc_chan_two;
    uint16_t dc_wrap_two;
    uint16_t dc_two_level;

}dc_pwm;


typedef struct dc_motor_configuration_s {

    dc_pins my_dc_pins;
    dc_pwm my_dc_pwm;

}dc_motor;


//  Hold two pins for now, more can be added fairly easily.
typedef struct servo_motor_pin_s {
  
  uint8_t servo_pin_one;
  uint8_t servo_pin_two;

}servo_pin;


typedef struct servo_motor_pwm_configuration_s {
  
  uint8_t servo_slice_one;
  uint8_t servo_chan_one;
  uint16_t servo_wrap_one;
  uint16_t servo_one_level;

  uint8_t servo_slice_two;
  uint8_t servo_chan_two;
  uint16_t servo_wrap_two;
  uint16_t servo_two_level;

  uint16_t servo_middle_ground;

}servo_pwm;


typedef struct servo_motor_configuration_s {

  servo_pin my_perv;
  servo_pwm my_serv;

}servo_motor;

// Singular Instance of an ADC pin.
typedef struct ADC_Instance_s {

  uint8_t adc_instance;
  //  pin num duh
  uint8_t pinum;
  uint16_t mapped;
  //  minimum in
  uint16_t miin;
  //  maximum in
  uint16_t main;
  //  minimum out
  uint16_t miout;
  //  maximum out
  uint16_t maout;
  // we should be sampling and averagering them.
  uint16_t raw[32];
  uint16_t raw_avg;

}ADC_Instance;


//  There are THREE ADC pins, and we have three adc objs, but the adc_instance start at zero
typedef struct ADC_Obj_s {

  ADC_Instance ADC_1;
  ADC_Instance ADC_2;
  ADC_Instance ADC_3;
  //  These bool will be set upon activation pin setup.
  bool adc1_status;
  bool adc2_status;
  bool adc3_status;

}ADC;


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
    uint dma_type_chan;
    volatile uint16_t button_value;
    
}input_types;


typedef struct core0_variables {

  volatile bool fifo_out_ready;

}c0_var;


typedef struct core1_variables {

  volatile bool fifo_location;
  volatile bool fifo_ready;
  bool fifo_read;
  volatile uint32_t fifo_read_in;
  volatile uint16_t wrapping_input;
  volatile uint16_t vertical_input;
  volatile uint16_t raw_adc_read;
  

}c1_vars;

typedef struct pre_calc_val_s {

uint16_t wrap_val_buffered;
uint16_t duty_cycle_buffed;
uint16_t servo_val_buffed;
uint16_t raw_adc_rec;

}pre_calcs;

//  Specifically useful for structs
typedef struct payload_size_s {

size_t zero_size;
size_t one_size;
size_t two_size;
size_t three_size;

}pay_size;




#endif