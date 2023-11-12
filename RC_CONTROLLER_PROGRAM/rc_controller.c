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

//  Define gpio_pin for rotary encoder inputs.  
#define rotary_clk      15          //  Rotary encoder pin clk.
#define rotary_dt       16          //  Rotary encoder pin dt.
#define rotary_button   17          //  Rotary encoder button pin.  Button pin held LOW(0).

//  Define gpio_pin for analog stick inputs.
#define analog_vertical     26      //  Analog input data for vertical axis.
#define analog_horizontal   27      //  Analog input data for horizontal axis.
#define analog_button       18      //  Volatile bool tied to analog button.  Button pin held LOW(0).

//  Define gpio_pins for CD74HC595E shift register.
#define data_pin            7       //  Set pin for serial data output pin.
#define display_toggle      8       //  Set pin to toggle register display enable. 
#define register_latch      9       //  Set pin to toggle pulse for register latching.
#define clock_pin           10      //  Set pin to pulse register clock.

#define step_a1 11
#define step_a2 12
#define step_b1 13
#define step_b2 14

/*
#define SPI pins
    #2
    #3
    #4
    #5
    #6
*/

int binary_converted_value[8];
int incrementer_value = 0;  

uint32_t wrap;                      //  Uint32_t  wrap value is tied to pwm_set_freq_duty function.
uint slice_num;                     //  uint slice_num is tied to pwm_generator pins.
uint chan;                          //  uint chan is tied to pwm_generator pins.

//   Bool array register_output, size 8, is used in data_buffer function.
uint8_t segment_register_buffer = 0b00000000;     
uint8_t peri_config_toggle = 0b00000000;     

//  Bool array rgb_lights_register_buffer, size 8, is used in rgb_light function.
uint8_t rgb_lights_register_buffer = 0b00000000;

//  Bool payload_zero will be used with shift register data.
uint8_t payload_zero;

//  Struct data from payload_one {vertical_buffer, horizontal_buffer} will transfer data from analog stick.
//  vertical_buffer will engage driver motors direction.
//  horizontal_buffer will engage servo motor direction.  
typedef struct payload_one_s { uint8_t vertical_buffer; uint8_t horizontal_buffer;} payload_one_t;
payload_one_t payload_one;

//  Uint8_t array payload_two, size 8, can be used to toggle various peripherials if desired, via shift register.
//  Currently set to transfer rotary encoder values.
uint8_t payload_two;

uint16_t payload_three;


//     Rotary_encoder_values struct holds volatile uint's, a single volatile bool, two uint's, and an uint32_t.
typedef struct rotary_encoder_values{
  //    Volatile settings for bool values as they'll be interrupting quickly.
    volatile bool clk_status;               //  Volatile bool to hold rotary clk_status.
    volatile bool dt_status;                //  Volatile bool to hold rotary dt_status.
    volatile uint16_t rotary_total_value;   //  Volatile uint16_t to hold value of total rotary encoder value.
    volatile bool rotary_button_status;     //  Volatile bool to hold status of rotary button.
    bool last_clk_status;                   //  Bool value to hold last status of rotary clk.
    bool last_dt_status;                    //  Bool value to hold last status of rotary dt.
    uint32_t lastInterruptTime;             //  Uint32_t value used in button_callback function to combat debounce.
  ///    Optional DEBUG volatile INT variables to plaster around where necessary.
/*
    volatile int debugclk_status;
    volatile int debugdt_status;
    volatile int debugLastclk_status;
    volatile int debugLastdt_status;
*/
}rotValues;
rotValues rotary;

