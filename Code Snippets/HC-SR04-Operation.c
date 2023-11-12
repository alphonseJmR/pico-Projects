Operation:
The operation of the HC-SR04 sensor involves both timed events and pulse measurement. Here's a general way to implement it:

    Triggering the Sensor: To initiate a measurement, you set the Trigger pin to a high level for a brief period (typically at least 10 microseconds). This sends an ultrasonic pulse.

    Receiving the Echo: The Echo pin will go high when it receives the ultrasonic pulse. It stays high until the reflected pulse is received. The time it takes for the Echo pin to go high and then low again is what you need to measure.

    Measuring the Distance: You can measure the time between sending the trigger pulse and receiving the echo pulse. This time is directly proportional to the distance between the sensor and the obstacle.

    Distance Calculation: Knowing the speed of sound in air (approximately 343 meters per second at 20°C), you can calculate the distance as follows: Distance = (Time / 2) * SpeedOfSound.

Here's a high-level example of how to use the HC-SR04 sensor with a Raspberry Pi Pico:

int ultra_sonic_distance(int int_distance){
        gpio_put(TRIGGER_PIN, 1);
        sleep_us(10);
        gpio_put(TRIGGER_PIN, 0);

        // Wait for the echo pulse
        while (gpio_get(ECHO_PIN) == 0) {}
        uint32_t start_time = time_us_32();

        while (gpio_get(ECHO_PIN) == 1) {}
        uint32_t end_time = time_us_32();

        // Calculate the distance
        uint32_t pulse_duration = end_time - start_time;
        double distance = (pulse_duration / 2.0) * 0.0343; // Speed of sound is 343 m/s
        int_distance = (int)distance;

        printf("Distance: %.2f cm\n", distance);
        return int_distance;
}

int main() {
    stdio_init_all();

    while (1) {
        // Send a trigger pulse
        gpio_put(TRIGGER_PIN, 1);
        sleep_us(10);
        gpio_put(TRIGGER_PIN, 0);

        // Wait for the echo pulse
        while (gpio_get(ECHO_PIN) == 0) {}
        uint32_t start_time = time_us_32();

        while (gpio_get(ECHO_PIN) == 1) {}
        uint32_t end_time = time_us_32();

        // Calculate the distance
        uint32_t pulse_duration = end_time - start_time;
        double distance = (pulse_duration / 2.0) * 0.0343; // Speed of sound is 343 m/s

        printf("Distance: %.2f cm\n", distance);

        sleep_ms(500); // Wait before taking the next measurement
    }

    return 0;
}