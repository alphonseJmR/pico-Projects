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

#define servod 10               //  SG90 Servo motor.
#define clk 18                  //  Rotary encoder pin clk.
#define dt 12                   //  Rotary encoder pin dt.
#define rotary_button 13        //  Rotary encoder button pin.
#define notOE 14                //  When pulled high it disables the 5641AS display output.
#define serialData 15           //  This gpio pin is the serial data line, find more info in function serial_output().
#define latchClk 16             //  Pulled low to disable reg loading, pull high after 8 shifts to reg load.
#define srclk 17                //  constant clock frequency.   
#define motor_pwm 20            //  Pwm output for motor
#define motor_pwm_test 19
   
//  The variables freq, up, and down are for an int range sweep between x-min & x-max.  Can be modified to accept change in increment/decrement speed, and potentitally x-min/max.
uint16_t freq = 1342;
uint8_t payload_convert;
bool up = true;
bool down = false;
volatile bool button = false;
int motor_speed = 1300; // Adjust as needed
int duty_cycle;
uint32_t wrap;

int rot_data_value;
volatile int rotCounter;

uint slice_num[1];
uint chan[1];
int data_output[8];
int bcd[4]; // BCD representation of rotary value

int zero[8] =   {0, 0, 0, 0, 0, 0, 0, 0};   //  Not currently displaying correctly.
int one[8] =    {0, 0, 0, 1, 0, 0, 0, 0};
int two[8] =    {0, 0, 1, 0, 0, 0, 0, 0};   //  Not currently displaying correctly.
int three[8] =  {0, 0, 1, 1, 0, 0, 0, 0};   //  Not currently displaying correctly.
int four[8] =   {0, 1, 0, 0, 0, 0, 0, 0};   //  Not currently displaying correctly.   
int five[8] =   {0, 1, 0, 1, 0, 0, 0, 0};
int six[8] =    {0, 1, 1, 0, 0, 0, 0, 0};
int seven[8] =  {0, 1, 1, 1, 0, 0, 0, 0};
int eight[8] =  {1, 0, 0, 0, 0, 0, 0, 0};
int nine[8] =   {1, 0, 0, 1, 0, 0, 0, 0};

int dOne[8] =   {0, 0, 0, 0, 0, 0, 0, 1};
int dTwo[8] =   {0, 0, 0, 0, 0, 0, 1, 0};
int dThree[8] = {0, 0, 0, 0, 0, 1, 0, 0};
int dFour[8] =  {0, 0, 0, 0, 1, 0, 0, 0};

    uint8_t payload_zero;
    uint8_t payload_one[5];
    typedef struct payload_two_s { uint8_t one; uint8_t two; } payload_two_t;
    payload_two_t payload_two;



uint8_t binary_value;

typedef struct rotValues{
    volatile bool lastClk;
    volatile bool Clk;
    volatile bool lastDt;
    volatile bool Dt;
    volatile bool Button;
    volatile int debugClk;
    volatile int debugDt;
    volatile int debugLastClk;
    volatile int debugLastDt;
    uint32_t lastInterruptTime;
    volatile int a;
    volatile int b;
}rotValues;
rotValues rot;

