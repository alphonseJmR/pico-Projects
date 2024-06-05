#include <stdio.h>
#include "stdlib.h"
#include "pico/stdlib.h"
#include "hardware/gpio.h"
#include "hardware/i2c.h"
#include "_2004a_i2c.h"

ebit v;
char buff[20];

int main(){

    v = 0;

    stdio_init_all();
        sleep_ms(7000);
    printf("Seven Second Wait Finished.\n");
        sleep_ms(7000);
        
    v += initialize_lcd(&my_lcd);
printf("V initialized: %i.\n\n", v);
sleep_ms(4000);

if(v == 1){

    v = 0;

while(1) {

    printf("Loop Start.\n");

    clear_lcd();

    F_lcd_home();
    print_lcd("   I2C Driver   ");
        sleep_ms(500);

    set_lcd_cursor(1, 0);
    print_lcd("   Testing Lines   ");
        sleep_ms(500);

    set_lcd_cursor(2, 0);
    sprintf(buff, " Test Input: 0x%04x", 0xFADE); 
    print_lcd(buff);
        sleep_ms(500);

    set_lcd_cursor(3, 0);
    print_lcd("Romoulan Bizcuitz :D");

    sleep_ms(2000);
    
    printf("Loop End.\n");
}
}else{
    printf("Error Upon Initialize.\n");
}
tight_loop_contents();
}