//  analog_data_r contains two uint16_t variables to hold a value from ADC0 & ADC1.
//  
typedef struct analog_data_r{

    uint16_t vertical;                      //  Current reading of vertical analog stick data.  Used for driving direction of motor.
    long vertical_raw_conversion;           //  Result of long map conversion of vertical analog data.
    uint16_t horizontal;                    //  Current reading of horizontal analog stick data.  Used for driving directin of servo motor.
    long horizontal_raw_conversion;         //  Result of long map conversion on horizontal analog data.
    uint16_t photo_resistor;
    long photo_resistor_conversion;
    bool reverse_status;                    //  Bool to hold state of whether vertical input crosses threshold.
    bool engine_rumble_status;              //  Bool value to hold status of whether engine should rumble.
    int reverse_cycle;                      //  Int value to cycle through rear headlight colors.               
    volatile bool analog_button_status;     //  Volatile bool to change state of whether the analog button has been pressed.

} analog_data_r;
analog_data_r analog;

//  rgb_register_values_s contains eight bool variables to hold the places of the rgb_register.
//
typedef struct rgb_register_values_s{
    bool front_toggled;
    bool rear_toggled;
    bool frontR_toggled;
    bool frontG_toggled;
    bool frontB_toggled;
    bool rearR_toggled;
    bool rearG_toggled;
    bool rearB_toggled;
    int rgb_cycle_value;
} rgb_register_values_s;
rgb_register_values_s rgb;

//  Completed
//  Long MAP function (standard, or very common, for ADC mapping)
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
void adc_pin_call() {

    //  Select inputs according to analog stick wiring.  Perform ADC call on inputs 0 & 1.
    //  Per ADC call, store respective values in analog.[vertical && horizontal].  

    //  Select which input to read data from (0 | 1 | 2).
  adc_select_input(0);
    analog.vertical = adc_read();
  adc_select_input(1);
    analog.horizontal = adc_read();
  adc_select_input(2);
    analog.photo_resistor = adc_read();
    

    //  Convert raw analog input data of vertical & horizontal using MAP function.
    //  Store in respectively named analog.()_raw_conversion variable.
    analog.vertical_raw_conversion = map(analog.vertical, 0, 4095, 0, 255);
    analog.horizontal_raw_conversion = map(analog.horizontal, 0, 4095, 1400, 1900);
    analog.photo_resistor_conversion = map(analog.photo_resistor, 4095, 0, 0, 128);

    (analog.reverse_status) = (analog.vertical_raw_conversion >= 0 && analog.vertical_raw_conversion <= 0x64) ? true : false;
    (rgb.rear_toggled) = (analog.reverse_status) ? true : false;
    analog.engine_rumble_status = (analog.vertical_raw_conversion > 0x41 && analog.vertical_raw_conversion <= 0x64) ? true : false;
    
    (rgb.front_toggled) = (analog.photo_resistor_conversion <= 0x40) ? true : false;
//  Optional print statment to DEBUG analog conversion values.
//  printf("Analog Conversion Values:/n\tVertical: %ld\n\tHorizontal: %ld.\n", analog.vertical_raw_conversion, analog.horizontal_raw_conversion);
        
};

