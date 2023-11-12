#ifndef PERI_FUNCS
#define PERI_FUNCS

#include <stdio.h>
#include <stdint.h>
#include "pico/stdlib.h"
#include "hardware/gpio.h"
#include "hardware/adc.h"

typedef struct {
  uint rotary_button_clk;
  uint rotary_button_dt;
  uint rotary_button_button;
  uint16_t rotary_current_value;
  uint16_t rotary_maximum_value;
  uint16_t rotary_minimum_value;
  uint analog_button_one;
  uint analog_button_two;
  uint button_one;
  uint button_two;
  uint button_three;
} button_types;
button_types enabled_buttons;

typedef struct {
  volatile  bool rotary_clk_status;
  volatile  bool rotary_dt_status;
  volatile  bool rotary_button_status;
  volatile  bool analog_one_status;
  volatile  bool analog_two_status;
  volatile  bool button_one_status;
  volatile  bool button_two_status;
  volatile  bool button_three_status;
} button_status;
button_status buttons_onoff;

typedef struct {
  bool previous_clk;
  bool previous_dt;
}rotary_status;
rotary_status last_status;

typedef struct {
  uint32_t rotary_clk_last_interrupt;
  uint32_t rotary_dt_last_interrupt;
  uint32_t rotary_button_last_interrupt;
  uint32_t analog_one_last_interrupt;
  uint32_t analog_two_last_interrupt;
  uint32_t button_one_last_interrupt;
  uint32_t button_two_last_interrupt;
  uint32_t button_three_last_interrupt;
} button_interrupts;
button_interrupts last_interrupt;

typedef struct {
  uint32_t rotary_clk_first_interrupt;
  uint32_t rotary_dt_first_interrupt;
  uint32_t rotary_button_first_interrupt;
  uint32_t analog_one_first_interrupt;
  uint32_t analog_two_first_interrupt;
  uint32_t button_one_first_interrupt;
  uint32_t button_two_first_interrupt;
  uint32_t button_three_first_interrupt;
} button_initial_interrupt;
button_initial_interrupt initial_interrupt;

typedef struct {
  int analog_one_max;
  int analog_two_max;
  int rotary_clk_max;
  int rotary_dt_max;
  int rotary_button_max;
  int button_one_max;
  int button_two_max;
  int button_three_max;
} button_interrupt_times;
button_interrupt_times interrupt_times;

typedef struct {
  uint adc0_pin;
  uint adc1_pin;
  uint adc2_pin;
  uint16_t adc0_raw_read;
  uint16_t adc1_raw_read;
  uint16_t adc2_raw_read;
  uint16_t adc0_min_in_map_value;
  uint16_t adc0_max_in_map_value;
  uint16_t adc1_min_in_map_value;
  uint16_t adc1_max_in_map_value;
  uint16_t adc2_min_in_map_value;
  uint16_t adc2_max_in_map_value;
  uint16_t adc0_min_out_map_value;
  uint16_t adc0_max_out_map_value;
  uint16_t adc1_min_out_map_value;
  uint16_t adc1_max_out_map_value;
  uint16_t adc2_min_out_map_value;
  uint16_t adc2_max_out_map_value;
  uint16_t adc0_mapped_value;
  uint16_t adc1_mapped_value;
  uint16_t adc2_mapped_value;
} adc_port_values;
adc_port_values pico_adc;

/*
    long impliments the variable to handle large values, can be changed if precision is necessary or easy.
    Parameters:     --long type implied--
    x:          Input for both analog.vertical & analog.horizontal.
    in_min:     Minimum value that an ADC can input. i.e {in_min = 0 || in_min = -1024}
    in_max:     Maximum value that an ADC can input. i.e {in_max = 1024 || in_max = 4096}
    out_min:    Minimum value to be set by MAP function. i.e {out_min = 0 || out_min = -1024}
    out_max:    Maximum value to be set by MAP function. i.e {out_max = 255 || out_max = 4096}

    EXAMPLE:
    x = 3600    in_min = 0      in_max = 4096       out_min = 200       out_max = 600

    (3600-0)*(600-200)/(4096-0)+200

    ===
    
    551.5625 the .5625 can be dropped if desired.
*/
long map(long x, long in_min, long in_max, long out_min, long out_max) {
    return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}