void button_callback(uint gpio, uint32_t events) {
    uint32_t currentTime = time_us_32();
    uint32_t timeSinceLastInterrupt = currentTime - rot.lastInterruptTime;

    if(gpio == rotary_button){
     //   printf("\n\nInterrupt occured at Button.\n\n");
        button = true;  
    }

    //  The > # is the debounce threshold.  It is measured in uS.
    //  9/12 previous interrupt time set as: 150.  Current best time:  1500:
 
    if(gpio == clk) {
    if (timeSinceLastInterrupt > 1500) {
        rot.lastInterruptTime = currentTime;
        gpio_acknowledge_irq(gpio, events);

        rot.Clk = gpio_get(clk);
        rot.Dt = gpio_get(dt);
        //  Previous declaration just swapped the ! on the first if statement.
        if(rot.Clk != rot.lastClk){
    if((!rot.Clk && rot.Dt)){
    
            if(rotCounter < 255) {
                rotCounter++;
            }else {
                printf("At maximum value.\n\n");
                }
            }
        }
    }
        rot.lastClk = rot.Clk;
        rot.lastDt = rot.Dt;
    }else if(gpio == dt){
    if (timeSinceLastInterrupt > 1500) {
        rot.lastInterruptTime = currentTime;
        gpio_acknowledge_irq(gpio, events);

        rot.Clk = gpio_get(clk);
        rot.Dt = gpio_get(dt);
        //  Previous decleration of using !rot.dt && !rot.clk worked well enough?
        if(rot.Dt != rot.lastDt){
    if(!rot.Dt && rot.Clk){
        
            if(rotCounter > 47){
                rotCounter--;
            }else {
                printf("At minimum value.\n\n");
                }
            }
        }
    }
        rot.lastClk = rot.Clk;
        rot.lastDt = rot.Dt;
    }
    
}

void freq_sweep(){
    
    if(up == true){
        //  Maximum value for ping-pong effect is set at < x value.
        if(freq < 2048){
        freq++;
        up = true;
        }else{
            up = false;
        }
    }else {
        down = true;
        //  `Minimum value for the ping-pong effect is set as != x value.
        if(freq != 450){
            freq--;
        }else {
            up = true;
        }
    }

}

//  Pin setup function: rotary_pin_setups
void rotary_pin_setups(uint gpio){
    gpio_init(gpio);
    gpio_set_dir(gpio, GPIO_IN);
    gpio_pull_up(gpio);
}

//  Pin setup function: shift register pins.
void shift_reg_pins(uint gpio){
    gpio_init(gpio);
    gpio_set_dir(gpio, GPIO_OUT);
}

//  Function to create pulse for shift latching.
void latch_data() {
    gpio_put(latchClk, 1); // Generate latch pulse
    gpio_put(latchClk, 0);
}

//  Function to create pulse for clock pulses.
void clk_pulse() {
    gpio_put(srclk, 1);
    gpio_put(srclk, 0);
}