//  Completed
/*
    Button_callback function takes parameters: uint gpio and uint32_t events.

    It initially checks which gpio pin is calling the function.
    Two are for buttons which just toggle a bool value to true.  In the main function the values get reset via function calls.

    Two uint32_t variables are implemented to keep track of interrupt time values.
    These are used to check if the time of the current time of interrupt is greater than the threshold value of readable variables.
    
    Example:
        button_callback(rotary_clk)
            current_InterruptTime: 40000
            lastInterruptTime: current_InterruptTime

        button_callback(rotary_clk)
            current_InterruptTime: 42000
                if(interrupt time > lastInterruptTime)
                if((current_interruptTime - lastInterruptTime) >= threshold_value)
                    perform action.

     
*/
void button_callback(uint gpio, uint32_t events) {

    if(gpio == rotary_button){
    //   printf("\n\nInterrupt occured at Button.\n\n");
        rotary.rotary_button_status = true;
        printf("\n\n Rotary Button Press Occured.\n\n");
    }

    if(gpio == analog_button){
    //   printf("\n\nInterrupt occured at Button.\n\n");
        analog.analog_button_status = true;
        printf("\n\n Analog Button Press Occured.\n\n");
    }

//    Useful time functions to combat button debounce on rotary encoder.
    uint32_t currentTime = time_us_32();
    uint32_t timeSinceLastInterrupt = currentTime - rotary.lastInterruptTime;

//  The > # is the debounce threshold.  It is measured in uS.
//  9/12 previous interrupt time set as: 150.  Current best time:  1500:


  //    Check if pin is rotary_clk
  if(gpio == rotary_clk) {
    if (timeSinceLastInterrupt > 1500) {
        rotary.lastInterruptTime = currentTime;
        gpio_acknowledge_irq(gpio, events);

      rotary.clk_status = gpio_get(rotary_clk);
      rotary.dt_status = gpio_get(rotary_dt);
        
  //    Check status of current rotary clk vs last rotary clk.
    if(rotary.clk_status != rotary.last_clk_status){

  //    Check if -clk = 1 && dt = 0- || -clk = 0 && dt = 1-
    if(((rotary.clk_status && !rotary.dt_status) || (!rotary.clk_status && rotary.dt_status))){
    
      if(rotary.rotary_total_value <= 500) {

        rotary.rotary_total_value += 5;

          if(rotary.rotary_total_value > 500){
            rotary.rotary_total_value = 495;
              printf("At maximum value.\n\n");
                    }
                }
            }
        }
    }
        rotary.last_clk_status = rotary.clk_status;
        rotary.last_dt_status = rotary.dt_status;
    }
  //    Check if pin is rotary_dt.  
    if(gpio == rotary_dt){

    if (timeSinceLastInterrupt > 1500) {
        rotary.lastInterruptTime = currentTime;
        gpio_acknowledge_irq(gpio, events);

        rotary.clk_status = gpio_get(rotary_clk);
        rotary.dt_status = gpio_get(rotary_dt);

  //    Check if current dt status != last dt status.
    if(rotary.dt_status != rotary.last_dt_status){

  //    Check if -dt = 1 && clk = 0- || -dt = 0 && clk = 1-
    if((rotary.dt_status && !rotary.clk_status) || (!rotary.dt_status && rotary.clk_status)){
        
      if(rotary.rotary_total_value >= 100){

        rotary.rotary_total_value -= 5;

          if(rotary.rotary_total_value < 100){

            rotary.rotary_total_value = 110;
            printf("At minimum value.\n\n");
                    }
                }
            }
        }
    }
        rotary.last_clk_status = rotary.clk_status;
        rotary.last_dt_status = rotary.dt_status;
    }
}


//  Completed
//  Pin setup function: rotary_pin_setups
void rotary_pin_setups(uint rotary_input){
    gpio_init(rotary_input);
    gpio_set_dir(rotary_input, GPIO_IN);
    gpio_pull_up(rotary_input);
}

//  Completed
//  Pin setup function: shift register pins.
void shift_reg_pins(uint gpio){
    gpio_init(gpio);
    gpio_set_dir(gpio, GPIO_OUT);
}

//  Completed
//  Function to create pulse for shift latching.
//  Takes a uint GPIO pin to allow multiple shift register locations.
void latch_data(uint lt_pin) {
    gpio_put(lt_pin, 1); // Generate latch pulse
    gpio_put(lt_pin, 0);
}

//  Completed
//  Function to create pulse for clock pulses.
//  Takes a uint GPIO pin to allow multiple shift register locations.
void clk_pulse(uint clk_pin) {
    gpio_put(clk_pin, 1);
    gpio_put(clk_pin, 0);
}