//  Completed
//  Adc_pin_call function takes in a struct and based on the set values, can call all three ADC ports, and map their respective values.
void adc_pin_call(adc_port_values *config) {

    //  Select inputs according to analog stick wiring.  Perform ADC call on inputs 0 & 1.
    //  Per ADC call, store respective values in analog.[vertical && horizontal].  

    //  Select which input to read data from (0 | 1 | 2).
  if(config->adc0_pin != NULL) {
    adc_select_input(0);
    config->adc0_raw_read = adc_read();
    }
  if(config->adc1_pin != NULL) {
    adc_select_input(1);
    config->adc1_raw_read = adc_read();
    }
  if(config->adc2_pin != NULL) {
    adc_select_input(2);
    config->adc2_raw_read = adc_read();
    }
    
    //  Convert raw analog input data of vertical & horizontal using MAP function.
    //  Store in respectively named analog.()_raw_conversion variable.
    if(config->adc0_raw_read > 0){
    config->adc0_mapped_value = map(config->adc0_raw_read, config->adc0_min_in_map_value, config->adc0_max_in_map_value, config->adc0_min_out_map_value, config->adc0_max_out_map_value);
    } else {
      printf("ADC0 Port Has No Value.\n");
    }
    if(config->adc1_raw_read > 0){
    config->adc1_mapped_value = map(config->adc1_raw_read, config->adc1_min_in_map_value, config->adc1_max_in_map_value, config->adc1_min_out_map_value, config->adc1_max_out_map_value);
    }  else {
      printf("ADC1 Port Has No Value.\n");
    }
    if(config->adc2_raw_read > 0){
    config->adc2_mapped_value = map(config->adc2_raw_read, config->adc2_min_in_map_value, config->adc2_max_in_map_value, config->adc2_min_out_map_value, config->adc2_max_out_map_value);
    }  else {
      printf("ADC2 Port Has No Value.\n");
    }
};

//  Button Interrupt Init
//  Sets GPIO function enabled with callback for up to four gpio interrupts.
//  Accepts a struct of button_types
//  Using if statements to determine if struct values are null or not
//  Initializes interrupts for gpio pins if true.
void button_interrupt_init(const button_types *types){
    if(types->analog_button_one != NULL){
        gpio_set_irq_enabled_with_callback(types->analog_button_one, 0x04 | 0x08, true, &button_callback);
        printf("Button Type: Analog Button One: Pin %d.  Initialized.\n", types->analog_button_one);
    } else {
        printf("Analog Button One Not Used.\n");
    }

    if(types->analog_button_two != NULL){
        gpio_set_irq_enabled_with_callback(types->analog_button_one, 0x04 | 0x08, true, &button_callback);
        printf("Button Type: Analog Button Two: Pin %d.  Initialized.\n", types->analog_button_two);
    } else {
        printf("Analog Button Two Not Used.\n");
    }

    if(types->rotary_button_clk != NULL){
        gpio_set_irq_enabled_with_callback(types->rotary_button_clk, 0x04 | 0x08, true, &button_callback);
        printf("Button Type: Rotary Clk: Pin %d.  Initialized.\n", types->rotary_button_clk);
    } else {
                printf("Rotary Clk Not Used.\n");
    }

        if(types->rotary_button_dt != NULL){
        gpio_set_irq_enabled_with_callback(types->rotary_button_dt, 0x04 | 0x08, true, &button_callback);
        printf("Button Type: Rotary DT: Pin %d.  Initialized.\n", types->rotary_button_dt);
    } else {
                printf("Rotary DT Not Used.\n");
    }

        if(types->rotary_button_button != NULL){
        gpio_set_irq_enabled_with_callback(types->rotary_button_button, 0x04 | 0x08, true, &button_callback);
        printf("Button Type: Rotary: Pin %d.  Initialized.\n", types->rotary_button_button);
    } else {
                printf("Rotary Button Not Used.\n");
    }

    if(types->button_one != NULL){

      gpio_set_irq_enabled_with_callback(types->button_one, 0x04 | 0x08, true, &button_callback);
        printf("Button Type: Rotary: Pin %d.  Initialized.\n", types->button_one);
    } else {
                printf("Button OneNot Used.\n");
    }
    if(types->button_two != NULL){

      gpio_set_irq_enabled_with_callback(types->button_two, 0x04 | 0x08, true, &button_callback);
        printf("Button Type: Rotary: Pin %d.  Initialized.\n", types->button_two);
    } else {
                printf("Button Two Not Used.\n");
    }

    if(types->button_three != NULL){

      gpio_set_irq_enabled_with_callback(types->button_three, 0x04 | 0x08, true, &button_callback);
        printf("Button Type: Rotary: Pin %d.  Initialized.\n", types->button_three);
    } else {
                printf("Button Three Not Used.\n");
    }
}

