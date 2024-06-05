#ifndef ULTRA_SONIC_SENSOR_S_H
#define ULTRA_SONIC_SENSOR_S_H

#include <stdio.h>
#include "pico_pin_enum.h"
#include "pico/stdlib.h"
#include "hardware/gpio.h"


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
/*
        // Wait for the echo pulse
        while (gpio_get(data->echo_two) == 0) {}
        data->usonic_one_start = time_us_32();

        while (gpio_get(data->echo_two) == 1) {}
        data->usonic_one_end = time_us_32();

        // Calculate the distance
        data->pulse_duration_two = data->usonic_two_end - data->usonic_two_start;
        data->distance_two = (data->pulse_duration_two / 2.0) * 0.0343; // Speed of sound is 343 m/s
        printf("Distance: %.2f cm\n", data->distance_two);
        data->returned_distance_two = data->distance_two;
*/
    }
}




#endif