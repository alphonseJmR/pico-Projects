#ifndef GENERIC_USE_FUNCTION_S_H
#define GENERIC_USE_FUNCTION_S_H

#include <stdio.h>
#include <math.h>
#include <stdint.h>

#define two_pi (3.141596*2)

typedef struct sin_sound_s {

    uint16_t amplitude;
    uint16_t period;
    uint16_t phase;
    uint8_t evaluated_out[8];

}sin_s;

sin_s dot_matrix = {
    
    .amplitude = 16,
    .period = 255,
    .phase = 2,
    .evaluated_out = {
        0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00
    }

};

uint8_t sin_eval(sin_s *that, uint16_t x_input){
    if(x_input <= 5){
        x_input = 10;
    }
    printf("Input Value: %i.\n", x_input);
    double output = abs(((that->phase)*(sin(two_pi * x_input / that->period) * that->amplitude)));
    printf("Sin Value: %lf.\n", output);
    return (uint8_t)output;
}





#endif