void button_callback(uint gpio, uint32_t events) {

//    Useful time functions to combat button debounce on rotary encoder.
    uint32_t current_interrupt;
    uint32_t last_interrupts;

    if(gpio == (enabled_buttons.analog_button_one)){
    initial_interrupt.analog_one_first_interrupt = current_interrupt;
    last_interrupts = initial_interrupt.analog_one_first_interrupt - last_interrupt.analog_one_last_interrupt;
      gpio_acknowledge_irq(gpio, events);
      if(last_interrupts >= interrupt_times.analog_one_max){
        buttons_onoff.analog_one_status = true;
        printf("\n\t Analog Button One Press Occured.\n");
      }
    last_interrupt.analog_one_last_interrupt = current_interrupt;
    }

    if(gpio == (enabled_buttons.analog_button_two)){
    initial_interrupt.analog_two_first_interrupt = current_interrupt;
    last_interrupts = initial_interrupt.analog_two_first_interrupt - last_interrupt.analog_two_last_interrupt;
      gpio_acknowledge_irq(gpio, events);
      if(last_interrupts >= interrupt_times.analog_two_max){
        buttons_onoff.analog_two_status = true;
        printf("\n\t Analog Button Two Press Occured.\n");
      }
    last_interrupt.analog_two_last_interrupt = current_interrupt;
    }

    if(gpio == (enabled_buttons.button_one)){
    initial_interrupt.button_one_first_interrupt = current_interrupt;
    last_interrupts = initial_interrupt.button_one_first_interrupt - last_interrupt.button_one_last_interrupt;
      gpio_acknowledge_irq(gpio, events);
      if(last_interrupts >= interrupt_times.button_one_max){
    buttons_onoff.button_one_status = true;
        printf("\n\t Button One Press Occured.\n");
      }
    last_interrupt.button_one_last_interrupt = current_interrupt;
    }

    if(gpio == (enabled_buttons.button_two)){
    initial_interrupt.button_two_first_interrupt = current_interrupt;
    last_interrupts = initial_interrupt.button_two_first_interrupt - last_interrupt.button_two_last_interrupt;
      gpio_acknowledge_irq(gpio, events);
      if(last_interrupts >= interrupt_times.button_two_max){
        buttons_onoff.button_two_status = true;
        printf("\n\t Button Two Press Occured.\n");
      }
    last_interrupt.button_two_last_interrupt = current_interrupt;
    }

    if(gpio == (enabled_buttons.button_three)){
    initial_interrupt.button_three_first_interrupt = current_interrupt;
    last_interrupts = initial_interrupt.button_three_first_interrupt - last_interrupt.button_three_last_interrupt;
      gpio_acknowledge_irq(gpio, events);
      if(last_interrupts >= interrupt_times.button_three_max){
        buttons_onoff.button_three_status = true;
        printf("\n\t Button Three Press Occured.\n");
      }
    last_interrupt.button_three_last_interrupt = current_interrupt;
    }

    if(gpio == (enabled_buttons.rotary_button_button)){
      initial_interrupt.rotary_button_first_interrupt = current_interrupt;
      last_interrupts = initial_interrupt.rotary_button_first_interrupt - last_interrupt.rotary_button_last_interrupt;
      gpio_acknowledge_irq(gpio, events);
      if(last_interrupts >= interrupt_times.rotary_button_max){
        buttons_onoff.rotary_button_status = true;
        printf("\n\t Rotary Button Press Occured.\n");
      }
    last_interrupt.rotary_button_last_interrupt = current_interrupt;
    }


  //    Check if pin is rotary_clk
  if(gpio == (enabled_buttons.rotary_button_clk)) {

  initial_interrupt.rotary_clk_first_interrupt = current_interrupt;
      last_interrupts = initial_interrupt.rotary_clk_first_interrupt - last_interrupt.rotary_clk_last_interrupt;
      gpio_acknowledge_irq(gpio, events);

    last_interrupt.rotary_clk_last_interrupt = current_interrupt;

    if (last_interrupt.rotary_clk_last_interrupt >= interrupt_times.rotary_clk_max) {

      buttons_onoff.rotary_clk_status = gpio_get(enabled_buttons.rotary_button_clk);
      buttons_onoff.rotary_dt_status = gpio_get(enabled_buttons.rotary_button_dt);
        
  //    Check status of current rotary clk vs last rotary clk.
    if(buttons_onoff.rotary_clk_status != last_status.previous_clk){

  //    Check if -clk = 1 && dt = 0- || -clk = 0 && dt = 1-
    if(((buttons_onoff.rotary_clk_status && !buttons_onoff.rotary_dt_status) || (!buttons_onoff.rotary_clk_status && buttons_onoff.rotary_dt_status))){
    
      if(enabled_buttons.rotary_current_value <= enabled_buttons.rotary_maximum_value) {

        if(enabled_buttons.rotary_maximum_value >= 2000){
          enabled_buttons.rotary_current_value += 100;
        } else if((enabled_buttons.rotary_maximum_value <= 1999) && (enabled_buttons.rotary_maximum_value >= 1500)){
          enabled_buttons.rotary_current_value += 50;
        } else if((enabled_buttons.rotary_maximum_value <= 1499) && (enabled_buttons.rotary_maximum_value >= 1000)){
          enabled_buttons.rotary_current_value += 25;
        } else if((enabled_buttons.rotary_maximum_value <= 999) && (enabled_buttons.rotary_maximum_value >= 0)){
          enabled_buttons.rotary_current_value += 15;
        } else {
          printf("\n\n\tMaximum Rotary Value Not Set.  Error Occured.\n");
        }
  
       if(enabled_buttons.rotary_maximum_value >= 2000 && (enabled_buttons.rotary_current_value >= 2500)){
          enabled_buttons.rotary_current_value = 2450;
        } else if((enabled_buttons.rotary_maximum_value <= 1999) && (enabled_buttons.rotary_current_value >= 1999)){
          enabled_buttons.rotary_current_value = 1950;
        } else if((enabled_buttons.rotary_maximum_value <= 1499) && (enabled_buttons.rotary_current_value >= 1499)){
          enabled_buttons.rotary_current_value = 1450;
        } else if((enabled_buttons.rotary_maximum_value <= 999) && (enabled_buttons.rotary_current_value >= 999)){
          enabled_buttons.rotary_current_value = 950;
        } else {
          printf("\n\n\tMaximum Rotary Reached.\n");
        }
        }
    }
        last_status.previous_clk = buttons_onoff.rotary_clk_status;
        last_status.previous_dt = buttons_onoff.rotary_dt_status;
    }
  }
}
  //    Check if pin is rotary_dt.  
  if(gpio == (enabled_buttons.rotary_button_dt)){

  initial_interrupt.rotary_dt_first_interrupt = current_interrupt;
      last_interrupts = initial_interrupt.rotary_dt_first_interrupt - last_interrupt.rotary_dt_last_interrupt;
      gpio_acknowledge_irq(gpio, events);

    last_interrupt.rotary_dt_last_interrupt = current_interrupt;

    if (last_interrupt.rotary_dt_last_interrupt >= interrupt_times.rotary_dt_max) {

      buttons_onoff.rotary_clk_status = gpio_get(enabled_buttons.rotary_button_clk);
      buttons_onoff.rotary_dt_status = gpio_get(enabled_buttons.rotary_button_dt);
        
  //    Check status of current rotary clk vs last rotary clk.
    if(buttons_onoff.rotary_dt_status != last_status.previous_dt){

  //    Check if -clk = 1 && dt = 0- || -clk = 0 && dt = 1-
    if(((buttons_onoff.rotary_dt_status && !buttons_onoff.rotary_clk_status) || (!buttons_onoff.rotary_dt_status && buttons_onoff.rotary_clk_status))){
    
      if(enabled_buttons.rotary_current_value <= enabled_buttons.rotary_maximum_value) {

        if(enabled_buttons.rotary_minimum_value >= 2000){
          enabled_buttons.rotary_current_value -= 100;
        } else if((enabled_buttons.rotary_maximum_value <= 1999) && (enabled_buttons.rotary_maximum_value >= 1500)){
          enabled_buttons.rotary_current_value -= 50;
        } else if((enabled_buttons.rotary_maximum_value <= 1499) && (enabled_buttons.rotary_maximum_value >= 1000)){
          enabled_buttons.rotary_current_value -= 25;
        } else if((enabled_buttons.rotary_maximum_value <= 999) && (enabled_buttons.rotary_maximum_value >= 0)){
          enabled_buttons.rotary_current_value -= 15;
        } else {
          printf("\n\n\tMaximum Rotary Value Not Set.  Error Occured.\n");
        }
  
       if(enabled_buttons.rotary_current_value <= 0){
          enabled_buttons.rotary_current_value = 25;
        } else {
          printf("\n\n\tMinimum Rotary Reached.\n");
        }
        }
    }
        last_status.previous_clk = buttons_onoff.rotary_clk_status;
        last_status.previous_dt = buttons_onoff.rotary_dt_status;
    }
}
}
}