//  Completed
/*
  Rgb_color_cycle function:
    Takes parameters of BOOL light_toggle, BOOL reverse_toggle, INT cycle_count, and INT reverse_count.

    light_toggle is to be used with analog.analog_button_status bool variable.
    cycle_count is to be used with rgb.rgb_cycle_value.

    Initial step, check if light_toggle is true, if 1 -> print previous values rgb bool values.
    Second, check if rgb cycle count variable is less than 6.
    Third, move to switch on cycle_count variable.
    Fourth, per case, change rgb.frontRGB bool values.
*/  
void rgb_color_cycle(bool light_toggle, bool reverse_toggle, int cycle_count, int reverse_count){

    if(light_toggle == true){
        if(cycle_count < 6){
            switch(cycle_count){
                case 0:
                    rgb.frontR_toggled = false;
                    rgb.frontG_toggled = true;
                    rgb.frontB_toggled = false;
                    
                break;
                case 1:
                    rgb.frontR_toggled = false;
                    rgb.frontG_toggled = false;
                    rgb.frontB_toggled = true;
                    
                break;
                case 2:
                    rgb.frontR_toggled = true;
                    rgb.frontG_toggled = false;
                    rgb.frontB_toggled = false;
                    
                break;
                case 3:
                    rgb.frontR_toggled = true;
                    rgb.frontG_toggled = true;
                    rgb.frontB_toggled = false;
                    
                break;
                case 4:
                    rgb.frontR_toggled = false;
                    rgb.frontG_toggled = true;
                    rgb.frontB_toggled = true;
                    
                break;
                case 5:
                    rgb.frontR_toggled = true;
                    rgb.frontG_toggled = true;
                    rgb.frontB_toggled = true;
                    
                break;
                default:
                    rgb.rgb_cycle_value = 0;
                    rgb.frontR_toggled = false;
                    rgb.frontG_toggled = false;
                    rgb.frontB_toggled = false;
                break;
            }
            (analog.analog_button_status) ? rgb.rgb_cycle_value++ : printf("No change to headlight color.\n");

        }else {
            printf("Cycle resetting to zero.\n");
            rgb.rgb_cycle_value = 0;
        }
    }else {

        printf("Front headlights did not toggle.\n");

    }

    if(reverse_toggle == true){
        if(reverse_count < 6){
            switch(reverse_count){
                case 0:
                    rgb.rearR_toggled = false;
                    rgb.rearG_toggled = true;
                    rgb.rearB_toggled = false;
                    
                break;
                case 1:
                    rgb.rearR_toggled = false;
                    rgb.rearG_toggled = false;
                    rgb.rearB_toggled = true;
                    
                break;
                case 2:
                    rgb.rearR_toggled = true;
                    rgb.rearG_toggled = false;
                    rgb.rearB_toggled = false;
                    
                break;
                case 3:
                    rgb.rearR_toggled = true;
                    rgb.rearG_toggled = true;
                    rgb.rearB_toggled = false;
                    
                break;
                case 4:
                    rgb.rearR_toggled = false;
                    rgb.rearG_toggled = true;
                    rgb.rearB_toggled = true;
                    
                break;
                case 5:
                    rgb.rearR_toggled = true;
                    rgb.rearG_toggled = true;
                    rgb.rearB_toggled = true;
                    
                break;
                default:
                    analog.reverse_cycle = 0;
                    rgb.rearR_toggled = false;
                    rgb.rearG_toggled = false;
                    
                break;
            }
            (analog.reverse_status) ? analog.reverse_cycle++ : printf("No change to rear light color.\n");

        }else {
            printf("Reverse cycle variable is out of intended parameters.\n Resetting to zero.\n");
            analog.reverse_cycle = 0;
        }
    }else {

        printf("Taillights did not toggle.\n");

    }
}

