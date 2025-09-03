#ifndef PICO_ADC_PORT_INPUT_S_H
#define PICO_ADC_PORT_INPUT_S_H

#include <stdio.h>
#include <stdint.h>
#include <math.h>
#include "../../hi_lvl_resources/project_struct_s.h"
#include "pico_pin_enum.h"
#include "pico/stdlib.h"
#include "pico/multicore.h"
#include "pico/time.h"
#include "hardware/gpio.h"
#include "hardware/adc.h"
#include "hardware/dma.h"
#include "hardware/timer.h"

typedef union {
  uint8_t u8;
  uint16_t u16;
  uint32_t u32;
}vari_size;

#define thermistor
#define run_multicore
#define map_between(a,b,c) (bool)( (a >= b) && (a <= c))

#ifdef thermistor

ADC_Instance adc_thermistor = {
  .adc_instance = 0,
  .pinum = 26,
  .mapped = 0,
  .miin = 0,
  .main = 4095,
  .miout = 0,
  .maout = 2047,
  .raw_avg = 0
};

ADC_Instance adc_thermistor1 = {
  .adc_instance = 1,
  .pinum = 22,
  .mapped = 0,
  .miin = 0,
  .main = 4095,
  .miout = 0,
  .maout = 2047,
  .raw_avg = 0
};

//  These values are for using the steinhart-hart equation for a thermistor.
//  This is the known resistor value.  Currently a 10k ohm.
#define v_div 10000

//  This is our known voltage in source.
#define v_in 5

//  These coefficients were measured august 21 2025, using the average of 6 thermistors values.
#define co_A   (double).000599
#define co_B   (double).00033411
#define co_C   (double)-.00000037309

#define ln_one(a)     (log(a))
#define ln_two(a, b)  (pow(log(a), b))

#define adc_Clk 48000000
#define Adc_Freq 10000

double map_inverse(uint16_t mapped, uint16_t max_map){
  printf("Mapping Inverse.\n");
  double output = 0;
    output = (((double)mapped / max_map) * v_in);
  printf("Mapped Inverse: %lf.\n", output);
  return output;
}


double get_celsius(double kelvin){
//  printf("Converting From Celsius.\n");
  double celsius = (kelvin - 273.15);
//  printf("\tCelsius: %.2f.\n", celsius);
  return (celsius);
}


double get_fahrenheit(double kelvin){
//  printf("Converting To Fahrenheit.\n");
  double fahrenheit = (get_celsius(kelvin)*((double)9/(double)5)) + 32;
//  printf("\tFahrenheit: %.2f.\n\n", fahrenheit);
  return (fahrenheit);
}


double get_resistance(double adc_input){
//  printf("\nGetting ADC Resistance.\n");
    double resistance = 0;
  //    printf("V_div: %i.\n", v_div);
      printf("\tAdc_Input: %lf.\n", adc_input);
  //    printf("V_in: %i.\n", v_in);
      resistance = v_div * ((adc_input)/(4095 - adc_input));
//  printf("\t\tCalulated Resistance: %lf.\n\n", resistance);
    return resistance;
}


double steinhart_hart(double rizzin){
//  printf("Performing Steinhart-Hart Equation.\n");
  double new_temp = 0;
  double coa = co_A;
  double cob = co_B;
  double coc = co_C;
    new_temp = (1 / ( coa + cob * (ln_one(rizzin) + (coc * ln_two(rizzin, 3))) ));
//  printf("\tSteinhart-Hart Result In Kelvin: %lf.\n\n", new_temp);
  return new_temp;
}

#endif



