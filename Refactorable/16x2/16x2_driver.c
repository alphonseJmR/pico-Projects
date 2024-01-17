#include <stdio.h>
#include "lcd_16x2.h"
#include "pico/stdlib.h"
#include "hardware/gpio.h"
#include "hardware/adc.h"
#include "hardware/i2c.h"
#include "ss_oled/ss_oled.h"

#define SDA_PIN 0
#define SCL_PIN 1
#define RESET_PIN -1

#define HC_trigger 12
#define HC_echo 13
#define lcd_backlight_toggle 14

#define pv_resistor_input 26

int rc;

SSOLED oled;

static uint8_t ucBuffer[1024];
#define OLED_WIDTH 128
#define OLED_HEIGHT 64

uint16_t pv_resistor_raw;
uint8_t pv_resistor_mapped;
char top_String[16];        //  Define a character array to store the formatted string.
char bottom_String[16];     //  Define a character array to store the formatted string.
char name_sake[16];
char my_name[16];
char adam_one[16];
char adam_two[16];
char u_the[16];
char lost_game[16];
uint8_t usonic_distance;        //  Define int to hold ultrasonic sensor distance.

// Define the LCD pins struct for your specific configuration
LCDPins lcd_pins = {
    .rs_pin = 16,      // Replace with the GPIO pin number for RS
    .e_pin = 17,       // Replace with the GPIO pin number for E
    .data4_pin = 8,   // Replace with the GPIO pin number for Data4
    .data5_pin = 9,   // Replace with the GPIO pin number for Data5
    .data6_pin = 10,   // Replace with the GPIO pin number for Data6
    .data7_pin = 11   // Replace with the GPIO pin number for Data7
};

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

void adc_data_call() {

              adc_select_input(0);
              pv_resistor_raw = adc_read();
              pv_resistor_mapped = map(pv_resistor_raw, 0, 4095, 0, 255);
    printf("Photoresistor Value: %i.\n", pv_resistor_mapped);
    (pv_resistor_mapped <= 175) ? gpio_put(lcd_backlight_toggle, 1) : gpio_put(lcd_backlight_toggle, 0);

/*
    //  adc_select_input(d);
    //  analog.vertical = adc_read();
    // analog.vert_pwm = map(analog.vertical, 0, 4095, 0, 4095);

    //  adc_select_input(d);
    //  analog.horizontal = adc_read();
    //  analog.hori_pwm = map(analog.horizontal, 0, 4095, 0, 4095);
    */
}

void ultra_sonic_distance(uint trigger_pin, uint echo_pin){

    printf("\n\tUltrasonic Sensor Function.\n");

        gpio_put(trigger_pin, 1);
        sleep_us(10);
        gpio_put(trigger_pin, 0);

        // Wait for the echo pulse
        while (gpio_get(echo_pin) == 0) {}
        uint32_t start_time = time_us_32();

        while (gpio_get(echo_pin) == 1) {}
        uint32_t end_time = time_us_32();

        // Calculate the distance
        uint32_t pulse_duration = end_time - start_time;
        double distance = (pulse_duration / 2.0) * 0.0343; // Speed of sound is 343 m/s
        printf("Distance: %.2f cm\n", distance);
        usonic_distance = distance;
        printf("N_Distance: 0x%02xcm\n", usonic_distance);
}

void cycle_check(int cycle){
    if(cycle == 4){
        cycle = 0;
    }
}

void oled_drive(){

      int x, y;
  oledFill(&oled, 0, 1);
  oledWriteString(&oled, 0,0,0,(char *)"Backbuffer Test", FONT_NORMAL,0,1);
  oledWriteString(&oled, 0,0,1,(char *)"96 lines", FONT_NORMAL,0,1);
  sleep_ms(2000);
  for (x=0; x<OLED_WIDTH-1; x+=2)
  {
    oledDrawLine(&oled, x, 0, OLED_WIDTH-x, OLED_HEIGHT-1, 1);
  }
  for (y=0; y<OLED_HEIGHT-1; y+=2)
  {
    oledDrawLine(&oled, OLED_WIDTH-1,y, 0,OLED_HEIGHT-1-y, 1);
  }
  oledWriteString(&oled, 0,0,1,(char *)"Without backbuffer", FONT_SMALL,0,1);
  sleep_ms(2000);
  oledFill(&oled, 0,1);
  for (x=0; x<OLED_WIDTH-1; x+=2)
  {
    oledDrawLine(&oled, x, 0, OLED_WIDTH-1-x, OLED_HEIGHT-1, 0);
  }
  for (y=0; y<OLED_HEIGHT-1; y+=2)
  {
    oledDrawLine(&oled, OLED_WIDTH-1,y, 0,OLED_HEIGHT-1-y, 0);
  }
  oledDumpBuffer(&oled, ucBuffer);
  oledWriteString(&oled, 0,0,1,(char *)"With backbuffer", FONT_SMALL,0,1);
  sleep_ms(2000);

  }


int main() {

    stdio_init_all();
    adc_init();

    int display_cycle_count = 0;

    sleep_ms(7000);
    // Initialize the LCD using the configured pins
    lcd_init(&lcd_pins);
    lcd_4_bit_init(&lcd_pins);

    adc_gpio_init(pv_resistor_input);

    gpio_init(HC_trigger);
    gpio_set_dir(HC_trigger, GPIO_OUT);

    gpio_init(HC_echo);
    gpio_set_dir(HC_echo, GPIO_IN);

    gpio_init(lcd_backlight_toggle);
    gpio_set_dir(lcd_backlight_toggle, GPIO_OUT);

    // Your LCD control code here...
    gpio_put(lcd_pins.rs_pin, 0);
        sleep_ms(5);
    lcd_clear(&lcd_pins);
        sleep_ms(20);
    lcd_home(&lcd_pins);
        sleep_ms(20);

  sleep_ms(3000);
    
while(1) {

    cycle_check(display_cycle_count);
    write_display(&lcd_pins, name_sake, my_name, 0, 0, display_cycle_count);
    oled_drive();
    display_cycle_count++;
    sleep_ms(3000);

    cycle_check(display_cycle_count);
    write_display(&lcd_pins, adam_one, adam_two, 0, 0, display_cycle_count);
    display_cycle_count++;
    sleep_ms(1000);

    cycle_check(display_cycle_count);
    write_display(&lcd_pins, u_the, lost_game, 0, 0, display_cycle_count);
    display_cycle_count++;
    sleep_ms(1000);

    adc_data_call();
    ultra_sonic_distance(HC_trigger, HC_echo);
    printf("\n\t\tDistance Reading: %dcm.\n\n", usonic_distance);
    sleep_ms(20);
    cycle_check(display_cycle_count);
    write_display(&lcd_pins, top_String, bottom_String, usonic_distance, pv_resistor_mapped, display_cycle_count);
    display_cycle_count++;
    sleep_ms(1000);
    printf("\n\n\tMy Name.\n\n");
    
}
}
