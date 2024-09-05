#include <stdio.h>
#include "stdlib.h"
#include "Poly_Tools.h"
#include "nrf24_driver.h"
#include "hardware/gpio.h"
#include "hardware/adc.h"
#include "pico/multicore.h"

#define mapped_between(a,b,c) (bool)( (a >= b) && (a < c))
#define vert_received ((turret_load.ready_load.load_vert_data.vert_upper << 8) | turret_load.ready_load.load_vert_data.vert_lower)
#define hori_received ((turret_load.ready_load.load_hori_data.hori_upper << 8) | turret_load.ready_load.load_hori_data.hori_lower)
#define raw_received  (turret_load.ready_load.raw_data.raw_lower)
#define raw_reset     ((turret_load.ready_load.raw_data.raw_lower = 0x00))
#define turret_turn (turret_load.ready_load.load_hori_data.hori_lower) 
#define non_itr -1

//  Laser Pattern # choices
#define lp_one   0x01
#define lp_two   0x02
#define lp_three 0x03


volatile uint16_t hori_fifo;
int total_steps;
volatile bool rx_two;
uint8_t turn_timer;

const uint8_t fpat_single_rotation[3] = {
  0x24, 0x12, 0x09
};

const uint8_t fpat_dualy_rotation[3]= {
  0x4D, 0x36, 0x1B
};
#define fpat_destruction_beam 0x3F

sg90 angled_turret = {
  .signal = GPIO_THREE,
  .center = 1500,   //  turret_tilt_angle[2],
  .min = 2100,      //  turret_tilt_angle[8],
  .max = 1300,      //  turret_tilt_angle[0],
  .clk_div = 64.f,
  .period_div = 39062,
  .m_slice = 0
};

//  NOT A SG90, but instead a MG996r servo.  Setting are the same though.
sg90 swivel_servo = {
  .signal = GPIO_FOUR,
  .center = 2343,
  .min = 781,
  .max = 3906,
  .clk_div = 64.f,
  .period_div = 39062,
  .m_slice = 0
};


register_lcd laser_pins = {
  .lcd_reg_data = GPIO_TEN,
  .lcd_reg_latch = GPIO_ELEVEN,
  .lcd_reg_enable = GPIO_TWELVE,
  .lcd_reg_clk = GPIO_THIRTEEN,
  .lcd_rs = UNDEFINED,
  .lcd_e = UNDEFINED
};

uint8_t weapon_selector(uint8_t pat_sel, uint8_t pat_itr){

  switch(pat_sel){
    case 1:
      printf("Firing Single Rotation.\n");
      return fpat_single_rotation[pat_itr];
    break;

    case 2:
    printf("Firing Dually Rotation.\n");
      return fpat_dualy_rotation[pat_itr];

    break;

    case 3:
    printf("Firing DESTRUCTION BEAM.\n");
      return fpat_destruction_beam;
    
    break;

    default:
      printf("Invalid weapon selector.\n");
    break;
  }
}

void l_reset(register_lcd *laser){
  send_nibble(laser, 0x00);
}

void l_fire(register_lcd *laser, uint8_t pattern, uint8_t fire_amt){

  uint8_t fire_pattern = 0x00;

  for(uint8_t a = 0; a < fire_amt; a++){
    if(pattern != 3){
      for(uint8_t b = 0; b < 3; b++){
        fire_pattern = weapon_selector(pattern, b);
          printf("\t\t\t\n\nRegister Firing: 0x%02x.\n", fire_pattern);
          send_nibble(laser, fire_pattern);
            sleep_ms(250);
      }
    }else{
      fire_pattern = weapon_selector(pattern, non_itr);
      printf("\t\t\t\n\nRegister Firing: 0x%02x.\n", fire_pattern);
      for(uint8_t c = 0; c < fire_amt; c++){
        send_nibble(laser, fire_pattern);
          sleep_ms(20);
        l_reset(laser);
          sleep_ms(250);
      }
    }
  }
  l_reset(laser);
}


void send_multicore_fifo(payload_data *data){

    if(multicore_fifo_rvalid() == false){
      multicore_fifo_push_blocking(data->ready_load.load_hori_data.hori_lower);
    }else {
      printf("Waiting for FIFO POP.\n");
    }
}

void receive_multicore_fifo(){

    hori_fifo = (multicore_fifo_pop_blocking() & 0x000F);
    printf("Rx'ed: 0x%04x.\n", hori_fifo);

}


void core1_entry(){

  nrf_client_t my_nrf;
  pipe_number = 0;
  turn_timer = 0;
  
  initialize_servo(&angled_turret);

  nrf_client_configured(&my_nrf, &my_pins, &my_config, 5000000);
  sleep_ms(500);

  while(1){
            printf("Normal NRF Operations\n");
    if(nrf_testing.nrf_initial_bypass){
      nrf_testing.nrf_initial_bypass = initial_bypass(&nrf_testing);
    }else {
      nrf_reset(&my_nrf, &my_pins, &my_config, 5000000, &nrf_testing);
    }

    while(!nrf_testing.reset_nrf){
      if(!(raw_received & 0x08)){
          printf("\n");
            //  printf("\t\t\t\t\t\nChecking PIPE: %d.\n", pipe_number);
            nrf_check_received(&my_nrf, &turret_load);
          send_multicore_fifo(&turret_load);
            sleep_us(500);
            printf("\n\n\tVertical Data: 0x%04x.\n", vert_received);
            printf("\tHorizontal Data: 0x%04x.\n", hori_received);
            printf("\tRaw Data: 0x%04x.\n\n", raw_received);
          angle_turret(&angled_turret, vert_received);
      }else{
          printf("Weapon Fire Signal Detected.\n");
        l_fire(&laser_pins, (raw_received & 0x03), 10);
        raw_reset;
      }
    }
  }

  //  Add Servo Setup, and add logic to set servo motor from this core.  core zero will just turn the stepper
  //  As well the laser firing.

}


int main() {

  hori_fifo = 0;
  rx_two = false;
  ctp = 2543;

  stdio_init_all();

  printf("Ball Turret: Receiver Initialising.\n");
//  Sleep arbitrary amount of seconds to allow user to connect to serial monitor.
      sleep_ms(7000);
    //  sleep_ms(7000);
      
    multicore_launch_core1(core1_entry);
      sleep_ms(500);
    setup_my_registers(&laser_pins);
      sleep_ms(500);
    initialize_servo(&swivel_servo);

while(1)
{
    if(multicore_fifo_rvalid()){
      receive_multicore_fifo();
        swivel_turret(&swivel_servo, hori_fifo);
    }else{
      hori_fifo = 0;
      printf("No data in FIFO yet.\n");
    }
      sleep_ms(25);

}
tight_loop_contents();
}