// ===== Functions Below ========

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
uint32_t map(uint16_t x, uint16_t in_min, uint16_t in_max, uint16_t out_min, uint16_t out_max) {
    return (uint32_t)((x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min);
}


void easy_map(ADC_Instance *input){
  input->mapped = map(input->raw[0], input->miin, input->main, input->miout, input->maout);
}


//  Just sums and averages the ADC poll.
void transform_adc_poll(ADC_Instance *alio){

  uint32_t poll_sum = 0x00000000;
  for(uint8_t a = 0; a < 32; a++){
    poll_sum += alio->raw[a];
  }
  poll_sum /= 32;
  alio->raw_avg = (poll_sum & 0x0000FFFF);

}


//  Use ADC_Obj_s as the main configurant instead of the singular adc_instance.
void poll_adc_pin(ADC *trio, uint8_t adc_num) {
  //  this wee variable will just be for our raw incrementing values
  uint8_t a = 0;

  printf("Polling ADC%i.\n",adc_num);
  switch(adc_num){
    case 0:
      if(trio->ADC_1.pinum != UNDEFINED){
        adc_select_input(0);
          while(a < 32){
            trio->ADC_1.raw[a] = adc_read();
            a++;
          } 
      }else {
          printf("\tADC Port 1: No Value.\n");
      }
      a = 0;

    transform_adc_poll(&trio->ADC_1);
    break;

    case 1:
      if(trio->ADC_2.pinum != UNDEFINED){
        adc_select_input(1);
          while(a < 32){
            trio->ADC_2.raw[a] = adc_read();
            a++;
          } 
      }else {
          printf("\tADC Port 2: No Value.\n");
      }
      a = 0;
      transform_adc_poll(&trio->ADC_2);
    break;

    case 2:
      if(trio->ADC_3.pinum != UNDEFINED){
        adc_select_input(2);
          while(a < 32){
            trio->ADC_3.raw[a] = adc_read();
            a++;
          } 
      }else {
          printf("\tADC Port 3: No Value.\n");
      }
      a = 0;
      transform_adc_poll(&trio->ADC_3);
    break;

    default:

      printf("ADC Pin Call was weirdly called with no ADC_Obj_s loaded.. weird.\n");
    break;


  }
 
};


//  map single adc read
void single_call_map(ADC_Instance *adc){
  adc->raw[0] = map(adc->raw[1], adc->miin, adc->main, adc->miout, adc->maout);
}


// The raw adc value gets called to raw[1], the mapped value will be in raw[0]
void single_call_adc(ADC_Instance *adc){
  adc_select_input(adc->adc_instance);
  adc->raw[1] = adc_read();
  single_call_map(adc);
}


//  Use ADC_Instance struct to call configued adc setup.
void adc_pin_call(ADC_Instance *alio, uint8_t raw_position) {
    adc_select_input(alio->adc_instance);
      alio->raw[raw_position] = adc_read();
};


//  Adjustable size return with the vari_size union.
vari_size map_adc_num(ADC_Instance *alio){
  vari_size mapped_to_size;

    transform_adc_poll(alio);

    mapped_to_size.u8 = (map(alio->raw_avg, alio->miin, alio->main, alio->miout, alio->maout) & 0x000000FF);
    mapped_to_size.u16 = (map(alio->raw_avg, alio->miin, alio->main, alio->miout, alio->maout) & 0x0000FFFF);
    mapped_to_size.u32 = (map(alio->raw_avg, alio->miin, alio->main, alio->miout, alio->maout) & 0xFFFFFFFF);

  return mapped_to_size;
}


//  Returns the 8bit value remainder of of the adc mapping.
uint8_t map_adc_8bit(ADC_Instance *alio){
  vari_size mapped_to_8;
    mapped_to_8 = map_adc_num(alio);
  return mapped_to_8.u8;
}


//  Returns the 16bit value of the adc mapping
uint16_t map_adc_16bit(ADC_Instance *alio){
  vari_size mapped_to_16;
    mapped_to_16 = map_adc_num(alio);
  return mapped_to_16.u16;
}


//  Doesn't actually map a 32bit, as the pi pico adc is only good for about 10 bit
//  so just good for peace of mind buffering I guess
uint32_t map_adc_32_bit(ADC_Instance *alio){
  vari_size mapped_to_32;
    mapped_to_32 = map_adc_num(alio);
  return mapped_to_32.u32;
}


//  Setups a single pico ADC pin.
// analog io -> al io -> alio
void setup_ADC_pin(ADC_Instance *alio) {
  printf("\nADC Pin Initialization.\n");

  if(alio->pinum != UNDEFINED){
    adc_gpio_init(alio->pinum);
      printf("Analog_To_Digital Pin: %i : Initialized.\n", alio->pinum);
  } else {
      printf("ADC0 Not Initialized.\n");
  }
}


//  Setups any of the configured ADC_Obj_s configured.
void setup_ADC_pin_X(ADC *trio){
  trio->adc1_status = false;
  trio->adc2_status = false;
  trio->adc3_status = false;

  if(trio->ADC_1.pinum != UNDEFINED){
      setup_ADC_pin(&trio->ADC_1);
      trio->adc1_status = true;
  }else{
    printf("ADC 1 not defined.\n");
  }

  if(trio->ADC_2.pinum != UNDEFINED){
      setup_ADC_pin(&trio->ADC_2);
      trio->adc2_status = true;
  }else{
    printf("ADC 2 not defined.\n");
  }

  if(trio->ADC_3.pinum != -1){
      setup_ADC_pin(&trio->ADC_3);
      trio->adc3_status = true;
  }else{
    printf("ADC 3 not defined.\n");
  }

}


void ADC_Single_Initialisation(ADC_Instance *alio){
  setup_ADC_pin(alio);
}

void ADC_X_Initialisation(ADC *trio){
  setup_ADC_pin_X(trio);
}

//  puts the temp result into raw[0] per instance used.
//  uncomment if not using repeating timer
double temp_result(ADC_Instance *alio){
  //  puts the temp result into raw[0]
  double temp_buff = 0x00;
  single_call_adc(alio);
  temp_buff = get_fahrenheit(steinhart_hart(get_resistance(alio->raw[0])));
  return temp_buff;
}


#ifdef run_multicore

repeating_timer_t adc_caller;

ADC joy_sticks = {

  .ADC_1 = {
    .adc_instance = 0,
    .pinum = 26,
    .mapped = 0,
    .miin = 0,
    .main = 4095,
    .miout = 0,
    .maout = 2047,
    .raw_avg = 0
  },

  .ADC_2 = {
    .adc_instance = 1,
    .pinum = 27,
    .mapped = 0,
    .miin = 0,
    .main = 4095,
    .miout = 0,
    .maout = 2047,
    .raw_avg = 0
  },

  .ADC_3 = {

    .adc_instance = 2,
    .pinum = 28
  }

};


void calling_adc(ADC_Instance *adc){
  adc_select_input(adc->adc_instance);
  adc->raw[1] = adc_read();
 // return true;
}


// repeating timer callback for adc_call
bool read_adc(repeating_timer_t *rt){
  calling_adc(&joy_sticks.ADC_1);
  calling_adc(&joy_sticks.ADC_2);
  return true;
}


// repeating timer callback for adc_call
bool read_adc_therm(repeating_timer_t *rt){
  calling_adc(&adc_thermistor);
  return true;
}

/*
void core1_entry(){

  stdio_init_all();
    printf("Core 2 Launched Successfully.\n");
  adc_init();
  ADC_Single_Initialisation(&adc_thermistor);
//  add_repeating_timer_ms(500, read_adc, NULL, &adc_caller);

  while(true){

    printf("Room Temperature: %f.\n\n", temp_result(&adc_thermistor));
      sleep_ms(500);
  }
  tight_loop_contents();
}
*/
#endif


/*
uint8_t stick_menu(adc_port_values *val, uint8_t prev_sel){
    uint8_t selector_buf = prev_sel;
      printf("Previous Menu Choice: %i.\n", prev_sel);

  adc_pin_call(val);
    printf("ADC1 Mapped Value: %i.\n", val->adc1_mapped_value);

    if(map_between(val->adc1_mapped_value, 0, 105)){
        printf("Menu UP.\n");
      if(selector_buf > 0){
        selector_buf--;
      }else{
        printf("Top Of Menu.\n");
      }
    }else if(map_between(val->adc1_mapped_value, 170, 254)){
        printf("Menu UP.\n");
      if(selector_buf < 4){
        selector_buf++;
      }else{
        printf("Bottom Of Menu.\n");
      }
    }else{
      printf("Menu Choice Ideling.\n");
    }
    printf("Current Menu Selection: %i.\n", selector_buf);
  sleep_ms(5);

  return selector_buf;
}

uint8_t analog_menu(adc_port_values *val, uint8_t menu_max){
  uint8_t selector_buf = 0;

  adc_pin_call(val);
    printf("ADC1 Mapped Value: %i.\n", val->adc1_mapped_value);

    if(map_between(val->adc1_mapped_value, 0, 105)){
        printf("Menu UP.\n");
      if(selector_buf > 0){
        selector_buf--;
      }else{
        printf("Top Of Menu.\n");
      }
    }else if(map_between(val->adc1_mapped_value, 170, 254)){
        printf("Menu UP.\n");
      if(selector_buf < menu_max){
        selector_buf++;
      }else{
        printf("Bottom Of Menu.\n");
      }
    }else{
      printf("Menu Choice Ideling.\n");
    }
    printf("Current Menu Selection: %i.\n", selector_buf);
  sleep_ms(5);

  return selector_buf;
}
*/


#endif
