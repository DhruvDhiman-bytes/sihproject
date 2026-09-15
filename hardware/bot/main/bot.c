#include <stdio.h>
#include "driver/gpio.h"

// defining the pins for motors
#define M1_PIN 18
#define M2_PIN 19
#define M3_PIN 22
#define M4_PIN 21

// defining the pins for camera
#define D0_PIN 4
#define D1_PIN 9

void m1_init_func(void) { // init the motor 1
    gpio_config_t m1_motor_config = {
        .pin_bit_mask = (1ULL << M1_PIN),
        .mode = GPIO_MODE_OUTPUT,
        .pull_down_en = GPIO_PULLDOWN_DISABLE,
        .pull_up_en = GPIO_PULLUP_DISABLE,
    };

    gpio_config(&m1_motor_config);
}

void m2_init_func(void) { // init the motor 2
    gpio_config_t m2_motor_config = {
        .pin_bit_mask = (1ULL << M2_PIN),
        .mode = GPIO_MODE_OUTPUT,
        .pull_down_en = GPIO_PULLDOWN_DISABLE,
        .pull_up_en = GPIO_PULLUP_DISABLE,
    };

    gpio_config(&m2_motor_config);
}

void m3_init_func(void) {
    gpio_config_t m3_motor_config = {
        .pin_bit_mask = (1ULL << M3_PIN),
        .mode = GPIO_MODE_OUTPUT,
        .pull_down_en = GPIO_PULLDOWN_DISABLE,
        .pull_up_en = GPIO_PULLUP_DISABLE,
    };

    gpio_config(&m3_motor_config);
}

void m4_init_func(void) {
    gpio_config_t m4_motor_config = {
        .pin_bit_mask = (1ULL << M4_PIN),
        .mode = GPIO_MODE_OUTPUT,
        .pull_down_en = GPIO_PULLDOWN_DISABLE,
        .pull_up_en  = GPIO_PULLUP_DISABLE,
    };

    gpio_config(&m4_motor_config);
}

// function to move forward
void motor_forward(void) {
    gpio_set_level(M1_PIN, 1);
    gpio_set_level(M2_PIN, 0);

    gpio_set_level(M3_PIN, 1);
    gpio_set_level(M4_PIN, 0);
}

//function to move backward
void motor_backward(void) {
    gpio_set_level(M1_PIN, 0);
    gpio_set_level(M2_PIN, 1);

    gpio_set_level(M3_PIN, 0);
    gpio_set_level(M4_PIN, 1);
}

//function to stop movement
void motor_stop(void) {
    gpio_set_level(M1_PIN, 0);
    gpio_set_level(M2_PIN, 0);
    gpio_set_level(M3_PIN, 0);
    gpio_set_level(M4_PIN, 0);
}

//function to move left movement
void motor_left(void) {
    gpio_set_level(M1_PIN, 1);
    gpio_set_level(M2_PIN, 1);
    gpio_set_level(M3_PIN, 0);
    gpio_set_level(M4_PIN, 0);
}

//function to move right movement
void motor_right(void) {
    gpio_set_level(M1_PIN, 0);
    gpio_set_level(M2_PIN, 0);
    gpio_set_level(M3_PIN, 1);
    gpio_set_level(M4_PIN, 1);
}



void app_main(void)
{

    // calling the init function for motor
    m1_init_func();
    m2_init_func();
    m3_init_func();
    m4_init_func();


}
