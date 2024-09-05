#ifndef NRF24_USER_FUNCTION_S_H
#define NRF24_USER_FUNCTION_S_H

#include <stdio.h>
#include "./Periphereals/resources/pico_pin_enum.h"
#include "project_struct_s.h"
#include "struct_definition_s.h"
#include "./lib/nrf24l01/nrf24_driver.h"
#include "general_lcd_595_control.h"

//#define client_transmitter
#define client_receiver

uint8_t pipe_number;

uint8_t pipe_line_addr[4][5] = {
  {0x37,0x37,0x37,0x37,0x37},
  {0xC7,0xC7,0xC7,0xC7,0xC7},
  {0xC8,0xC7,0xC7,0xC7,0xC7},
  {0xC9,0xC7,0xC7,0xC7,0xC7}
};

#ifdef client_transmitter
pin_manager_t my_pins = { 

    .mosi = GPIO_ELEVEN, 
    .miso = GPIO_TWELVE, 
    .sck = GPIO_TEN,
    .csn = GPIO_THIRTEEN, 
    .ce = GPIO_NINE

};
#endif
#ifdef client_receiver
pin_manager_t my_pins = { 

    .mosi = GPIO_NINETEEN, 
    .miso = GPIO_SIXTEEN, 
    .sck = GPIO_EIGHTEEN,
    .csn = GPIO_SEVENTEEN, 
    .ce = GPIO_TWENTY

};
#endif

nrf_manager_t my_config = {

    // AW_3_BYTES, AW_4_BYTES, AW_5_BYTES
    .address_width = AW_5_BYTES,

    // dynamic payloads: DYNPD_ENABLE, DYNPD_DISABLE
    .dyn_payloads = DYNPD_ENABLE,
    
    // retransmission delay: ARD_250US, ARD_500US, ARD_750US, ARD_1000US
    .retr_delay = ARD_500US,

    // retransmission count: ARC_NONE...ARC_15RT
    .retr_count = ARC_10RT,

    // data rate: RF_DR_250KBPS, RF_DR_1MBPS, RF_DR_2MBPS
    .data_rate = RF_DR_1MBPS,

    // RF_PWR_NEG_18DBM, RF_PWR_NEG_12DBM, RF_PWR_NEG_6DBM, RF_PWR_0DBM
    .power = RF_PWR_NEG_6DBM,

    // RF Channel 
    .channel = 115

  };

nrf_verify nrf_testing = {

  .nrf_initial_bypass = true,
  .reset_nrf = false,
  .transmit_NCS = 0,
  .transmit_RAN = 0,
  .pkt_tx_time = 0,
  .pkt_rx_time = 0

};


