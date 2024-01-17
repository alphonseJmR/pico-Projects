# Pico-Projects:  _Current Focus:_ **RC Controller & RC Receiver**

(Currently in process of editing)

## Folders:
 * Code Snippets
 * Component Datasheets
 * Partials
 * Refactorable
 * component_libs
 * **rc_controller_revision_test_1**
 * **rc_reciever_revision_1**

### EXTRA FILES/FOLDERS:
  * build/
  * pwm_init_settings.c

## _DESCRIPTION_

  All project folders contain hopefully buildable files for programs in a C SDK for the RP2040 (PI PICO).
  
  folders -analog_fun, motor_run, rot_serve, servo_test-
  These folders were / are my attempts to implement different periphericals for the above MCU.

  rc_controller & rc_receiver, rf_driver & rf_receiver attempt folders were / are my attempt at implementing the NRF24L01 driver from (include code author here)

  spin_me_hall_round was a bit of fun adding together a few of the other different implementations.


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

  
