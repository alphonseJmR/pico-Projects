# Pico-Projects:  _Current Focus:_ **RC Controller & RC Receiver**

(Currently in process of editing)

## Folders:
 * pico-Projects_main->
  * Controller_Revision_Two
  * RC_Car_Revision_Two
  * CMakeLists.txt
   
 * Resourses--------->
  * Code Snippets
  * Component Datasheets
  * Out-Sourced Lib Data
   
 * Partial Projects-->
  * 2004A_screen
  * 9488_screen
  * RC_CONTROLLER_PROGRAM
  * RC_RECIEVER_PROGRAM
    
  * Refactorable -->
   * 16x2
   * PWM_generator
   * analog_fun
   * laser_prism
   * rot_serve
   * rotary_lcd_test
   * rotary_segment_display
   * servo_test
   * spin_me_hall_round
    
  * Servo_And_DC_Motor.c
  * beer_rater_master
  * beer_rater_slave
  * motor_ht
  * motor_run
  * solder_hood
  
 * component_libs--->
  * Periphereals-->
   * Motors
   * resources->
     * Rc_RCB_Combinations.h
     * pico_adc_input.h
     * pico_pin_enum.h
     * rand_funcs.h
   *  dc_motors.h
   *  interrupt_handler.h
   *  pwm_notor_controller.h
   *  shift_registers.h
   *  sn74hc595.h
   *  ultra_sonic_sensor.h
     
  * Pico_Communication
  * Screen_lib---->
    * ILI9488_Screen
    * _1602A_lcd
    * _2004A_lcd
    * sd1306_oled 
  * beer_lib
  * lib
  * rand_lib
   * CMakeLists.txt
   * Poly_Tools.h 

### EXTRA FILES/FOLDERS:
  * build/
  * pwm_init_settings.c //  Never mind this lil guy.

## _DESCRIPTION_

  All project folders contain hopefully buildable files for programs in a C SDK for the RP2040 (PI PICO).
  !!!! This is doubtful as of the most current commit, as the dependanices of Poly_Tools will have mushed stuff up a bit. !!!!!

## Current goals:

  Continue building program for rc_controller & rc_receiver.
  1/17/2024 - Currently tackling trying to control a 1602a v5.5 LCD screen via a CD74HC595E shift register.
      The function to perform this requries some bit manipulation due to my wiring.  Over time I'm hoping to adopt it to be more versatile.

  Have recently been running into trouble with the nrf24 driver, yet, due to lack of test equipment (or knowledge) I'm unsure if it was the code/hardware/connections.  It's currently active.

## Future goals (not in order):
 1.  Refactor entire RC car code into an appropriate project.
 2.  Add features to make configuring the project simple.
 3.  (Think about adding eeprom to hold these configurations)
 4.  Optimize code base for quick and agile actions between the transmitter and receiver.
 5.  Add dual-core / mutex to address the rotary encoder irq issues (this also requires fifo know how)

  
