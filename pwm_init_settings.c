/*
    gpio_init(srclk);
    gpio_set_function(srclk, GPIO_FUNC_PWM);
    uint slice_num = pwm_gpio_to_slice_num(srclk);
    uint chan = pwm_gpio_to_channel(srclk);
    pwm_set_wrap(slice_num, 1450);
    pwm_set_chan_level(slice_num, chan, 0);
    pwm_set_enabled(slice_num, true);
*/



   // printf("\n\nDEBUGGING\n\n");
   // printf("\tCurrent Clk: %i\tCurrent Dt: %i.\n", rot.debugClk, rot.debugDt);
   // printf("\tLast Clk: %i\tLast Dt: %i.\n", rot.debugLastClk, rot.debugLastDt);


