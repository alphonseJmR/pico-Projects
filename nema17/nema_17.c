#include <stdio.h>
#include "stdlib.h"
#include "Poly_Tools.h"

//  mode0-mode2 work togetger

typedef struct nema17_pin_s = {
  uint8_t enable;
  uint8_t mode_0;
  uint8_t mode_1;
  uint8_t mode_2;
  uint8_t reset;
  uint8_t sleep;
  uint8_t step;
  uint8_t dir;
  uint8_t fault;
}nema_pins;

int main() {

  stdio_init_all();
  sleep_ms(7000);
  sleep_ms(4000);
    printf("Main Program Setting Up Now.\n");

while(1) {


}
tight_loop_contents();
}