void nrf_client_configured(nrf_client_t *client, pin_manager_t *pins, nrf_manager_t *config, uint32_t baudrate){

  printf("Configuring NRF Client.\n");
  uint8_t nrf_status;

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


void nrf_reset(nrf_client_t *client, pin_manager_t *pins, nrf_manager_t *config, uint32_t baud, nrf_verify *ver){
      printf("\n\nResetting NRF24L01 Module.\n");
      nrf_client_configured(client, pins, config, baud);
    ver->reset_nrf = false;
    ver->transmit_NCS = 0;
      printf("\n");
      sleep_ms(10);
}


void packet_print_verification(uint8_t data_one, uint8_t data_two, nrf_verify *check, uint8_t for_pipe){

      printf("\n\n");
  uint32_t packet_time;
    packet_time = (check->pkt_rx_time - check->pkt_tx_time);

      printf("Packet Sent on Pipe: %i.\n", for_pipe);
      printf("Upper Payload: 0x%04x\n", data_one);
      printf("Lower Payload: 0x%02x\n", data_two);
      printf("Packet Time Response: %lluuS\n", packet_time);

      printf("\n\n");

}


void nrf_packet_transmit(nrf_client_t *client, payload_data *data, uint8_t address, uint8_t load_num, nrf_verify *check){

  uint8_t success = 0;

  printf("\n\n////\tNRF Transmitting\t////\n");

  switch(load_num){

    case 1:
    while(!success){
      client->tx_destination(pipe_line_addr[address]);
      check->pkt_tx_time = to_us_since_boot(get_absolute_time());
      printf("Packet: Vertical Struct: 0x%04x\n", data->ready_load.load_vert_data);
      success = client->send_packet(&data->ready_load.load_vert_data, sizeof(data->ready_load.load_vert_data));
      check->pkt_rx_time = to_us_since_boot(get_absolute_time());
    if(success) {
        check->transmit_NCS = 0;
      packet_print_verification(data->ready_load.load_vert_data.vert_upper, data->ready_load.load_vert_data.vert_lower, &nrf_testing, 0);
    }else{
      printf("\nPacket not sent:- Receiver not available.\n");
    }
    }
    break;

  case 2:
    while(!success){
      client->tx_destination(pipe_line_addr[address]);
      check->pkt_tx_time = to_us_since_boot(get_absolute_time());
      printf("Packet: Horizontal Struct: 0x%04x\n", data->ready_load.load_hori_data);
      success = client->send_packet(&data->ready_load.load_hori_data, sizeof(data->ready_load.load_hori_data));
      check->pkt_rx_time = to_us_since_boot(get_absolute_time());
        if(success) {
          check->transmit_NCS = 0;
          packet_print_verification(data->ready_load.load_hori_data.hori_upper, data->ready_load.load_hori_data.hori_lower, &nrf_testing, 1);
        }else{
          printf("\nPacket not sent:- Receiver not available.\n");
        }
      }
    break;

  case 3:
    while(!success){
      client->tx_destination(pipe_line_addr[address]);
      check->pkt_tx_time = to_us_since_boot(get_absolute_time());
      printf("Packet: Raw Struct: 0x%04x\n", data->ready_load.raw_data);
      success = client->send_packet(&data->ready_load.raw_data, sizeof(data->ready_load.raw_data));
      check->pkt_rx_time = to_us_since_boot(get_absolute_time());
        if(success) {
          check->transmit_NCS = 0;

          packet_print_verification(data->ready_load.raw_data.raw_upper, data->ready_load.raw_data.raw_lower, &nrf_testing, 2);
        }else{
          printf("\nPacket not sent:- Receiver not available.\n");
        }
    } 
        break;
/*
  case 4:

      client->tx_destination(pipe_line_addr[address]);
      check->pkt_tx_time = to_us_since_boot(get_absolute_time());
      success = client->send_packet(&(data)"User Input data choice", sizeof( "user input selection"));
      check->pkt_rx_time = to_us_since_boot(get_absolute_time());
    if(success) {
      check->transmit_NCS = 0;
      packet_print_verification(""User Set Selection For Data To Show"",, &nrf_testing, 3)
    }else{
      printf("\nPacket not sent:- Receiver not available.\n");
    }
    break;
*/
  default:

    printf("Non-payload # selected.\n");
    break;

  }
  
  (success > 0) ? check->transmit_NCS = 0 : check->transmit_NCS++;
  printf("\n// //\tNo Connection Attempts: %i\t// //\n", check->transmit_NCS);

}


void nrf_rc_update(nrf_client_t* client, nrf_verify *check, payload_data *data){

  printf("Sending Data To Turret.\n");
  for(uint8_t a = 0; a < 3; a++){
    printf("\n// // //\tPayload %i\t// // //\n", a);
    nrf_packet_transmit(client, data, a, (a+1), &nrf_testing);
      sleep_ms(100);
    if(check->transmit_NCS > 59){
        check->reset_nrf = true;
        check->transmit_RAN++;
        check->transmit_NCS = 0;
    }else{
        printf("\n\tNRF Reset: %i.\n", check->transmit_RAN);
    }
  }


}


void nrf_check_received(nrf_client_t *client, payload_data *loads){

  uint8_t success;
  success = 0;

if(client->is_packet(&pipe_number)){
  printf("\n\t\tPacket Received on: %d.\n", pipe_number);

  switch (pipe_number){
    case DATA_PIPE_0:
  //  while(!success){
    printf("\t\t\t\tData Pipe: 0.\n");
      // read payload
      success = client->read_packet(&loads->ready_load.load_vert_data, sizeof(loads->ready_load.load_vert_data));
      if(success){
      printf("\nPacket received:- Payload 0x%02x | Payload 0x%02x on data pipe (%d)\n", (loads->ready_load.load_vert_data.vert_upper), (loads->ready_load.load_vert_data.vert_lower), pipe_number);
        my_nrf_con.NCS = 0;
      }else {
        printf("Transfer Failed | Previous Values %i | %i.\n",  (loads->ready_load.load_vert_data.vert_upper), (loads->ready_load.load_vert_data.vert_lower));
      printf("\n\n\t\t\tNO DATA\n\n");
      my_nrf_con.NCS++;
      }
  //    }
      break;

      case DATA_PIPE_1:
  //    while(!success){
      printf("\t\t\t\tData Pipe: 1.\n");
        // read payload
        success = client->read_packet(&loads->ready_load.load_hori_data, sizeof(loads->ready_load.load_hori_data));
        if(success){
          printf("\nPacket received:- Payload 0x%02x | Payload 0x%02x on data pipe (%d)\n", (loads->ready_load.load_hori_data.hori_upper), (loads->ready_load.load_hori_data.hori_lower), pipe_number);
          my_nrf_con.NCS = 0;
        }else {
          printf("Transfer Failed | Previous Values %i | %i.\n",  (loads->ready_load.load_hori_data.hori_upper), (loads->ready_load.load_hori_data.hori_lower));
        printf("\n\n\t\t\tNO DATA\n\n");
        my_nrf_con.NCS++;
  //      }
        }
      break;
        
      case DATA_PIPE_2:
  //    while(!success){
      printf("\t\t\t\tData Pipe: 2.\n");
        // read payload
        success = client->read_packet(&loads->ready_load.raw_data, sizeof(loads->ready_load.raw_data));
        if(success){
          printf("\nPacket received:- Payload 0x%02x | Payload 0x%02x on data pipe (%d)\n", (loads->ready_load.raw_data.raw_upper), (loads->ready_load.raw_data.raw_lower), pipe_number);
            my_nrf_con.NCS = 0;
        }else {
          printf("Transfer Failed | Previous Values %i | %i.\n",  (loads->ready_load.raw_data.raw_upper), (loads->ready_load.raw_data.raw_lower));
        printf("\n\n\t\t\tNO DATA\n\n");
        my_nrf_con.NCS++;
        }
  //   }
      break;
  
      case DATA_PIPE_3:
      break;
        
      case DATA_PIPE_4:
      break;
        
      case DATA_PIPE_5:
      break;
     
      default:
      break;
        }

}else{
  printf("\t\tNo Data on Pipe: %d.\n", pipe_number);
}
    printf("Pipe: %i.\n", pipe_number);

}

/*
void nrf_check_received(nrf_client_t *client, payload_data *loads, uint8_t *pipe){

  uint8_t success;
  success = 0;
    printf("Pipe: %i.\n", pipe);
  if(pipe){
    printf("Pipeline: #%i\n", pipe);

      // read payload
      success = client->read_packet(&loads->ready_load.load_vert_data, sizeof(loads->ready_load.load_vert_data));
      if(success){
      printf("\nPacket received:- Payload 0x%02x | Payload 0x%02x on data pipe (%d)\n", (loads->ready_load.load_vert_data.vert_upper), (loads->ready_load.load_vert_data.vert_lower), pipe);
        my_nrf_con.NCS = 0;
      }else {
        printf("Transfer Failed | Previous Values %i | %i.\n",  (loads->ready_load.load_vert_data.vert_upper), (loads->ready_load.load_vert_data.vert_lower));
      }
        my_nrf_con.NCS = 0;

      // read payload
      success = client->read_packet(&loads->ready_load.load_hori_data, sizeof(loads->ready_load.load_hori_data));
      if(success){
        printf("\nPacket received:- Payload 0x%02x | Payload 0x%02x on data pipe (%d)\n", (loads->ready_load.load_hori_data.hori_upper), (loads->ready_load.load_hori_data.hori_lower), pipe);
      }else {
        printf("Transfer Failed | Previous Values %i | %i.\n",  (loads->ready_load.load_hori_data.hori_upper), (loads->ready_load.load_hori_data.hori_lower));

      }
      my_nrf_con.NCS = 0;

      // read payload
      success = client->read_packet(&loads->ready_load.raw_data, sizeof(loads->ready_load.raw_data));
      if(success){
        printf("\nPacket received:- Payload 0x%02x | Payload 0x%02x on data pipe (%d)\n", (loads->ready_load.raw_data.raw_upper), (loads->ready_load.raw_data.raw_lower), pipe);
      }else {
        printf("Transfer Failed | Previous Values %i | %i.\n",  (loads->ready_load.raw_data.raw_upper), (loads->ready_load.raw_data.raw_lower));

      }
      my_nrf_con.NCS = 0;

} 
  else {
      my_nrf_con.NCS++;
  }
}
*/

bool initial_bypass(nrf_verify *check){
  if(check->nrf_initial_bypass){
      printf("NRF Inital Client Activation.\n");
    check->reset_nrf = false;
      printf("Beginning Operation\n");
      sleep_ms(100);
    return false;
  }else{
    printf("Nrf Initial Bypass Error?\n");
  }
}

void set_nrf_init_vals(nrf_manager_t *config, nrf_setup *clinit){

  switch(clinit->channel_set){
    case 0:
      config->channel = 110;
    break;
    case 1:
      config->channel = 111;
    break;
    case 2:
      config->channel = 112;
    break;
    case 3:
      config->channel = 113;
    break;
    case 4:
      config->channel = 114;
    break;
    case 5:
      config->channel = 115;
    break;
    case 6:
      config->channel = 116;
    break;
    case 7:
      config->channel = 117;
    break;
    case 8:
      config->channel = 118;
    break;
    case 9:
      config->channel = 119;
    break;
    case 10:
      config->channel = 120;
    break;
    default:
      printf("Invalid channel selection.\n");
        sleep_ms(2000);
    break;
  }
  switch(clinit->data_set){
    case 0:
      config->data_rate = RF_DR_250KBPS;
    break;
    case 1:
      config->data_rate = RF_DR_1MBPS;
    break;
    case 2:
      config->data_rate = RF_DR_2MBPS;
    break;
    default:
      printf("Invalid data rate selection.\n");
        sleep_ms(2000);
    break;
  }
  switch(clinit->power_set){
    case 0:
      config->power = RF_PWR_NEG_18DBM;
    break;
    case 1:
      config->power = RF_PWR_NEG_12DBM;
    break;
    case 2:
      config->power = RF_PWR_NEG_6DBM;
    break;
    case 3:
      config->power = RF_PWR_0DBM;
    break;
    default:
      printf("Invalid power selection.\n");
        sleep_ms(2000);
    break;
  }

}

bool check_completes(nrf_setup *fin){
  if(fin->channel_complete && fin->data_rate_complete && fin->power_complete){
    return true;
  }else{
    printf("All settings are not complete.\n");
    return false;
  }
}


#endif