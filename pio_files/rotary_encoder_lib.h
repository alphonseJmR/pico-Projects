#ifndef ROTARY_ENCODER_PIO_LIBRARY_S
#define ROTARY_ENCODER_PIO_LIBRARY_S

#include "hardware/pio.h"
#include "pico/time.h"

#define separate_r1(d) (uint8_t)( (d & 0x0003))
#define separate_r2(d) (uint8_t)( ((d & 0x000C) >> 2) & 0x0003)
#define separate_r3(d) (uint8_t)( ((d & 0x0030) >> 4) & 0x0003)

/*

//  Running the repeating timer at 2.5ms has allowed best input so far.
//  Currently testing if the pio program speed affects it much at a clk_div
//  of around 1250.

*/

// generic timer for rotary encoder input-r
repeating_timer_t reir_timer;


// Holds PIO and pins data for rotary_encoder_lib
typedef struct generic_hc165_s {
  PIO re_port;
  uint re_SM;
  uint re_offset;
  uint8_t input;
  uint8_t latch;
  uint8_t clk_inh;
  uint8_t clk;
  //  this is the same as reg_return in the reir generic setup
  volatile uint32_t reg_ret;
}gen_reg;


// Rotary encoder struct setup
typedef struct rotary_encoder_input_r {
  gen_reg info;
  volatile uint32_t reg_return;
  volatile int16_t pre_re1;
  volatile int16_t pre_re2;
  volatile int16_t pre_re3;
  volatile int16_t re1_v;
  volatile int16_t re2_v;
  volatile int16_t re3_v;
  //  The delta's are the amount we want each encoder to change value by.
  //  We should use const, but lets try without it
   uint16_t delta1;
   uint16_t delta2;
   uint16_t delta3;

  volatile bool switch1;
  volatile bool switch2;
}reir;


//  this is the generic setup for the rotary encoder
reir generic_setup = {
    .info = {
      .re_port = pio1,
      .re_SM = 0,
      .input = 0,
      .latch = 1,
      .clk_inh = 2,
      .clk = 3,
    },
    .reg_return = 0,
    .delta1 = 5,
    .delta2 = 5,
    .delta3 = 5,
    .switch1 = false,
    .switch2 = false
};


//  returns 1 or -1 based on bit pattern
int16_t rotary_logic(uint8_t A, uint8_t B){

//  printf("Rotary Logic Testing: 0x%02x vs 0x%02x.\n", A, B);
  //  (A & 0x01) ? printf("A High\n") : printf("A Low\n");
  //  (A & 0x02) ? printf("B High\n") : printf("B Low\n");

      if( (B == 0x03)){
  //        printf("B Equal.\n");

        if( (A == 0x02)){
  //          printf("Increment.\n");
            return 1;
          }else if( (A == 0x01)){
  //          printf("Decrement.\n");
            return -1;
          }

    }
      return 0;
}


int8_t pcb_rotary_logic(uint8_t vtt, uint8_t sre){
  switch(sre){
    case 1:
      if(vtt == 0x1F){
        return 1;
      }else if(vtt == 0x2F){
        return -1;
      }else{}
    break;

    case 2:
      if(vtt == 0x33){
        return 1;
      }else if(vtt == 0x3B){
        return -1;
      }else{}
    break;

    case 3:
      if(vtt == 0x3C){
        return 1;
      }else if(vtt == 0x3E){
        return -1;
      }else{}
  }
}


//  rep = rotary eval portion.  ris = rotary input switcher
//  amount should be a positive value in the params.
void set_rotary_value(reir *input, uint8_t rep, uint8_t ris, int8_t amount){
  int8_t buff = 0;
  switch(ris){
    case 1:
     if(rep != input->pre_re1){
      buff = rotary_logic(rep, input->pre_re1);
      if(buff != 0){
        input->re1_v += ((buff == 1) ? amount : -amount);
      }
     }
     break;

     case 2:
     if(rep != input->pre_re2){
      buff = rotary_logic(rep, input->pre_re2);
      if(buff != 0){
        input->re2_v += ((buff == 1) ? amount : -amount);
      }
     }
     break;

     case 3:
     if(rep != input->pre_re3){
      buff = rotary_logic(rep, input->pre_re3);
      if(buff != 0){
        input->re3_v += ((buff == 1) ? amount : -amount);
      }
     }
     break;

     default:
      printf("No rotary encoder selected to set value.\n");
      break;
  }
}


// produces correct return for input error / my error.
int16_t rotary_error_correction(reir *input){
    uint8_t inc_check = 0x00;
    uint16_t boofer = input->reg_return;
    printf("Register Return: 0x%04x.\n", input->reg_return);
//  printf("Checking Return: 0x%02x\n", boofer);
  uint8_t known_errors[4] = {0x36, 0x39, 0x2A, 0x24};

  switch(boofer){
    case 0x24:
  //  printf("returning: 0x2F\n");
      return 0x2F;
    break;

    case 0x25:
  //  printf("returning: 0x2F\n");
      return 0x2F;
    break;

    case 0x29:
  //  printf("returning: 0x2F\n");
      return 0x2F;
    break;

    case 0x2A:
  //  printf("returning: 0x2F\n");
      return 0x2F;
    break;

    case 0x33:
      return 0x37;

    case 0x36:
  //  printf("returning: 0x37\n");
      return 0x37;
    break;

    case 0x39:
  //  printf("returning: 0x3B\n");
      return 0x3B;
    break;

    case 0x3C:
  // printf("returning: 0x3C\n");
      return 0x3D;
    break;

    case 0xBE:
  //  printf("returning: 0x3E\n");
      return 0x3E;

    default:
      return boofer;
    break;
      // printf("acceptable\n");
  }
}


//  previously crementation
void rotary_gauge(reir *input){
  //  default return with no indents set - 0x3F
//  printf("Gauging Rotary Input.\n");
  volatile  uint8_t a, b, c = 0;
  volatile  uint16_t reg_buf = rotary_error_correction(input);

  if(input->reg_return & 0x40){
    input->switch1 = true;
  }
  if(input->reg_return & 0x80){
    input->switch2 = true;
  }

//  printf("Checked Return: 0x%02x\n", reg_buf);
  int8_t q, y, z = 0;

    a = separate_r3(reg_buf);
//    printf("A: 0x%02x\n", a);
  
    b = separate_r2(reg_buf); 
//    printf("B: 0x%02x\n", b);
  
    c = separate_r1(reg_buf);
//    printf("C: 0x%02x\n\n", c);
//    printf("\n");

  set_rotary_value(input, a, 1, input->delta1);
  set_rotary_value(input, b, 2, input->delta2);
  set_rotary_value(input, c, 3, input->delta3);

    input->pre_re1 = a;
    input->pre_re2 = b;
    input->pre_re3 = c;
}


// helper function to pull the fifo into the reg_return struct variable.
bool fifo_pulled(reir *input){

  if(pio_sm_get_rx_fifo_level(input->info.re_port, input->info.re_SM) > 0){
    input->reg_return = pio_sm_get(input->info.re_port, input->info.re_SM);
  }else{
    input->reg_return = 0;
  }
    rotary_gauge(input);
  return true;
}


// loaded with "generic_setup" reir struct
bool get_rot_pos(repeating_timer_t *rt){
  return fifo_pulled(&generic_setup);
}


/*
  run_timer = add_repeating_timer_us(500, get_rot_pos, NULL, &rqt_tests);
    if(!run_timer){
      printf("Repeating Timer Error.\nEnd Program\n");
      prb = false;
    }else{
      printf("Repeating Timer Started.\n");
    }
*/


#endif