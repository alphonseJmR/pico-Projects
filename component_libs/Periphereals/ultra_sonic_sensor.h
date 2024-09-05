#ifndef ULTRA_SONIC_SENSOR_S_H
#define ULTRA_SONIC_SENSOR_S_H

#include <stdio.h>
#include "resources/pico_pin_enum.h"
#include "../hi_lvl_resources/project_struct_s.h"
#include "pico/stdlib.h"
#include "hardware/gpio.h"

#define calc_distance(a) (double)((a / 2.0) * 0.0343)

// #define common_use
#define turret_use

#ifdef common_use

ultra_sonic_data_t usonic;

void ultra_sonic_sensor_pin_setup(ultra_sonic_data_t *pins) {
  if(pins->trigger_one != UNDEFINED){
    gpio_init(pins->trigger_one);
    gpio_set_dir(pins->trigger_one, GPIO_OUT);
    printf("Ultra Sonic Trigger One:\tInitialized.\n");
  }else {
    printf("Ultra Sonic Trigger One:\tNot Initialized.\n");
  }

  if(pins->echo_one != UNDEFINED){
    gpio_init(pins->echo_one);
    gpio_set_dir(pins->echo_one, GPIO_IN);
    printf("Ultra Sonice Echo One:\tInitialized.\n");
  }else {
    printf("Ultra Sonice Echo One:\tNot Initialized.\n");
  }
  if(pins->echo_two != UNDEFINED){
    gpio_init(pins->echo_two);
    gpio_set_dir(pins->echo_two, GPIO_IN);
    printf("Ultra Sonice Echo Two:\tInitialized.\n");
  }else {
    printf("Ultra Sonice Echo Two:\tNot Initialized.\n");
  }
}

void ultra_sonic_distance(ultra_sonic_data_t *data){

    printf("\n\tUltrasonic Sensor Function.\n");

    if((data->trigger_one != -1) && (data->echo_one != -1)){

        gpio_put(data->trigger_one, 1);
        sleep_us(10);
        gpio_put(data->trigger_one, 0);

        // Wait for the echo pulse
        while (gpio_get(data->echo_one) == 0 || gpio_get(data->echo_two) == 0) {}
        data->usonic_one_start = time_us_32();

        while (gpio_get(data->echo_one) == 1 || gpio_get(data->echo_two) == 1) {}
        data->usonic_one_end = time_us_32();

        // Calculate the distance
        data->pulse_duration_one = data->usonic_one_end - data->usonic_one_start;
        data->distance_one = (data->pulse_duration_one / 2.0) * 0.0343; // Speed of sound is 343 m/s
        printf("Distance: %.2f cm\n", data->distance_one);
        data->returned_distance_one = data->distance_one;
    }
}

#endif

#ifdef turret_use

void setup_hcsr04_sensor(hcsr04 *sensor){

  printf("Initializing HC-SR04 Ultra Sonic Sensor.\n");

    gpio_init(sensor->trigger_pin);
    gpio_set_dir(sensor->trigger_pin, GPIO_OUT);

    gpio_init(sensor->echo_pin);
    gpio_set_dir(sensor->echo_pin, GPIO_IN);

  printf("HC-SR04 Sensor Initialized.\n");

}

void send_trigger_pulse(hcsr04 *sensor){

  gpio_put(sensor->trigger_pin, 1);
    sleep_us(10);
  gpio_put(sensor->trigger_pin, 0);

}

void get_distance(hcsr04 *sensor){

//  printf("Detecting Distance.\n");

  send_trigger_pulse(sensor);

  while(gpio_get(sensor->echo_pin) == 0){}
    sensor->time_start = time_us_32();
    //  printf("Sensor Start Time: %i.\n", sensor->time_start);
  while(gpio_get(sensor->echo_pin) == 1){}
    sensor->time_end = time_us_32();
    //  printf("Sensor Detect Time: %i.\n", sensor->time_end);

  sensor->pulse_duration = (sensor->time_end - sensor->time_start);
  //  printf("Measured Pulse Duration: %i.\n", sensor->pulse_duration);
      sleep_ms(100);
    if(sensor->pulse_duration > 2000){
  //    printf("Distance Out of range.\n");
      sensor->distance = 0;
    }else{
  sensor->distance = calc_distance(sensor->pulse_duration);
    printf("Current Distance: %.2f cm\n", sensor->distance);
    }
}

bool check_for_detection(hcsr04 *sensor){

    get_distance(sensor);

  if(sensor->distance >= 10){
    sensor->detection = true;
  }else {
  //  printf("\nNo Sensor Detection.\n\n");
    sensor->detection = false;
  }

  return sensor->detection;

}

#endif


#endif