//  Pin setup function: rotary_pin_setups
//  Rotary Input Pin One Associated With Clk Pin
//  Rotary Input Pin Two Associated With DT Pin
//  Rotary Input Pin Three Associated With Rotary Button
void rotary_pin_setups(uint rotary_input_pin_one, uint rotary_input_pin_two, uint rotary_input_pin_three){
    
    gpio_init(rotary_input_pin_one);
    gpio_set_dir(rotary_input_pin_one, GPIO_IN);
    gpio_pull_up(rotary_input_pin_one);

    gpio_init(rotary_input_pin_two);
    gpio_set_dir(rotary_input_pin_two, GPIO_IN);
    gpio_pull_up(rotary_input_pin_two);

    gpio_init(rotary_input_pin_three);
    gpio_set_dir(rotary_input_pin_three, GPIO_IN);
    gpio_pull_up(rotary_input_pin_three);
}

void shift_register_pin_init(uint data, uint clk, uint r_latch, uint enable){
    gpio_init(data);
    gpio_set_dir(data, GPIO_OUT);
    gpio_init(clk);
    gpio_set_dir(clk, GPIO_OUT);
    gpio_init(r_latch);
    gpio_set_dir(r_latch, GPIO_OUT);
    gpio_init(enable);
    gpio_set_dir(enable, GPIO_OUT);
}