/*  Function takes in an array, and during initital loop, for each place value [0-3]
sets data_output array to values of digit array.  After inital loop completes, it 
checks the value in the [x] place, and adds that value to the data_output array.

Upgrade to use just uint8_t's?
*/
void data_out_sequence(int *bcd) {

    for(int l = 4; l >= 0; l--){

            //  Check first digit (or place of array)
        if(l == 0){
            //  Iterate through the digitmap array dOne to set data_output values for 0-3;
            for(int k = 4; k < 7; k++){
                data_output[k] = dOne[k];
            }   
            //    printf("bcd value of %i place: %i.\n", l, bcd[l]);
                if(bcd[l] == 0){
                    for(int ab = 0; ab < 4; ab++){
                        data_output[ab] = zero[ab];
                    }
                }else if(bcd[l] == 1){
                    for(int ac = 0; ac < 4; ac++){
                        data_output[ac] = one[ac];
                    }
                }else if(bcd[l] == 2){
                    for(int ad = 0; ad < 4; ad++){
                        data_output[ad] = two[ad];
                    }
                }else if(bcd[l] == 3){
                    for(int ae = 0; ae < 4; ae++){
                        data_output[ae] = three[ae];
                    }
                }else if(bcd[l] == 4){
                    for(int af = 0; af < 4; af++){
                        data_output[af] = four[af];
                    }
                }else if(bcd[l] == 5){
                    for(int ag = 0; ag < 4; ag++){
                        data_output[ag] = five[ag];
                    }
                }else if(bcd[l] == 6){
                    for(int ah = 0; ah < 4; ah++){
                        data_output[ah] = six[ah];
                    }
                }else if(bcd[l] == 7){
                    for(int ai = 0; ai < 4; ai++){
                        data_output[ai] = seven[ai];
                    }
                }else if(bcd[l] == 8){
                    for(int aj = 0; aj < 4; aj++){
                        data_output[aj] = eight[aj];
                    }
                }else if(bcd[l] == 9){
                    for(int ak = 0; ak < 4; ak++){
                        data_output[ak] = nine[ak];
                    }
                }else {
                    printf("Error Number in bcd array.\n");
                }

                gpio_put(notOE, 1);
            
            for(int aa = 0; aa < 8; aa++){
                // clk_pulse();
                gpio_put(serialData, data_output[aa]);
                clk_pulse();
            }
                latch_data();
                clk_pulse();
                gpio_put(notOE, 0);
                printf("Data Out: [%i %i %i %i %i %i %i %i].\n", data_output[0], data_output[1], data_output[2], data_output[3], data_output[4], data_output[5], data_output[6], data_output[7]);
                sleep_ms(100);
                printf("\n\n");

        }else if(l == 1){
                for(int ka = 4; ka < 7; ka++){
                data_output[ka] = dTwo[ka];
            }
            //    printf("bcd value of %i place: %i.\n", l, bcd[l]);
                if(bcd[l] == 0){
                    for(int ab = 0; ab < 4; ab++){
                        data_output[ab] = zero[ab];
                    }
                }else if(bcd[l] == 1){
                    for(int ac = 0; ac < 4; ac++){
                        data_output[ac] = one[ac];
                    }
                }else if(bcd[l] == 2){
                    for(int ad = 0; ad < 4; ad++){
                        data_output[ad] = two[ad];
                    }
                }else if(bcd[l] == 3){
                    for(int ae = 0; ae < 4; ae++){
                        data_output[ae] = three[ae];
                    }
                }else if(bcd[l] == 4){
                    for(int af = 0; af < 4; af++){
                        data_output[af] = four[af];
                    }
                }else if(bcd[l] == 5){
                    for(int ag = 0; ag < 4; ag++){
                        data_output[ag] = five[ag];
                    }
                }else if(bcd[l] == 6){
                    for(int ah = 0; ah < 4; ah++){
                        data_output[ah] = six[ah];
                    }
                }else if(bcd[l] == 7){
                    for(int ai = 0; ai < 4; ai++){
                        data_output[ai] = seven[ai];
                    }
                }else if(bcd[l] == 8){
                    for(int aj = 0; aj < 4; aj++){
                        data_output[aj] = eight[aj];
                    }
                }else if(bcd[l] == 9){
                    for(int ak = 0; ak < 4; ak++){
                        data_output[ak] = nine[ak];
                    }
                }else {
                    printf("Error Number in bcd array.\n");
                }

                gpio_put(notOE, 1);

            for(int bb = 0; bb < 8; bb++){
                // clk_pulse();
                gpio_put(serialData, (data_output[bb]));
                clk_pulse();               
            }
                latch_data();
                clk_pulse();
                gpio_put(notOE, 0);
                printf("Data Out: [%i %i %i %i %i %i %i %i].\n", data_output[0], data_output[1], data_output[2], data_output[3], data_output[4], data_output[5], data_output[6], data_output[7]);
                sleep_ms(100);
                printf("\n\n");
            
        }else if(l == 2){
                for(int kb = 4; kb < 7; kb++){
                data_output[kb] = dThree[kb];
            }
             //   printf("bcd value of %i place: %i.\n", l, bcd[l]);
                if(bcd[l] == 0){
                    for(int ab = 0; ab < 4; ab++){
                        data_output[ab] = zero[ab];
                    }
                }else if(bcd[l] == 1){
                    for(int ac = 0; ac < 4; ac++){
                        data_output[ac] = one[ac];
                    }
                }else if(bcd[l] == 2){
                    for(int ad = 0; ad < 4; ad++){
                        data_output[ad] = two[ad];
                    }
                }else if(bcd[l] == 3){
                    for(int ae = 0; ae < 4; ae++){
                        data_output[ae] = three[ae];
                    }
                }else if(bcd[l] == 4){
                    for(int af = 0; af < 4; af++){
                        data_output[af] = four[af];
                    }
                }else if(bcd[l] == 5){
                    for(int ag = 0; ag < 4; ag++){
                        data_output[ag] = five[ag];
                    }
                }else if(bcd[l] == 6){
                    for(int ah = 0; ah < 4; ah++){
                        data_output[ah] = six[ah];
                    }
                }else if(bcd[l] == 7){
                    for(int ai = 0; ai < 4; ai++){
                        data_output[ai] = seven[ai];
                    }
                }else if(bcd[l] == 8){
                    for(int aj = 0; aj < 4; aj++){
                        data_output[aj] = eight[aj];
                    }
                }else if(bcd[l] == 9){
                    for(int ak = 0; ak < 4; ak++){
                        data_output[ak] = nine[ak];
                    }
                }else {
                    printf("Error Number in bcd array.\n");
                }

                    gpio_put(notOE, 1);
            
            for(int cc = 0; cc < 8; cc++){
                // clk_pulse();
                gpio_put(serialData, data_output[cc]);
                clk_pulse();               
            }   
                latch_data();
                clk_pulse();
                gpio_put(notOE, 0);
                printf("Data Out: [%i %i %i %i %i %i %i %i].\n", data_output[0], data_output[1], data_output[2], data_output[3], data_output[4], data_output[5], data_output[6], data_output[7]);
                sleep_ms(100);
                printf("\n\n");

        }else if(l == 3){
                 for(int kd = 4; kd < 7; kd++){
                data_output[kd] = dFour[kd];
            }
            //    printf("bcd value of %i place: %i.\n", l, bcd[l]);
                if(bcd[l] == 0){
                    for(int ab = 0; ab < 4; ab++){
                        data_output[ab] = zero[ab];
                    }
                }else if(bcd[l] == 1){
                    for(int ac = 0; ac < 4; ac++){
                        data_output[ac] = one[ac];
                    }
                }else if(bcd[l] == 2){
                    for(int ad = 0; ad < 4; ad++){
                        data_output[ad] = two[ad];
                    }
                }else if(bcd[l] == 3){
                    for(int ae = 0; ae < 4; ae++){
                        data_output[ae] = three[ae];
                    }
                }else if(bcd[l] == 4){
                    for(int af = 0; af < 4; af++){
                        data_output[af] = four[af];
                    }
                }else if(bcd[l] == 5){
                    for(int ag = 0; ag < 4; ag++){
                        data_output[ag] = five[ag];
                    }
                }else if(bcd[l] == 6){
                    for(int ah = 0; ah < 4; ah++){
                        data_output[ah] = six[ah];
                    }
                }else if(bcd[l] == 7){
                    for(int ai = 0; ai < 4; ai++){
                        data_output[ai] = seven[ai];
                    }
                }else if(bcd[l] == 8){
                    for(int aj = 0; aj < 4; aj++){
                        data_output[aj] = eight[aj];
                    }
                }else if(bcd[l] == 9){
                    for(int ak = 0; ak < 4; ak++){
                        data_output[ak] = nine[ak];
                    }
                }else {
                    printf("Error Number in bcd array.\n");
                }

                gpio_put(notOE, 1);
            
            for(int dd = 0; dd < 8; dd++){
                // clk_pulse();
                gpio_put(serialData, data_output[dd]);
                clk_pulse();
            }
                latch_data();
                clk_pulse();
                gpio_put(notOE, 0);
                printf("Data Out: [%i %i %i %i %i %i %i %i].\n", data_output[0], data_output[1], data_output[2], data_output[3], data_output[4], data_output[5], data_output[6], data_output[7]);
                sleep_ms(100);
                printf("\n\n");
        }
    }
}

