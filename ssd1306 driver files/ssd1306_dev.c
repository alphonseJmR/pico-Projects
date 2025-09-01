#include <stdio.h>
#include "stdlib.h"
#include "pico/time.h"
#include "Poly_Tools.h"
#include "cdhc_in.pio.h"
#include "ssd1306_i2c_driver.h"

uint8_t p_chan;
uint8_t p_slice;
pwm_config base;

int16_t current_value[3] = {0, 0, 0};
int16_t previous_value[3] = {0, 0, 0};

s_buffer rotary;
s_buffer temps;

repeating_timer_t gr_timer;

nema_info slim = {
  .n_pins = {
    .step = 22,
    .direction = 13,
    .fault = 21,
    .fan1 = 9,
    .fan2 = 8
  },
  .pwm_cfg = {
    .slice = 0,
    .channel = 0,
    .init_clk_div = minimum_clk_div,
    .init_wrap = minimum_wrap_val,
    .init_level = minimum_level_val
  },
  .motor_temp = 0.00,
  .drv_temp = 0.00,
  .motor_direction = true,
  .fan1_status = false,
  .fan2_status = false,
  .fault_status = true,
  .max_motor_temp = 475,
  .max_drv_temp = 285,
  .max_clk_div = 250

};

ssd_i2c rotary_screen = {
  
  .inst = i2c0,
  .scl = 17,
  .sda = 16,
  .b_rate = 50000

};

ssd_i2c motor_temp_screen = {
  
  .inst = i2c1,
  .scl = 19,
  .sda = 18,
  .b_rate = 50000

};


// Include rotary encoder to change to the frequency of the HV output
void setup_pwm_pin(uint8_t pina, uint32_t clkdiv, uint16_t wrap, uint16_t level){

    p_chan = pwm_gpio_to_channel(pina);
    p_slice = pwm_gpio_to_slice_num(pina);

    gpio_init(pina);
    gpio_set_function(pina, GPIO_FUNC_PWM);

    base = pwm_get_default_config();
    pwm_config_set_clkdiv_int(&base, clkdiv);
    pwm_config_set_wrap(&base, wrap);
    pwm_init(p_slice, &base, true);
    
    pwm_set_chan_level(p_slice, p_chan, level);
}

void set_new_wrap(uint16_t new_wrap){
    pwm_set_wrap(p_slice, new_wrap);
}

void set_new_clkdiv(uint8_t new_clkdiv){
  pwm_set_enabled(p_slice, false);
      sleep_ms(50);
  //  pwm_config_set_clkdiv_int(&base, new_clkdiv);
    pwm_set_clkdiv(p_slice, new_clkdiv);
      sleep_ms(50);
  pwm_set_enabled(p_slice, true);
}

// store current rotary values in this array
void update_value(reir *input, int16_t *value){
  value[0] = input->re1_v;  
  value[1] = input->re2_v;  
  value[2] = input->re3_v;
    
}

// returns the result of checking any
bool have_values_updated(reir *input){
  
  bool a = false;
  bool b = false;
  bool c = false;

  update_value(input, current_value);
  
  if(current_value[0] != previous_value[0]){
    a = true;
  //  printf("value change in re1\n");
  }else{
    a = false;
  }
  if(current_value[1] != previous_value[1]){
    b = true;
  //  printf("value change in re2\n");
  }else{
    b = false;
  }
  if(current_value[2] != previous_value[2]){
    c = true;
  //  printf("value change in re3\n");
  }else{
    c = false;
  }

  update_value(input, previous_value);
  // we want to if any of values changed.
//  printf("end result: %i\n", (uint8_t)(a|b|c));
  return (a|b|c);
  
}


void update_screen_value(ssd_i2c *cfg, s_buffer *line){
  double pdc = ((double)(current_value[1] / (double)current_value[0]) * 100);
  update_screen_rotary_buffers(line, current_value[0], current_value[1], current_value[2], pdc);
  write_string(cfg, 0, 0, line->line1_buf, false);
  write_string(cfg, 0, 10, line->line2_buf, false);
  write_string(cfg, 0, 20, line->line3_buf, false);
  write_string(cfg, 0, 30, line->line4_buf, false);
  send_to_screen(cfg, temp_buf, 1);
}

void update_clkdiv(reir *input){
      if(input->switch1){
      printf("switch1 activated.\n");
      if(input->re3_v <= 254 && input->re3_v > 0){
        printf("New Clk Div: %i.\n", input->re3_v);
          set_new_clkdiv(input->re3_v);
      }else{
        set_new_clkdiv(210);
        input->re3_v = 5;
      }
      input->switch1 = false;
   }
}

// this could be done with a dma channel
void update_adc_to_nema(nema_info *set, double r1, double r2){
  set->motor_temp = r1;
  set->drv_temp = r2;
}


void get_he_sensors(generic_register *reg){
  reg->reg_ret = pio0->rxf[reg->gr_SM];
}


bool sensor_read(repeating_timer_t *rtt){
  get_he_sensors(&he_sensor);
  return true;
}


