# pico-Projects

#FOLDERS:

  analog_fun
  motor_run
  RC_CONTROLLER_PROGRAM
  RC_RECEIVER_PROGRAM
  rf_driver_attempt
  rf_driver_receiver_attempt
  rotary_segment_display
  rot_serve
  roverProject
  servo_test
  spin_me_hall_round

  EXTRA FILES/FOLDERS:
    build/
    pwm_init_settings.c

  #ANALOG_FUN:
    anal_fun.c
    interrupt.c
    CMakeLists.txt
    pico_sdk_import.cmake

  #MOTOR_RUN:
    motor.c
    CMakeLists.txt
    pico_sdk_import.cmake

  #RC_CONTROLLER_PROGRAM:
    rc_controller.c
    CMakeLists.txt
    pico_sdk_import.cmake
    lib/

  #RC_RECEIVER_PROGRAM:
    rc_receiver.c
    CMakeLists.txt
    pico_sdk_import.cmake
    lib/

  #NRF_DRIVER_ATTEMPT:
    Nrfer.c
    CMakeLists.txt
    pico_sdk_import.cmake

  #NRF_DRIVER_RECEIVER_ATTEMPT:
    Nrfer.c
    CMakeLists.txt
    pico_sdk_import.cmake

  #ROTARY_SEGMENT_DISPLAY:
    rotary_to_display.c
    CMakeLists.txt
    pico_sdk_import.cmake

  #ROT_SERVE:
    served.c
    CMakeLists.txt
    pico_sdk_import.cmake

  #ROVERPROJECT:
    roverBody.c
    CMakeLists.txt
    pico_sdk_import.cmake

  #SERVO_TEST:
    served.c
    CMakeLists.txt
    pico_sdk_import.cmake

  #SPIN_ME_HALL_ROUND:
    hall_round.c
    CMakeLists.txt
    pico_sdk_import.cmake


##DESCRIPTION##

  All folders contain hopefully buildable files for programs in a C SDK for the RP2040 (PI PICO).
  
  folders -analog_fun, motor_run, rot_serve, servo_test-
  These folders were / are my attempts to implement different periphericals for the above MCU.

  rc_controller & rc_receiver, rf_driver & rf_receiver attempt folders were / are my attempt at implementing the NRF24L01 driver from (include code author here)

  spin_me_hall_round was a bit of fun adding together a few of the other different implementations.

  roverproject is unfinished and unorgainized.


##Current goals:

  Continue building program for rc_controller & rc_receiver.