//  Completed
/*
    RGB_LIGHT_CONTROL_BUFFER:
    Parameters:
       uint8_t rgb_lights_register_buffer is for a bool array (currently of size 8 [8]).
       bool front_toggle is for switching front headlights on/off.
       bool rear_toggle is for switiching brake lights on/off.
*/
int rgb_light_control_buffer(uint8_t *register_buffer, bool front_toggle, bool rear_toggle){

  //  if analog button was pressed, set rgb_lights_register_buffer to equal ! it's value to flip the power on or off.
    *register_buffer += (front_toggle == true) ? 0x80 : 0;
  //  if analog reverse status is true, set rgb_lights_register_buffer 0x40.  Else set to 0.
    *register_buffer += (rear_toggle == true) ? 0x40 : 0;
  //    Toggle front & rear light color based on color cycle function.
    *register_buffer += (rgb.frontR_toggled == true) ? 0x20 : 0;
    *register_buffer += (rgb.frontG_toggled == true) ? 0x10 : 0;
    *register_buffer += (rgb.frontB_toggled == true) ? 0x08 : 0;
    *register_buffer += (rgb.rearR_toggled == true) ? 0x04 : 0;
    *register_buffer += (rgb.rearG_toggled == true) ? 0x02 : 0;
    *register_buffer += (rgb.rearB_toggled == true) ? 0x01 : 0;

    printf("\nRGB Light Register Buffer Value: 0b%08x.\n", rgb_lights_register_buffer);
    return *register_buffer;
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


//  Completed
/*
    Decimal_to_binary_converted_value function:
    parameters: 
    uint16_t    decimal
    int *binary_converted_value

    Through succession the binary_converted_value[x++] progressively divides the input
    and modulo's the remainder into the array of [x] place.
*/
void decimal_to_binary_converted_value(uint16_t decimal, int *b_c_v) {

    b_c_v[0] = (decimal / 1000) % 10;
    b_c_v[1] = (decimal / 100) % 10;
    b_c_v[2] = (decimal / 10) % 10;
    b_c_v[3] = (decimal % 10);
}


int binary_converted_buffer(int decimal_array[], uint8_t *output_uint, int incrementer){

    printf("Binary Converted Buffer Function Inititated.\n\n");

       *output_uint = 0b00000000;
       printf("Incrementer Value: %i.\n", incrementer);

    (incrementer == 0) ? (*output_uint += 0x08) : 0;
    (incrementer == 1) ? (*output_uint += 0x04) : 0;
    (incrementer == 2) ? (*output_uint += 0x02) : 0;
    (incrementer == 3) ? (*output_uint += 0x01) : 0;
    
    switch(decimal_array[incrementer]){
        case 0:
            *output_uint += 0x00;
            break;
        case 1:
            *output_uint += 0x10;
            break;
        case 2:
            *output_uint += 0x20;
            break;
        case 3:
            *output_uint += 0x30;
            break;
        case 4:
            *output_uint += 0x40;
            break;
        case 5:
            *output_uint += 0x50;
            break;
        case 6:
            *output_uint += 0x60;
            break;
        case 7:
            *output_uint += 0x70;
            break;
        case 8:
            *output_uint += 0x80;
            break;
        case 9:
            *output_uint += 0x90;
            break;
        default:
            printf("\n\nBinary Converted Buffer Error.\n\n");
            break;

    }
    (*output_uint > 0) ? printf("Value of output: 0b%04x.\n", &output_uint) : printf("Output is ZERO.\n");
        return *output_uint;
    }


/*
    Peri_pin_status_update function:
    Parameters: (long)
    photo_r_value - takes in the photoresistors adc value, and if below threshold, toggles the bit 0x08.
    vertical_analog - takes in the vertical value of the ADC and based on above or below threshold, sets bit 0x04, or 0x02 respectively.
    
    uint8_t peripheral_output is a pointer to the payload output variable.

*/
int peri_pin_status_update(long photo_r_value, long vertical_analog, uint8_t *peripheral_output){

    printf("Peripheral Pin Config Function Inititated.\n\n");
    printf("\n\tPhotoresistor Value: %d.\n\tVertical Analog: %d.\n", photo_r_value, vertical_analog);
    *peripheral_output = 0x00;

    (*peripheral_output) += (photo_r_value <= 0x32) ? 0x08 : 0x00;
    (*peripheral_output) += (vertical_analog >= 0xAF) ? 0x04 : 0x00;
    (*peripheral_output) += (vertical_analog <= 0x64) ? 0x02 : 0x00;

    printf("\n\t\tPeripheral output value: 0x%02x.\n", *peripheral_output);
    return *peripheral_output;
}


void payload_buffer(uint8_t pay_zero, long data1, long data2, uint16_t data3, uint8_t pay_two){

    printf("Payload Buffer Function Inititalized.\n");
    
  //    Payload data to be sent to receiver data pipeline 0.
    payload_zero = pay_zero;
   

  //    Payload data to be sent to receiver data pipeline 1.
    payload_one.vertical_buffer = data1;
    payload_one.horizontal_buffer = data2;
    
 
  //    Payload data to be sent to receiver data pipeline 2.

    payload_two = pay_two;

  //    Payload data to be sent to receiver data pipeline 3.
    payload_three = data3;
}


uint8_t a_on = 0x01;
uint8_t a_off = 0x02;
uint8_t b_on = 0x03;
uint8_t b_off = 0x04;

int previous_input = 0;

void move_stepper_clockwise(int input_value){

    printf("\n\t\tStepper Motor Clockwise.\n\n");

    (input_value > 500) ? (input_value = 495) : (input_value);

    if(((input_value * 4) - previous_input) > 0){

        printf("\n\n\t\tAdvancing by: %i.\n", ((input_value * 4) - previous_input));

    for(int i = 0; i < ((input_value * 4) - previous_input); i++){

      for(int j = 0; j < 4; j++){

      gpio_put(step_a1, (a_on & (0x01 << j)));
      
      (a_on & (0x01 << j))  ? printf("\n\t\t\tSTEPPER_A1_PIN HIGH.\n") : printf("\n\t\t\tSTEPPER_A1_PIN LOW.\n");
      gpio_put(step_a2, (a_off & (0x01 << j)));
      
      (a_off & (0x01 << j)) ? printf("\n\t\t\tSTEPPER_A2_PIN HIGH.\n") : printf("\n\t\t\tSTEPPER_A2_PIN LOW.\n");
      gpio_put(step_b1, (b_on & (0x01 << j)));
      
      (b_on & (0x01 << j)) ? printf("\n\t\t\tSTEPPER_B1_PIN HIGH.\n") : printf("\n\t\t\tSTEPPER_B1_PIN LOW.\n");
      gpio_put(step_b2, (b_off & (0x01 << j)));
      
      (b_off & (0x01 << j)) ? printf("\n\t\t\tSTEPPER_B2_PIN HIGH.\n") : printf("\n\t\t\tSTEPPER_B2_PIN LOW.\n");

      gpio_put(step_a1, 0);
      gpio_put(step_a2, 0);
      gpio_put(step_b1, 0);
      gpio_put(step_b2, 0);
  
      sleep_ms(50);
      }
    }
    previous_input = (input_value * 4);
    } else {
        printf("\n\t\tStepper In Position.\n");
    }
    
}

void move_stepper_counter_clockwise(int input_value){

    printf("\n\t\tStepper Motor Counter-Clockwise.\n\n");

    (input_value <= 0) ? (input_value = 1) : (input_value); 

    if((previous_input > (input_value * 4))){

        printf("\n\n\t\tRetracting by: %i.\n", (previous_input - (input_value * 4)));

    for(int i = 0; i < (previous_input - (input_value * 4)); i++){

      for(int j = 0; j < 4; j++){

      gpio_put(step_b1, (a_on & (0x08 >> j)));
     
      (b_off & (0x01 >> j)) ? printf("\n\t\t\tSTEPPER_B2_PIN HIGH.\n") : printf("\n\t\t\tSTEPPER_B2_PIN LOW.\n");
      gpio_put(step_b2, (a_off & (0x08 >> j)));
       
      (b_on & (0x01 >> j)) ? printf("\n\t\t\tSTEPPER_B1_PIN HIGH.\n") : printf("\n\t\t\tSTEPPER_B1_PIN LOW.\n");
      gpio_put(step_a1, (b_on & (0x08 >> j)));
      
      (a_off & (0x01 >> j)) ? printf("\n\t\t\tSTEPPER_A2_PIN HIGH.\n") : printf("\n\t\t\tSTEPPER_A2_PIN LOW.\n");
      gpio_put(step_a2, (b_off & (0x08 >> j)));
      
      (a_on & (0x01 >> j))  ? printf("\n\t\t\tSTEPPER_A1_PIN HIGH.\n") : printf("\n\t\t\tSTEPPER_A1_PIN LOW.\n");

      gpio_put(step_b1, 0);
      gpio_put(step_b2, 0);
      gpio_put(step_a1, 0);
      gpio_put(step_a1, 0);
      
      sleep_ms(50);
      }

    }
    previous_input = (input_value * 4);
    } else {

        printf("\n\t\tStepper In Position.\n");
    }
}

int main () {

    stdio_init_all();
    adc_init();

    shift_reg_pins(7);
    shift_reg_pins(8);
    shift_reg_pins(9);
    shift_reg_pins(10);

    rotary_pin_setups(12);
    rotary_pin_setups(13);
    gpio_init(17);
    gpio_set_dir(17, GPIO_IN);

    gpio_init(11);
    gpio_set_dir(11, GPIO_OUT);
    gpio_init(12);
    gpio_set_dir(12, GPIO_OUT);
    gpio_init(13);
    gpio_set_dir(13, GPIO_OUT);
    gpio_init(14);
    gpio_set_dir(14, GPIO_OUT);

    adc_gpio_init(28);
    adc_gpio_init(26);
    adc_gpio_init(27);

    gpio_init(18);
    gpio_set_dir(18, GPIO_IN);

    gpio_set_irq_enabled_with_callback(rotary_button, 0x04 | 0x08, true, &button_callback);
    gpio_set_irq_enabled_with_callback(analog_button, 0x04 | 0x08, true, &button_callback);
    gpio_set_irq_enabled_with_callback(rotary_clk, 0x08 | 0x04, true, &button_callback);
    gpio_set_irq_enabled_with_callback(rotary_dt, 0x08 | 0x04, true, &button_callback);

    pin_manager_t my_pins = { 
    .sck = 2,
    .mosi = 3, 
    .miso = 4, 
    .csn = 5, 
    .ce = 6 
  };

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

  rotary.clk_status = false;
  rotary.last_clk_status = false;
    
while(1){

    adc_pin_call();
    peri_pin_status_update(analog.photo_resistor_conversion, analog.vertical_raw_conversion, &peri_config_toggle);
    rgb_color_cycle(analog.analog_button_status, analog.reverse_status, rgb.rgb_cycle_value, analog.reverse_cycle);
    rgb_light_control_buffer(&rgb_lights_register_buffer, rotary.rotary_button_status, analog.reverse_status); 
    payload_buffer(rgb_lights_register_buffer, analog.vertical_raw_conversion, analog.horizontal_raw_conversion, rotary.rotary_total_value, peri_config_toggle);
  //    These three are for active components on the controller.  
    decimal_to_binary_converted_value(rotary.rotary_total_value, binary_converted_value);   //  
    binary_converted_buffer(binary_converted_value, &segment_register_buffer, incrementer_value);
    incrementer_value++;
    (incrementer_value > 3) ? (incrementer_value = 0) : printf("Incrementer value: %i.\n", incrementer_value);
    serial_register_output(display_toggle, data_pin, clock_pin, register_latch, segment_register_buffer);

    (((rotary.rotary_total_value) * 4) >= previous_input) ? move_stepper_clockwise(rotary.rotary_total_value) : move_stepper_counter_clockwise((rotary.rotary_total_value));
    
    printf("\n\nTotal rotary value: %i.\n\n", rotary.rotary_total_value);
    printf("Analog Vertical Value: %i.\n", analog.vertical_raw_conversion);
    printf("Analog Horizontal Value: %i.\n", analog.horizontal_raw_conversion);
    printf("Analog photoresistor value: %i.\n", analog.photo_resistor_conversion);
 
    for(int zv = 0; zv < 4; zv++){

        switch(zv){

        case 0:
            printf("Transmitter Switch Case 0.\n");

    // send to receiver's DATA_PIPE_0 address
    my_nrf.tx_destination((uint8_t[]){0x37,0x37,0x37,0x37,0x37});

    // time packet was sent
    time_sent = to_us_since_boot(get_absolute_time()); // time sent

    // send packet to receiver's DATA_PIPE_0 address
    printf("\n Sending: 0b%08x.\n", payload_zero);
    success = my_nrf.send_packet(&payload_zero, sizeof(payload_zero));

    // time auto-acknowledge was received
    time_reply = to_us_since_boot(get_absolute_time()); // response time

    if (success)
    {
      printf("\nPacket sent:- Response: %lluμS | Payload: 0b%08x.\n", time_reply - time_sent, payload_zero);

    } else {

      printf("\nPacket not sent:- Receiver not available.\n");
    }
        sleep_ms(300);

        break;
        case 1:
            printf("Transmitter Switch Case 1.\n");

    // send to receiver's DATA_PIPE_1 address
    my_nrf.tx_destination((uint8_t[]){0xC7,0xC7,0xC7,0xC7,0xC7});

    // time packet was sent
    time_sent = to_us_since_boot(get_absolute_time()); // time sent

    // send packet to receiver's DATA_PIPE_1 address
    printf("\n Sending: 0b%08x.\n", payload_one.vertical_buffer);
    printf("\n Sending: 0b%08x.\n", payload_one.horizontal_buffer);
    success = my_nrf.send_packet(&payload_one, sizeof(payload_one));
    
    // time auto-acknowledge was received
    time_reply = to_us_since_boot(get_absolute_time()); // response time

    if (success)
    {
        printf("\nPacket sent:- Response: %lluμS | Payload: 0b%04x & 0b%04x.\n",time_reply - time_sent, payload_one.vertical_buffer, payload_one.horizontal_buffer);
      
    } else {

      printf("\nPacket not sent:- Receiver not available.\n");
    }
        sleep_ms(300);
                
        break;
        case 2:
                printf("Transmitter Switch Case 2.\n");

    // send to receiver's DATA_PIPE_2 address
    my_nrf.tx_destination((uint8_t[]){0xC8,0xC7,0xC7,0xC7,0xC7});

    // time packet was sent
    time_sent = to_us_since_boot(get_absolute_time()); // time sent

    // send packet to receiver's DATA_PIPE_2 address
    printf("\n Sending: 0b%04x.\n", peri_config_toggle);
    success = my_nrf.send_packet(&payload_two, sizeof(payload_two));
    
    // time auto-acknowledge was received
    time_reply = to_us_since_boot(get_absolute_time()); // response time

    if (success)
    {
      printf("\nPacket sent:- Response: %lluμS | Payload: 0b%04x.\n", time_reply - time_sent, peri_config_toggle);

    }  else {

      printf("\nPacket not sent:- Receiver not available.\n");
    }
        sleep_ms(300);

        break;
        case 3:
        printf("Transmitter Switch Case 3.\n");

    // send to receiver's DATA_PIPE_3 address
    my_nrf.tx_destination((uint8_t[]){0xC9,0xC7,0xC7,0xC7,0xC7});

    // time packet was sent
    time_sent = to_us_since_boot(get_absolute_time()); // time sent

    // send packet to receiver's DATA_PIPE_3 address
    printf("\n Sending: 0b%08x.\n", payload_three);
    success = my_nrf.send_packet(&payload_three, sizeof(payload_three));

    // time auto-acknowledge was received
    time_reply = to_us_since_boot(get_absolute_time()); // response time

    if (success)
    {
      printf("\nPacket sent:- Response: %lluμS | Payload: 0b%08x.\n", time_reply - time_sent, payload_three);

    } else {

      printf("\nPacket not sent:- Receiver not available.\n");
    }
        sleep_ms(300);

        break;
        default:
            printf("Transmitter Switch Default.\n");
        break;
        }
    }
    sleep_ms(500);

}
        
tight_loop_contents();
}

