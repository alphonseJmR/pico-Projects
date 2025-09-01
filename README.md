# Pico-Projects:  _Current Focus:_ **RC Controller & RC Receiver**

(Currently in process of editing)

## Folders:
 * pico-Projects_main->
  1. Controller_Revision_Two
  2. RC_Car_Revision_Two
  3. CMakeLists.txt
   
 * Resourses--------->
  1. Code Snippets
  2. Component Datasheets
  3. Out-Sourced Lib Data
   
 * Partial Projects-->
  1. 2004A_screen
  2. 9488_screen
  3. RC_CONTROLLER_PROGRAM
  4. RC_RECIEVER_PROGRAM
    
  * Refactorable -->
   - 16x2
   - PWM_generator
   - analog_fun
   - laser_prism
   - rot_serve
   - rotary_lcd_test
   - rotary_segment_display
   - servo_test
   - spin_me_hall_round
   - Servo_And_DC_Motor.c
   - beer_rater_master
   - beer_rater_slave
   - motor_ht
   - motor_run
   - solder_hood
  
 * component_libs--->
  1. Periphereals-->
   - Motors
   - resources->
     1. Rc_RCB_Combinations.h
     2. pico_adc_input.h
     3. pico_pin_enum.h
     4. rand_funcs.h
   -  dc_motors.h
   -  interrupt_handler.h
   -  pwm_notor_controller.h
   -  shift_registers.h
   -  sn74hc595.h
   -  ultra_sonic_sensor.h
     
  2. Pico_Communication
  3. Screen_lib---->
    - ILI9488_Screen
    - _1602A_lcd
    - _2004A_lcd
    - sd1306_oled 
  4. beer_lib
  5. lib
  6. rand_lib
   - CMakeLists.txt
   - Poly_Tools.h 

### EXTRA FILES/FOLDERS:
  * build/
  * pwm_init_settings.c //  Never mind this lil guy.

## _DESCRIPTION_

  All project folders contain hopefully buildable files for programs in a C SDK for the RP2040 (PI PICO).
  !!!! This is doubtful as of the most current commit, as the dependanices of Poly_Tools will have mushed stuff up a bit. !!!!!

## Current goals:

  RF Controller - Main Running Components
  #### LCD Screen  -- In Next Hardware Design, I'll Use SSD1306 screens.
    -- Run via PIO
  #### SN74HC165 Shift Register -> As Input For -> Triple Rotary Encoder
    -- Run via PIO, DMA, and Timers.
  #### Dual analog stick input - ADC read

  #### NRF24L01 Radio Module

  Continue building program for rc_controller & rc_receiver.
  1/17/2024 - Currently tackling trying to control a 1602a v5.5 LCD screen via a CD74HC595E shift register.
      The function to perform this requries some bit manipulation due to my wiring.  Over time I'm hoping to adopt it to be more versatile.

  7/06/2024 - Currently running tests to optimize the speed between analog input and reaction time to the receiver?

  4/20/2025 - (or at some point) Successfully implemented an ili9488 driver, and wrote a game for use with it, as well as a nametag for the below happening.

  7/19/2025 - Went to OpenSauce in California.  It was fking epic.

  9/01/2025 - Currently refactoring previous attempts of controller.  Currently v4.  Update on the 1602/2004a screens via shift register, they work, but will not be implemented in the controller project.  Due to my design error, the code for the rotary encoder system on the controller is different than what it is on my proto pcb.  There may also be other issues because I'm using reclaimed parts from other projects (and I'm not great at desoldering).  I've also added a 4 pin DIP switch to use to be able to select what program to start upon power on.

  Have recently been running into trouble with the nrf24 driver (9/1/25 - will see if I can rewrite some stuff there), yet, due to lack of test equipment (or knowledge) I'm unsure if it was the code/hardware/connections.  It's currently active.

## Future goals (not in order):
 1.  Refactor entire RC car code into an appropriate project.  (9/1/25 - Have started)
 2.  Add features to make configuring the project simple.  (9/1/25 - In works with refactor above)
 4.  Optimize code base for quick and agile actions between the transmitter and receiver.  // current goal, adc->dma->nrf (9/1/25 - implementation in progress)
 5.  Add dual-core for better optimization. (9/1/25 - in progress with analog stick ADC)

  
