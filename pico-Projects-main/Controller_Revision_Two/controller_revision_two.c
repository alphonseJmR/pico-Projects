#include <stdio.h>
#include "stdlib.h"
#include "Poly_Tools.h"
#include "nrf24_driver.h"
#include "hardware/gpio.h"
#include "hardware/adc.h"

#define client_transmitter
#define ebit uint8_t
bool nrf_reset;
uint8_t transmit_NCS, transmit_RAN;
uint8_t nrf_initial_bypass;
uint8_t char_val;
uint64_t time_sent, time_reply;
fn_status_t success = 0;
uint8_t function_status_checker;

uint8_t pipe_line_addr[4][5] = {
  {0x37,0x37,0x37,0x37,0x37},
  {0xC7,0xC7,0xC7,0xC7,0xC7},
  {0xC8,0xC7,0xC7,0xC7,0xC7},
  {0xC9,0xC7,0xC7,0xC7,0xC7}
};

register_pins my_regi = {

    .register_one_data = GPIO_TWELVE,
    .register_one_latch = GPIO_SIXTEEN,
    .register_one_enable = GPIO_SEVENTEEN,

    .register_two_data = GPIO_ZERO,
    .register_two_latch = UNDEFINED,
    .register_two_enable = UNDEFINED,

    .register_clk_line = GPIO_ONE
    
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

adc_port_values my_adc = {
    .adc0_pin = GPIO_TWENTY_SIX,
    .adc1_pin = GPIO_TWENTY_SEVEN,
    .adc2_pin = GPIO_TWENTY_EIGHT,

    .adc0_min_in_map_value = 0,       //  ADC0 Port will be the battery voltage detection.
    .adc0_max_in_map_value = 4096,    //  4096 is the max resolution we can expect.

    .adc1_min_in_map_value = 0,       //  ADC1 Port will route the vertical channel of the joystick into a mapped value we can use.
    .adc1_max_in_map_value = 4096,    //  4096 is the max resolution we can expect.

    .adc2_min_in_map_value = 0,       //  ADC2 Port will route the horizontal channel of the joystick into a mapped value we can use.
    .adc2_max_in_map_value = 4096,    //  4096 is the max resolution we can expect.

    .adc0_min_out_map_value = 0,      //  Usually a minimum of zero, but for say a servo, keeping it center may require non zero.  Also, the pico can't do negatives?
    .adc0_max_out_map_value = 10,     //  For the Battery voltage, we can try an arbirtraily low number, like 10.

    .adc1_min_out_map_value = 0,      //  Joystick input minimum of zero in our case.
    .adc1_max_out_map_value = 1024,   //  For joystick input lets set the cap at 1024

    .adc2_min_out_map_value = 0,      //  Joystick input minimum of zero in our case.
    .adc2_max_out_map_value = 1024,      //  For joystick input lets set the cap at 1024.
    
    .adc0_mapped_value = 0,
    .adc1_mapped_value = 0,
    .adc2_mapped_value = 0,
    .adc0_mapped_float = 0,
    .adc1_mapped_float = 0,
    .adc2_mapped_float = 0
};

pin_manager_t my_pins = { 

    .sck = GPIO_TWO,
    .mosi = GPIO_THREE, 
    .miso = GPIO_FOUR, 
    .csn = GPIO_FIVE, 
    .ce = GPIO_SIX 

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


void nrf_client_configured(nrf_client_t *client, pin_manager_t *pins, nrf_manager_t *config, uint32_t baudrate){

  printf("Configuring NRF Client.\n");
  uint8_t nrf_status;

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

void nrf_packet_transmit(nrf_client_t *client, payload_data *data, uint8_t address, uint8_t load_num){

  printf("\n////\tNRF Transmitting\t////\n");

  switch(load_num){

    case 1:

      client->tx_destination(pipe_line_addr[address]);
      time_sent = to_us_since_boot(get_absolute_time());
      success = client->send_packet(&data->payload_buffer.load_zero_buffer, sizeof(data->payload_buffer.load_zero_buffer));
      time_reply = to_us_since_boot(get_absolute_time());
    if(success) {
        transmit_NCS = 0;
      printf("Packet sent:- Response: %lluuS | Payload: 0x%04x\n", (time_reply - time_sent), data->payload_buffer.load_zero_buffer);
    }else{

      printf("\nPacket not sent:- Receiver not available.\n");
    }
    break;

  case 2:

      client->tx_destination(pipe_line_addr[address]);

      time_sent = to_us_since_boot(get_absolute_time());
      printf("Packet: Vertical Struct: 0x%04x\n", data->ready_load.load_vert_data);
      success = client->send_packet(&data->ready_load.load_vert_data, sizeof(data->ready_load.load_vert_data));
      time_reply = to_us_since_boot(get_absolute_time());
        if(success) {
          transmit_NCS = 0;
          printf("Packet sent:- Response: %lluuS | Payload Upper: 0x%02x | Payload Lower: 0x%02x\n", (time_reply - time_sent), (data->ready_load.load_vert_data.vert_upper), (data->ready_load.load_vert_data.vert_lower));
        }else{
          printf("\nPacket not sent:- Receiver not available.\n");
        }

    break;

  case 3:

      client->tx_destination(pipe_line_addr[address]);
      time_sent = to_us_since_boot(get_absolute_time());
      success = client->send_packet(&data->ready_load.load_hori_data, sizeof(data->ready_load.load_hori_data));
      time_reply = to_us_since_boot(get_absolute_time());

    if(success) {
        transmit_NCS = 0;
      printf("Packet sent:- Response: %lluuS | Payload Upper: 0x%02x | Payload Lower: 0x%02x\n", (time_reply - time_sent), data->ready_load.load_hori_data.hori_upper, data->ready_load.load_hori_data.hori_lower);
    }else{

      printf("\nPacket not sent:- Receiver not available.\n");
    }
    break;

  case 4:

      client->tx_destination(pipe_line_addr[address]);
      time_sent = to_us_since_boot(get_absolute_time());
      success = client->send_packet(&data->payload_buffer.load_three_buffer, sizeof(data->payload_buffer.load_three_buffer));
      time_reply = to_us_since_boot(get_absolute_time());
      
    if(success) {
        transmit_NCS = 0;
      printf("Packet sent:- Response: %lluuS | Payload: 0x%04x\n", (time_reply - time_sent), data->payload_buffer.load_three_buffer);
    }else{
      printf("\nPacket not sent:- Receiver not available.\n");
    }
    break;

  default:

    printf("Non-payload # selected.\n");
    break;

  }
  
  (success > 0) ? transmit_NCS = 0 : transmit_NCS++;

}

void nrf_rc_update(nrf_client_t* client){

  printf("Sending All Data to RC Car.\n");

    printf("\n// // //\tPayload Zero\t// // //\n");
    nrf_packet_transmit(client, &my_loads, 0, 1);
    printf("// // //\tPayload One\t// // //\n");
      nrf_packet_transmit(client, &my_loads, 1, 2);
    printf("// // //\tPayload Two\t// // //\n");
        nrf_packet_transmit(client, &my_loads, 2, 3);
    printf("// // //\tPayload Three\t// // //\n");
          nrf_packet_transmit(client, &my_loads, 3, 4);

    if(transmit_NCS > 59){
        nrf_reset = true;
        transmit_RAN++;
        transmit_NCS = 0;
      }else{
        printf("\n// //\tNo Connection Attempts: %i\t// //\n", transmit_NCS);
      }

}


int main() {


  function_status_checker = 0x00;
  nrf_reset = false;
  transmit_NCS, transmit_RAN = 0;
  nrf_initial_bypass = 1;

  stdio_init_all();

  printf("RC Controller Program Initialising.\n");
//  Sleep arbitrary amount of seconds to allow user to connect to serial monitor.
      sleep_ms(7000);
      sleep_ms(7000);

//  Initialise ADC PORT's.
      adc_init();
      adc_pin_setup(&my_adc);
        sleep_ms(200);

//  Initialize pin setup for rotary encoder.
      interruption_input_initialisation(&my_types);
        sleep_ms(200);

//  Initialize pin setup for cd74hc595e(s).
      setup_my_registers(&my_regi);
        sleep_ms(200);

//  Initialize LCD for control interface.
      pico_lcd_initialise(&my_regi);
        sleep_ms(200);

  nrf_client_t my_nrf;

  nrf_client_configured(&my_nrf, &my_pins, &my_config, 5000000);
  // result of packet transmission

  sleep_ms(2500);

while(1) {

  if(nrf_initial_bypass == 1){
    printf("NRF Client Active.\n");
    nrf_reset = false;
    nrf_initial_bypass = 0;
  sleep_ms(1000);
        printf("Beginning Operation\n");
  sleep_ms(1000);
  }else {

    nrf_client_configured(&my_nrf, &my_pins, &my_config, 5000000);
    printf("Re-Activating NRF Client.\n");
    nrf_reset = false;
    transmit_NCS = 0;
      printf("////\tNum of Reset: %i\t////\n", transmit_RAN);
      sleep_ms(1000);
          printf("Resuming Operation\n");
      sleep_ms(1000);
  }

  while(!nrf_reset){

    printf("\n");
    printf("Value of Butt->button_value: %i.\n", my_types.button_value);
    
    adc_pin_call(&my_adc);
    
      sleep_ms(200);
    pico_to_default_screen(&my_regi, &my_lcd_lines, my_adc.adc1_mapped_value, my_adc.adc2_mapped_value, my_types.button_value, 0x36);
      sleep_ms(200);

    printf("\n");
    
    buffer_payload(&my_loads, my_types.button_value, my_adc.adc1_mapped_value, my_adc.adc2_mapped_value);
    sleep_ms(10);
      nrf_rc_update(&my_nrf);
    sleep_ms(10);

    printf("\n");


  }
}
tight_loop_contents();
}