void core1_entry(){
  // for general register use (in this case, hall effect sensors)
  bool gr_timer_status = false;
  bool arp1 = true;
  uint8_t dir_poll = 0;
  uint8_t display_poll = 0;
  double result1 = 0.00;
  double result2 = 0.00;
  double prevult1 = 0.00;
  double prevult2 = 0.00;

  stdio_init_all();
    printf("Core 2 Launched Successfully.\n");
  adc_init();
  ADC_Single_Initialisation(&adc_thermistor);
  ADC_Single_Initialisation(&adc_thermistor1);
  init_he_sensor_program(&he_sensor);
    sleep_ms(200);
  setup_hall_dma(&hall_dma, &he_sensor, &dma_c1, 200);
  initialize_nema17_pins(&slim);
  setup_ssd1306(&motor_temp_screen, 2);
    sleep_ms(300);
  clear_gram(&motor_temp_screen, 2);


  // grab initial values to preset things... ya know
    result1 = temp_result(&adc_thermistor);
    result2 = temp_result(&adc_thermistor1);
  printf("Initial Temperature 1: %f.\n\n", result1);
  printf("Initial Temperature 2: %f.\n\n", result2);
  update_fan_status(&slim);

  write_temp_screen_string(&motor_temp_screen, &temps, result1, result2, slim.fan1_status, slim.fan2_status, 2);

// this is the setup for repeating timer if not using DMA
/*
  gr_timer_status = add_repeating_timer_us(50, sensor_read, NULL, &gr_timer);
    if(!gr_timer_status){
      printf("Motor Hall Effect Timer Start-Up Error.\n");
      arp1 = false;
    }else{
      printf("Motor Hall Effect Timer Started Correctly.\n");
    }
*/

  printf("\n\nCore2 Program Starting Now.\n\n");
      sleep_ms(750);

  while(arp1){
    
      update_motor_direction(&slim, (dir_poll < 10));
        printf("Rotation Increments: %i.\n", he_sensor.rotation_inc);
        printf("Total Rotations: %i.\n\n", he_sensor.rotations);
      if(dir_poll < 20){
        dir_poll = 0;
      }
      dir_poll++;

    result1 = temp_result(&adc_thermistor);
    result2 = temp_result(&adc_thermistor1);
      update_adc_to_nema(&slim, result1, result2);
  //  printf("Motor Temperature: %f.\n", result1);
  //  printf("Drv8825 Temperature: %f.\n\n", result2);

  printf("Hall Sensor Returned: 0x%02x.\n", he_sensor.reg_ret);

      update_fan_status(&slim);

    
    if(result1 != prevult1 || result2 != prevult2){
      if(display_poll < 5){
      }else{
    //    printf("Printing to Temps Screen.\n");
        write_temp_screen_string(&motor_temp_screen, &temps, result1, result2, slim.fan1_status, slim.fan2_status, 2);
        display_poll = 0;
      }
    }
    display_poll++;
  //    printf("\nDisplay Poll: %i.\n", display_poll);
      sleep_ms(500);
    prevult1 = result1;
    prevult2 = result2;
}
  tight_loop_contents();
}


int main() {
  // rotary encoder timer.
  bool re_timer = false;
  
  //  active run program boolean.
  bool arp = true;

  stdio_init_all();
  
  sleep_ms(7000);
  sleep_ms(4000);

  multicore_launch_core1(core1_entry);
  setup_ssd1306(&rotary_screen, 1);
    sleep_ms(300);
  clear_gram(&rotary_screen, 1);
  init_rotary_encoder_program(&generic_setup);
  setup_pwm_pin(15, 208, 60, 30);
  pwm_set_enabled(p_slice, true);
  re_timer = add_repeating_timer_us(600, get_rot_pos, NULL, &reir_timer);
    if(!re_timer){
      printf("Rotary Encoder Timer Start-Up Error.\n");
      arp = false;
    }else{
      printf("Rotary Encoder Timer Started Correctly.\n");
    }

    printf("\n\nCore1 Program Starting Now.\n\n");
      sleep_ms(750);


while(arp) {

  if(have_values_updated(&generic_setup)){
    update_screen_value(&rotary_screen, &rotary);
  }

  sleep_ms(200);
    if(generic_setup.re1_v <= 200 && generic_setup.re1_v > 0){
      set_new_wrap(generic_setup.re1_v);
        if(generic_setup.re2_v <= 199 && generic_setup.re2_v > 0){
          pwm_set_chan_level(p_slice, p_chan, generic_setup.re2_v);
        }else{
          pwm_set_chan_level(p_slice, p_chan, (generic_setup.re1_v / 2));
          generic_setup.re2_v = 5;
        }
    }else{
      set_new_wrap(60);
      pwm_set_chan_level(p_slice, p_chan, 30);
      generic_setup.re1_v = 10;
      generic_setup.re3_v = 60;
    }
    
    update_clkdiv(&generic_setup);

}
tight_loop_contents();
}