void decimal_to_bcd(uint16_t decimal, int *bcd) {

    bcd[0] = (decimal / 1000) % 10;
    bcd[1] = (decimal / 100) % 10;
    bcd[2] = (decimal / 10) % 10;
    bcd[3] = (decimal) % 10;
}

uint32_t pwm_set_freq_duty(uint slice_num, uint chan, uint32_t f, int d){
        uint32_t clock = 125000000;
        uint32_t divider16 = clock / f / 4096 + (clock % (f * 4096) != 0);
        if (divider16 / 16 == 0) {
            divider16 = 16;
            wrap = clock * 16 / divider16 / f - 1;
            pwm_set_clkdiv_int_frac(slice_num, divider16/16, divider16 & 0xF);
            pwm_set_wrap(slice_num, wrap);
            pwm_set_chan_level(slice_num, chan, wrap * d / 100);
        return wrap;
        }
}

void rotation_degree(){
    
    if(motor_speed >= 2600){
            printf("At maximum value.\n");
            motor_speed = 2590;
    }else if(motor_speed <= 400){
            printf("At minimum value.\n");
            motor_speed = 410;
    }else {
        printf("What.. Figure out this logic.\n");
    }
}

void pwm_pin_init(uint pin){
    gpio_init(pin);
    gpio_set_function(pin, GPIO_FUNC_PWM);
}

