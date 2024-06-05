#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "nrf24_driver.h"
#include "stdlib.h"
#include "hardware/gpio.h"
#include "hardware/adc.h"
#include "hardware/pwm.h"
#include "hardware/spi.h"
#include "pico/time.h"

#define servo_output 12                             //  SG90 Servo motor.
#define register_display_toggle 7                  //  When pulled high it disables the 5641AS display output.
#define segment_register_data_output 8             //  This gpio pin is the serial data line on the 7 segment display.
#define rgb_register_data_output 9                 //  This gpio pin is the serial data line on the RGB register setting.
#define register_latch_toggle 10                    //  Pulled low to disable reg loading, pull high after 8 shifts to reg load.
#define register_clock_toggle 11                    //  constant clock frequency.

#define buzzer_toggle 14
#define motor_drive 15
#define motor_reverse 16
#define esm_motor 17

#define battery_adc_read 26
#define battery_v_indicator 18

//  Define pin for headlights.
#define automatic_headlights 0


int duty_cycle;
uint32_t wrap;

uint slice_num;
uint chan;

uint16_t motor_wrap_set_value;
uint16_t motor_speed_set_value;

uint16_t previous_motor_wrap_set_value;
uint16_t previous_motor_speed_set_value;

bool reverse_motor_status;

uint16_t reverse_motor_wrap;
uint16_t reverse_motor_speed;
uint16_t previous_reverse_wrap_set_value;

uint8_t payload_zero;
typedef struct payload_one_s { uint8_t vert_pwm; long hori_pwm;} payload_one_t;
payload_one_t payload_one;
uint8_t payload_two;
uint16_t payload_three;

uint8_t photo_resisitor_value;

uint16_t battery_input_voltage;
uint8_t mapped_battery_voltage;

typedef struct nrf_data_r{
    uint8_t payload_zero_buffer;
    uint8_t payload_one_data1;
    uint8_t payload_one_data2;
    uint8_t payload_two_buffer;
    uint16_t payload_three_buffer;
} nrf_data_s;
nrf_data_s nrf_data;


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
//  Adc_pin_call function CAN be used to straight shoot all three ADC inputs in a row.
//  For this purpose of just two analog inputs, only ADC0 & ADC1 will be called.
void battery_v_test() {

  adc_select_input(0);
  battery_input_voltage = adc_read();
  mapped_battery_voltage = map(battery_input_voltage, 0, 4095, 0, 255);

  ((mapped_battery_voltage >= 0xC8)&&(mapped_battery_voltage <= 0xFF)) ? printf("Battery Voltage Normal.\n") : (int)NULL;
  ((mapped_battery_voltage >= 0x64)&&(mapped_battery_voltage <= 0xC7)) ? printf("Battery Voltage Medium.\n") : (int)NULL;
  ((mapped_battery_voltage >= 0x0A)&&(mapped_battery_voltage <= 0x63)) ? printf("Battery Voltage Low.\n") : gpio_put(battery_v_indicator, 1);
};


//  Pin setup functions: rotary_pin_setups & shift_reg_pins
void shift_reg_pins(uint register_pins){
    gpio_init(register_pins);
    gpio_set_dir(register_pins, GPIO_OUT);
}
void latch_data(uint latch_pin) {
    gpio_put(latch_pin, 1); // Generate latch pulse
    gpio_put(latch_pin, 0);
}
void clk_pulse(uint clk_pin) {
    gpio_put(clk_pin, 1);
    gpio_put(clk_pin, 0);
}

//  Completed
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


  //  Call function in while loop or other function to DRIVE servo motor.
void set_motor_degrees(int servoPin, float degrees){
    pwm_set_gpio_level(servoPin, (degrees/20000.f)*39062.f);
}

  //  Initial servo motor setup.
void set_servo_motor(int servoPin, float initial_speed){
    gpio_set_function(servoPin, GPIO_FUNC_PWM);
    uint slice_num = pwm_gpio_to_slice_num(servoPin);

    pwm_config config = pwm_get_default_config();
    if(servoPin == servo_output){
    pwm_config_set_clkdiv(&config, 64.f);
    pwm_config_set_wrap(&config, 39062.f);
    pwm_init(slice_num, &config, true);
    }
    set_motor_degrees(servoPin, initial_speed);
    
    
}


//  Peripheral toggle configuration function
void peri_toggle_init(uint8_t peripheral_input_value){

  gpio_put(automatic_headlights, ((peripheral_input_value & 0x08) ? 1 : 0));
  pwm_set_chan_level(pwm_gpio_to_slice_num(esm_motor), pwm_gpio_to_channel(esm_motor), ((peripheral_input_value & 0x04) ? 7500 : 0));
  pwm_set_chan_level(pwm_gpio_to_slice_num(buzzer_toggle), pwm_gpio_to_channel(buzzer_toggle), ((peripheral_input_value & 0x02) ? 3750 : 0));

}


