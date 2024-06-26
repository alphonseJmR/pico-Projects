#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "pico/stdlib.h"
#include "Poly_Tools.h"

#define client_receiver
uint8_t nrf_initial_loop;
//  nrf_NCS = No Connections
//  nrf_RAN = Restart Attempt Nums
uint8_t nrf_NCS, nrf_RAN;
bool nrf_reset;
uint16_t previous_position;

uint16_t sent_vertical_value;
uint16_t sent_horizontal_value;

uint8_t pipe_number;


pin_manager_t my_pins = { 

    .sck = GPIO_EIGHTEEN,
    .mosi = GPIO_NINETEEN, 
    .miso = GPIO_SIXTEEN, 
    .csn = GPIO_SEVENTEEN, 
    .ce = GPIO_FIFTEEN

};

servo_motor my_servo = {

  .my_perv.servo_pin_one = GPIO_TEN,
  .my_perv.servo_pin_two = UNDEFINED,

  .my_serv.servo_wrap_one = 39062.f,
  .my_serv.servo_one_level = 0,

  .my_serv.servo_middle_ground = 752

};

dc_motor my_dc = {

  .my_dc_pins.motor_one_forward = GPIO_TWELVE,
  .my_dc_pins.motor_one_reverse = GPIO_THIRTEEN,

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

payload_data my_loads = {

  .ready_load.load_zero = 0,
  .ready_load.load_vert_data.vert_upper = 0,
  .ready_load.load_vert_data.vert_lower = 0,
  .ready_load.load_hori_data.hori_upper = 0,
  .ready_load.load_hori_data.hori_lower = 0,
  .ready_load.load_two = 0,
  .ready_load.load_three = 0,

  .payload_buffer.load_zero_buffer = 0,
  .payload_buffer.load_vert_buffer.vert_upper_buf = 0,
  .payload_buffer.load_vert_buffer.vert_lower_buf = 0,
  .payload_buffer.load_hori_buffer.hori_upper_buf = 0,
  .payload_buffer.load_hori_buffer.hori_lower_buf = 0,
  .payload_buffer.load_two_buffer = 0,
  .payload_buffer.load_three_buffer = 0

};

nrf_manager_t my_config = {

    // RF Channel 
    .channel = 110,

    // AW_3_BYTES, AW_4_BYTES, AW_5_BYTES
    .address_width = AW_5_BYTES,

    // dynamic payloads: DYNPD_ENABLE, DYNPD_DISABLE
    .dyn_payloads = DYNPD_ENABLE,

    // data rate: RF_DR_250KBPS, RF_DR_1MBPS, RF_DR_2MBPS
    .data_rate = RF_DR_250KBPS,

    // RF_PWR_NEG_18DBM, RF_PWR_NEG_12DBM, RF_PWR_NEG_6DBM, RF_PWR_0DBM
    .power = RF_PWR_NEG_18DBM,

    // retransmission count: ARC_NONE...ARC_15RT
    .retr_count = ARC_10RT,

    // retransmission delay: ARD_250US, ARD_500US, ARD_750US, ARD_1000US
    .retr_delay = ARD_500US 
  
};

void set_readied_values(payload_data *load){


    if(nrf_NCS > 10){

      my_loads.payload_buffer.load_vert_buffer.vert_upper_buf = 0x01;
      my_loads.payload_buffer.load_vert_buffer.vert_lower_buf = 0xF9;
      my_loads.payload_buffer.load_hori_buffer.hori_upper_buf = 0x01;
      my_loads.payload_buffer.load_hori_buffer.hori_lower_buf = 0xF9;


      sent_vertical_value = ((load->payload_buffer.load_vert_buffer.vert_upper_buf << 8) | (load->payload_buffer.load_vert_buffer.vert_lower_buf));
      sent_horizontal_value = ((load->payload_buffer.load_hori_buffer.hori_upper_buf << 8) | (load->payload_buffer.load_hori_buffer.hori_lower_buf));
      printf("\nVertical Data: 0x%04x.", sent_vertical_value);
      printf("\nHorizontal Data: 0x%04x.\n", sent_horizontal_value);

    }else{

      sent_vertical_value = ((load->payload_buffer.load_vert_buffer.vert_upper_buf << 8) | (load->payload_buffer.load_vert_buffer.vert_lower_buf));
      sent_horizontal_value = ((load->payload_buffer.load_hori_buffer.hori_upper_buf << 8) | (load->payload_buffer.load_hori_buffer.hori_lower_buf));
      printf("\nVertical Data: 0x%04x.", sent_vertical_value);
      printf("\nHorizontal Data: 0x%04x.\n", sent_horizontal_value);

    }
}

void set_servo_turn(uint16_t horizontal_data){

  printf("\n////\tSet Servo Turn\t////\n");

  bool idle_turn;
  idle_turn = false;

  printf("///\tPrevious Position: %i.\t///\n", previous_position);

    idle_turn = input_Test(495, 515, horizontal_data);
  
  if(!idle_turn){
    (!idle_turn) ? printf("Servo Turning.\n") :  printf("Servo Turn Idle\n");

printf("Re-Enabling Servo PWM");
    pwm_set_enabled(my_servo.my_serv.servo_slice_one, true);
      sleep_us(200);

    if(horizontal_data > 525){
        printf("Horizontal Data: %i.\n", horizontal_data);
      set_servo_position(10, &my_servo, 2300);

    }else if(horizontal_data < 485){
      set_servo_position(10, &my_servo, 550);
        printf("Horizontal Data: %i.\n", horizontal_data);
    }

    previous_position = horizontal_data;

  }else{

      if(previous_position != 1450){
    
        set_servo_position(10, &my_servo, 1450);
        //  pwm_set_counter(my_servo.my_serv.servo_slice_one, 0);
        sleep_us(200);
          pwm_set_enabled(my_servo.my_serv.servo_slice_one, false);
        previous_position = 1450;

      }else {

            printf("Horizontal Data Irrelevent.\n");

      }


  }
    
}

void nrf_client_configured(nrf_client_t *client, pin_manager_t *pins, nrf_manager_t *config, uint32_t baudrate){

  printf("Configuring NRF Client.\n");
  uint8_t nrf_status;
  uint64_t time_sent, time_reply;

  time_sent, time_reply = 0;
  nrf_status = 0;


#ifdef client_transmitter

nrf_status = nrf_driver_create_client(client);
  printf("NRF Transmitter Client Created.\n");

client->configure(pins, baudrate);
client->initialise(config);

client->standby_mode();

#endif

#ifdef client_receiver

nrf_status = nrf_driver_create_client(client);
  printf("NRF Reciever Client Created.\n");

client->configure(pins, baudrate);
client->initialise(config);

client->rx_destination(DATA_PIPE_0, (uint8_t[]){0x37,0x37,0x37,0x37,0x37});
client->rx_destination(DATA_PIPE_1, (uint8_t[]){0xC7,0xC7,0xC7,0xC7,0xC7});
client->rx_destination(DATA_PIPE_2, (uint8_t[]){0xC8,0xC7,0xC7,0xC7,0xC7});
client->rx_destination(DATA_PIPE_3, (uint8_t[]){0xC9,0xC7,0xC7,0xC7,0xC7});

client->receiver_mode();

#endif


}

void nrf_check_received(nrf_client_t *client, payload_data *loads){

  uint8_t success;
  success = 0;

  if(client->is_packet(&pipe_number)){
    printf("Pipeline: #%i\n", &pipe_number);

  switch (pipe_number){

    case DATA_PIPE_0:
      // read payload
        client->read_packet(&loads->payload_buffer.load_zero_buffer, sizeof(loads->payload_buffer.load_zero_buffer));
        loads->ready_load.load_zero = loads->payload_buffer.load_zero_buffer;

        printf("\nPacket received:- Payload (0x%04x) on data pipe (%d)\n", loads->ready_load.load_zero, pipe_number);
        nrf_NCS = 0;
      break;

      case DATA_PIPE_1:
        // read payload
        success = client->read_packet(&loads->payload_buffer.load_vert_buffer, sizeof(loads->payload_buffer.load_vert_buffer));
        if(success){
        printf("\nPacket received:- Payload 0x%02x | Payload 0x%02x on data pipe (%d)\n", (loads->payload_buffer.load_vert_buffer.vert_upper_buf), (loads->payload_buffer.load_vert_buffer.vert_lower_buf), pipe_number);
        }else {
          printf("\n//\tVertical Data Not Received.\t//\n");
          loads->payload_buffer.load_vert_buffer.vert_upper_buf = 0;
          loads->payload_buffer.load_vert_buffer.vert_lower_buf = 0;
        }
        nrf_NCS = 0;
      break;
        
      case DATA_PIPE_2:
        // read payload
        success = client->read_packet(&loads->payload_buffer.load_hori_buffer, sizeof(loads->payload_buffer.load_hori_buffer));
        if(success){
          printf("\nPacket received:- Payload 0x%02x | Payload 0x%02x on data pipe (%d)\n", (loads->payload_buffer.load_hori_buffer.hori_upper_buf), (loads->payload_buffer.load_hori_buffer.hori_lower_buf), pipe_number);
        }else {
          printf("\n//\tHorizontal Data Not Received.\t//\n");
          loads->payload_buffer.load_hori_buffer.hori_upper_buf = 0;
          loads->payload_buffer.load_hori_buffer.hori_lower_buf = 0;
        }
        nrf_NCS = 0;
      break;
        
      case DATA_PIPE_3:
        // read payload
        client->read_packet(&loads->payload_buffer.load_three_buffer, sizeof(loads->payload_buffer.load_three_buffer));
        loads->ready_load.load_three = loads->payload_buffer.load_three_buffer;
        printf("\nPacket received:- Payload (0x%04x) on data pipe (%d)\n", loads->ready_load.load_three, pipe_number);
        nrf_NCS = 0;
      break;
        
      case DATA_PIPE_4:
      break;
        
      case DATA_PIPE_5:
      break;
        
      default:
      break;
        }

} 
    else {
  printf("No Data Received.\n\n");
  nrf_NCS++;
  
}

}


int main () {
  
  int pin_init;
  pin_init = 0;
  nrf_reset = false;
  nrf_initial_loop = 1;
  nrf_NCS = 0;
  nrf_RAN = 0;
  previous_position = 0;

   stdio_init_all();

  printf("RC Car Program Initialising.");

  sleep_ms(7000);
  sleep_ms(7000);

my_dc = initialise_dc_pwm_pins(&my_dc);
    printf("\n\n");
      sleep_ms(5000);
servo_initialization(&my_servo);
    printf("\n\n");
      sleep_ms(5000);

  // provides access to driver functions
  nrf_client_t my_nrf;
  pipe_number = 0;

  nrf_client_configured(&my_nrf, &my_pins, &my_config, 5000000);


//  set_servo_initial_position(&my_servo);

while(1){

  if(nrf_initial_loop == 1){

      printf("NRF Client Active.\n");
      nrf_reset = false;
      nrf_initial_loop = 0;

  }else{

    nrf_client_configured(&my_nrf, &my_pins, &my_config, 5000000);
    printf("NRF Client Re-Activated.\n");
    nrf_reset = false;
    printf("////\tNum of Resets: %i.\t////\n", nrf_RAN);
      sleep_ms(1000);
  }
  
  while(!nrf_reset){

    printf("\n\n////\tMain Looping\t////\n\n");
    sleep_ms(150);
    nrf_check_received(&my_nrf, &my_loads);

    if(nrf_NCS > 20){
      nrf_reset = true;
      nrf_RAN++;
      nrf_NCS = 0;
    }else{
      printf("\n////\tNo Connection Attempts: %i\t////\n", nrf_NCS);
    }

    set_readied_values(&my_loads);

    set_motor_values(&my_dc, sent_vertical_value, my_loads.ready_load.load_zero);
    set_servo_turn(sent_horizontal_value);

  }
}
tight_loop_contents();
}