void payload_buffer(){
    
  // payload sent to receiver data pipe 0
    payload_convert = rotCounter;
    payload_zero = payload_convert;

  // payload sent to receiver data pipe 1
  for(int ac = 0; ac < 5; ac++){
    payload_one[ac] = ac;
  }
  typedef struct payload_two_s { uint8_t one; uint8_t two; } payload_two_t;
  // payload sent to receiver data pipe 2
  payload_two_t payload_two = { .one = motor_speed, .two = freq };
}

uint slice_num[1];
uint chan[1];

int main () {

   stdio_init_all();
    rotary_pin_setups(13);
    rotary_pin_setups(12);
    rotary_pin_setups(18);
    shift_reg_pins(14);
    shift_reg_pins(15);
    shift_reg_pins(16);
    shift_reg_pins(17);

    pwm_pin_init(19);
    slice_num[0] = pwm_gpio_to_slice_num(motor_pwm);
    chan[0] = pwm_gpio_to_channel(motor_pwm);
    pwm_set_wrap(slice_num[0], 1450);
    pwm_set_chan_level(slice_num[0], chan[0], 0);
    pwm_set_enabled(slice_num[0], true);
    
    pwm_set_chan_level(slice_num[0], chan[0], 75);

    pwm_pin_init(20);
    slice_num[1] = pwm_gpio_to_slice_num(motor_pwm_test);
    chan[1] = pwm_gpio_to_channel(motor_pwm_test);
    pwm_set_wrap(slice_num[1], 1450);
    pwm_set_chan_level(slice_num[1], chan[1], 0);
    pwm_set_enabled(slice_num[1], true);

    pwm_set_chan_level(slice_num[1], chan[1], 50);
    
    pin_manager_t my_pins = { 
    .sck = 2,
    .mosi = 3, 
    .miso = 4, 
    .csn = 5, 
    .ce = 6 
  };

  /**
   * nrf_manager_t can be passed to the nrf_client_t
   * initialise function, to specify the NRF24L01 
   * configuration. If NULL is passed to the initialise 
   * function, then the default configuration will be used.
   */

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

  nrf_client_t my_nrf;

  // initialise my_nrf
  nrf_driver_create_client(&my_nrf);

  // configure GPIO pins and SPI
  my_nrf.configure(&my_pins, my_baudrate);

  // not using default configuration (my_nrf.initialise(NULL)) 
  my_nrf.initialise(&my_config);

  // set to Standby-I Mode
  my_nrf.standby_mode();

  // result of packet transmission
  fn_status_t success = 0;

  uint64_t time_sent = 0; // time packet was sent
  uint64_t time_reply = 0; // response time after packet sent

    gpio_set_irq_enabled_with_callback(rotary_button, 0x04 | 0x08, true, &button_callback);
    gpio_set_irq_enabled_with_callback(clk, 0x08 | 0x04, true, &button_callback);
    gpio_set_irq_enabled_with_callback(dt, 0x08 | 0x04, true, &button_callback);

    rot.Clk = false;
    rot.lastClk = false;
    
  //  pwm_set_chan_level(slice_num, chan, 1000);

    rotCounter = 47;
    
   
while(1){


    rot.debugClk = gpio_get(clk);
    rot.debugDt = gpio_get(dt);

    freq_sweep();
    rotation_degree();
    motor_speed = (rotCounter * 9.8);
    
    printf("\nNumber Sweep Count: %i\n\t Max: 2048.  Min: 0\n", freq);
    printf("Rot Counter: %i.\n", rotCounter);

   decimal_to_bcd(rotCounter, bcd);
   data_out_sequence(bcd);

//  for(int abc = 0; abc < 4; abc++){
//  printf("Decimal value is: %i.\n", bcd[abc]);
// }
   
// printf("Data out was shifted.\n\n");
    if(button == true) {
        payload_buffer();
        button = false;
    }
    
    // send to receiver's DATA_PIPE_0 address
    my_nrf.tx_destination((uint8_t[]){0x37,0x37,0x37,0x37,0x37});

    // time packet was sent
    time_sent = to_us_since_boot(get_absolute_time()); // time sent

    // send packet to receiver's DATA_PIPE_0 address
    printf("\n Sending: %d.\n", payload_zero);
    success = my_nrf.send_packet(&payload_zero, sizeof(payload_zero));

    // time auto-acknowledge was received
    time_reply = to_us_since_boot(get_absolute_time()); // response time

    if (success)
    {
      printf("\nPacket sent:- Response: %lluμS | Payload: %d\n", time_reply - time_sent, payload_zero);

    } else {

      printf("\nPacket not sent:- Receiver not available.\n");
    }

    sleep_ms(300);

    // send to receiver's DATA_PIPE_1 address
    my_nrf.tx_destination((uint8_t[]){0xC7,0xC7,0xC7,0xC7,0xC7});

    // time packet was sent
    time_sent = to_us_since_boot(get_absolute_time()); // time sent

    // send packet to receiver's DATA_PIPE_1 address
    success = my_nrf.send_packet(&payload_one, sizeof(payload_one));
    
    // time auto-acknowledge was received
    time_reply = to_us_since_boot(get_absolute_time()); // response time

    if (success)
    {
      printf("\nPacket sent:- Response: %lluμS | Payload: %s\n", time_reply - time_sent, payload_one);

    } else {

      printf("\nPacket not sent:- Receiver not available.\n");
    }

    sleep_ms(300);

    // send to receiver's DATA_PIPE_2 address
    my_nrf.tx_destination((uint8_t[]){0xC8,0xC7,0xC7,0xC7,0xC7});

    // time packet was sent
    time_sent = to_us_since_boot(get_absolute_time()); // time sent

    // send packet to receiver's DATA_PIPE_2 address
    success = my_nrf.send_packet(&payload_two, sizeof(payload_two));
    
    // time auto-acknowledge was received
    time_reply = to_us_since_boot(get_absolute_time()); // response time

    if (success)
    {
      printf("\nPacket sent:- Response: %lluμS | Payload: %d & %d\n",time_reply - time_sent, payload_two.one, payload_two.two);

    } else {

      printf("\nPacket not sent:- Receiver not available.\n");
    }

    sleep_ms(300);

}
        
tight_loop_contents();
}