int main () {

   stdio_init_all();
   adc_init();

   adc_gpio_init(26);

    shift_reg_pins(7);
    shift_reg_pins(8);
    shift_reg_pins(9);
    shift_reg_pins(10);
    shift_reg_pins(11);

    initial_pwm_pin_init(14);
    initial_pwm_pin_init(15);
    initial_pwm_pin_init(16);
    initial_pwm_pin_init(17);

    gpio_init(automatic_headlights);
    gpio_set_dir(automatic_headlights, GPIO_OUT);

    gpio_init(battery_v_indicator);
    gpio_set_dir(battery_v_indicator, GPIO_OUT);

    pin_manager_t my_pins = { 
    .sck = 2,
    .mosi = 3, 
    .miso = 4, 
    .csn = 5, 
    .ce = 6 
  };

    set_servo_motor(servo_output, 1575);

nrf_manager_t my_config = {
    // RF Channel 
    .channel = 120,

    // AW_3_BYTES, AW_4_BYTES, AW_5_BYTES
    .address_width = AW_5_BYTES,

    // dynamic payloads: DYNPD_ENABLE, DYNPD_DISABLE
    .dyn_payloads = DYNPD_ENABLE,

    // data rate: RF_DR_250KBPS, RF_DR_1MBPS, RF_DR_2MBPS
    .data_rate = RF_DR_1MBPS,

    // RF_PWR_NEG_18DBM, RF_PWR_NEG_12DBM, RF_PWR_NEG_6DBM, RF_PWR_0DBM
    .power = RF_PWR_NEG_12DBM,

    // retransmission count: ARC_NONE...ARC_15RT
    .retr_count = ARC_10RT,

    // retransmission delay: ARD_250US, ARD_500US, ARD_750US, ARD_1000US
    .retr_delay = ARD_500US 
  };

  // SPI baudrate
  uint32_t my_baudrate = 5000000;

  // provides access to driver functions
  nrf_client_t my_nrf;

  // initialise my_nrf
  nrf_driver_create_client(&my_nrf);

  // configure GPIO pins and SPI
  my_nrf.configure(&my_pins, my_baudrate);

  // not using default configuration (my_nrf.initialise((int)NULL)) 
  my_nrf.initialise(&my_config);

  /**
   * set addresses for DATA_PIPE_0 - DATA_PIPE_3.
   * These are addresses the transmitter will send its packets to.
   */
  my_nrf.rx_destination(DATA_PIPE_0, (uint8_t[]){0x37,0x37,0x37,0x37,0x37});
  my_nrf.rx_destination(DATA_PIPE_1, (uint8_t[]){0xC7,0xC7,0xC7,0xC7,0xC7});
  my_nrf.rx_destination(DATA_PIPE_2, (uint8_t[]){0xC8,0xC7,0xC7,0xC7,0xC7});
  my_nrf.rx_destination(DATA_PIPE_3, (uint8_t[]){0xC9,0xC7,0xC7,0xC7,0xC7});

  // set to RX Mode
  my_nrf.receiver_mode();

  // data pipe number a packet was received on
  uint8_t pipe_number = 0;

 
while(1){

    if (my_nrf.is_packet(&pipe_number))
        {
            switch (pipe_number)
      {
        case DATA_PIPE_0:
          // read payload
          my_nrf.read_packet(&payload_zero, sizeof(payload_zero));
            nrf_data.payload_zero_buffer = payload_zero;
          // receiving a one byte uint8_t payload on DATA_PIPE_0.
          printf("\nPacket received:- Payload (0x%04x) on data pipe (%d)\n", payload_zero, pipe_number);
        break;

        case DATA_PIPE_1:
          // read payload
          my_nrf.read_packet(&payload_one, sizeof(payload_one));
          nrf_data.payload_one_data1 = payload_one.vert_pwm;
          nrf_data.payload_one_data2 = payload_one.hori_pwm;
          // receiving a eight byte struct payload on DATA_PIPE_1.
          printf("\nPacket received:- Payload (1: 0x%04x, 2: 0x%08x) on data pipe (%d)\n", payload_one.vert_pwm, payload_one.hori_pwm, pipe_number);
        break;
        
        case DATA_PIPE_2:
          // read payload
          my_nrf.read_packet(&payload_two, sizeof(payload_two));
      
                nrf_data.payload_two_buffer = payload_two;
            
          // receiving a one byte string payload on DATA_PIPE_2.
          printf("\nPacket received:- Payload (0x%04x) on data pipe (%d)\n", payload_two, pipe_number);
        break;
        
        case DATA_PIPE_3:
        // read payload
          my_nrf.read_packet(&payload_three, sizeof(payload_three));
          nrf_data.payload_three_buffer = payload_three;
          // receiving a two byte string payload on DATA_PIPE_2.
          printf("\nPacket received:- Payload (0x%04x) on data pipe (%d)\n", payload_three, pipe_number);
        break;
        
        case DATA_PIPE_4:
        break;
        
        case DATA_PIPE_5:
        break;
        
        default:
        break;
      }
        }
    peri_toggle_init(nrf_data.payload_two_buffer);
    drive_motor_wrap_config(nrf_data.payload_three_buffer, &motor_wrap_set_value);
    reverse_motor_wrap_config(nrf_data.payload_one_data1, &reverse_motor_wrap);
    motor_drive_function(nrf_data.payload_one_data1, &motor_speed_set_value, &reverse_motor_speed);
    set_motor_degrees(servo_output, nrf_data.payload_one_data2);
    pwm_set_motor_config(motor_drive, motor_speed_set_value, motor_wrap_set_value);
    pwm_set_motor_config(motor_reverse, reverse_motor_speed, reverse_motor_wrap);
    serial_register_output(7, 9, 10, 11, nrf_data.payload_zero_buffer);
    printf("\n\npayload_zero value: 0b%08x.\n\n", nrf_data.payload_zero_buffer);
    printf("payload_one vert value: 0b%08x.\n", nrf_data.payload_one_data1);
    printf("payload_one hori value: 0b%08x.\n", nrf_data.payload_one_data2);
    printf("\nPayload Two value: 0b%04x.\n", nrf_data.payload_two_buffer);
    printf("payload_three motor wrap value: 0b%08x.\n\n", payload_three);
    battery_v_test();
    printf("\n\n\tBattery Voltage: 0x%03x.\n\n", mapped_battery_voltage);
    sleep_ms(500);
}

tight_loop_contents();
}