//  Function to create pulse for shift latching.
//  Takes a uint GPIO pin to allow multiple shift registers.
void latch_data(uint latch_pin) {
    gpio_put(latch_pin, 1); // Generate latch pulse
    gpio_put(latch_pin, 0);
}

//  Function to create pulse for clock pulses.
//  Takes a uint GPIO pin to allow multiple shift registers.
void clk_pulse(uint clk_pin) {
    gpio_put(clk_pin, 1);
    gpio_put(clk_pin, 0);
}

/*
    ::serial_register_ouput function::
    Params:
    uint gpio_display - Reference to gpio pin tied to register display enable.
    uint gpio_pin -     Reference to serial data output pin.  Currently labeled serialData.  Additional can be added by muxing clock, notOE, and reg_clk.
    bool data_in[] -    With use for RGB led register and sevsegdis in mind.

*/

void serial_register_output(uint gpio_display, uint gpio_data_pin, uint clk_pin, uint latch_pin, uint8_t data_in) {

    printf("Serial Register Function Initialized.\n\n");
    //  Set display enable high for duration of data transfer.
    gpio_put(gpio_display, 1);

    printf("Value of shift register buffer: 0b%08x.\n", data_in);

    //  Create loop to shift data into selected register output.
    for(int a = 0; a < 8; a++){
    //  Put data_in line high or low, pulse register clock.
    ((data_in >> a) & 0x01) ? gpio_put(gpio_data_pin, 1) : gpio_put(gpio_data_pin, 0);     //  During each iteration, the selected gpio pin will be left shifted the next bit of data_in by cc.
    printf("Shifting: 0b%01x.\n", (data_in >> a));
    clk_pulse(clk_pin);                               //  Pulse clock per iteration.              
    }

    latch_data(latch_pin);                            //  After loop iterates, call latch function to pulse register latch.
    clk_pulse(clk_pin);                               //  Pulse clock finish input loop.

    gpio_put(gpio_display, 0);                        //  Set display enable low to enable register output pins.
        sleep_ms(100);

}

void ultra_sonic_distance(uint trigger_pin, uint echo_pin, uint16_t distance_return){

    printf("\n\tUltrasonic Sensor Function.\n");

        gpio_put(trigger_pin, 1);
        sleep_us(10);
        gpio_put(trigger_pin, 0);

        // Wait for the echo pulse
        while (gpio_get(echo_pin) == 0) {}
        uint32_t start_time = time_us_32();

        while (gpio_get(echo_pin) == 1) {}
        uint32_t end_time = time_us_32();

        // Calculate the distance
        uint32_t pulse_duration = end_time - start_time;
        double distance = (pulse_duration / 2.0) * 0.0343; // Speed of sound is 343 m/s
        printf("Distance: %.2f cm\n", distance);
        distance_return = distance;
        printf("N_Distance: 0x%02xcm\n", distance_return);
}

#endif