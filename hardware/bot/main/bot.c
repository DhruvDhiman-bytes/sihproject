#include <stdio.h>
#include <stdint.h>
#include "driver/gpio.h"
#include "driver/adc.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_log.h"

/* ================= MOTOR PINS ================= */

#define M1_PIN 18
#define M2_PIN 19
#define M3_PIN 22
#define M4_PIN 21

/* ================= ULTRASONIC ================= */

#define ULTRASONIC_TRIG 25
#define ULTRASONIC_ECHO 26

/* ================= MQ-5 ================= */

#define MQ5_ADC_CHANNEL ADC1_CHANNEL_6   // GPIO34

/* ================= DHT22 ================= */

#define DHT22_PIN 27

/* ================= DVP CAMERA ================= */
/* Change these according to your camera module */

#define CAM_D0     4
#define CAM_D1     5
#define CAM_D2     13
#define CAM_D3     14
#define CAM_D4     16
#define CAM_D5     17
#define CAM_D6     23
#define CAM_D7     32

#define CAM_XCLK   33
#define CAM_PCLK   34
#define CAM_HREF   35
#define CAM_VSYNC  39

#define CAM_SDA    15
#define CAM_SCL    2

#define TAG "BOT"

/* =========================================================
 * MOTOR
 * ========================================================= */

void motor_init(void)
{
    gpio_config_t config = {
        .pin_bit_mask =
            (1ULL << M1_PIN) |
            (1ULL << M2_PIN) |
            (1ULL << M3_PIN) |
            (1ULL << M4_PIN),

        .mode = GPIO_MODE_OUTPUT,
        .pull_down_en = GPIO_PULLDOWN_DISABLE,
        .pull_up_en = GPIO_PULLUP_DISABLE,
        .intr_type = GPIO_INTR_DISABLE
    };

    gpio_config(&config);

    gpio_set_level(M1_PIN, 0);
    gpio_set_level(M2_PIN, 0);
    gpio_set_level(M3_PIN, 0);
    gpio_set_level(M4_PIN, 0);
}

void motor_forward(void)
{
    gpio_set_level(M1_PIN, 1);
    gpio_set_level(M2_PIN, 0);
    gpio_set_level(M3_PIN, 1);
    gpio_set_level(M4_PIN, 0);
}

void motor_backward(void)
{
    gpio_set_level(M1_PIN, 0);
    gpio_set_level(M2_PIN, 1);
    gpio_set_level(M3_PIN, 0);
    gpio_set_level(M4_PIN, 1);
}

void motor_left(void)
{
    gpio_set_level(M1_PIN, 0);
    gpio_set_level(M2_PIN, 1);
    gpio_set_level(M3_PIN, 1);
    gpio_set_level(M4_PIN, 0);
}

void motor_right(void)
{
    gpio_set_level(M1_PIN, 1);
    gpio_set_level(M2_PIN, 0);
    gpio_set_level(M3_PIN, 0);
    gpio_set_level(M4_PIN, 1);
}

void motor_stop(void)
{
    gpio_set_level(M1_PIN, 0);
    gpio_set_level(M2_PIN, 0);
    gpio_set_level(M3_PIN, 0);
    gpio_set_level(M4_PIN, 0);
}

/* =========================================================
 * ULTRASONIC
 * ========================================================= */

void ultrasonic_init(void)
{
    gpio_config_t trig = {
        .pin_bit_mask = (1ULL << ULTRASONIC_TRIG),
        .mode = GPIO_MODE_OUTPUT,
        .pull_down_en = GPIO_PULLDOWN_DISABLE,
        .pull_up_en = GPIO_PULLUP_DISABLE,
        .intr_type = GPIO_INTR_DISABLE
    };

    gpio_config(&trig);

    gpio_config_t echo = {
        .pin_bit_mask = (1ULL << ULTRASONIC_ECHO),
        .mode = GPIO_MODE_INPUT,
        .pull_down_en = GPIO_PULLDOWN_DISABLE,
        .pull_up_en = GPIO_PULLUP_DISABLE,
        .intr_type = GPIO_INTR_DISABLE
    };

    gpio_config(&echo);

    gpio_set_level(ULTRASONIC_TRIG, 0);
}

/* =========================================================
 * MQ-5
 * ========================================================= */

void mq5_init(void)
{
    adc1_config_width(ADC_WIDTH_BIT_12);

    adc1_config_channel_atten(
        MQ5_ADC_CHANNEL,
        ADC_ATTEN_DB_11
    );
}

int mq5_read(void)
{
    return adc1_get_raw(MQ5_ADC_CHANNEL);
}

/* =========================================================
 * DHT22
 * ========================================================= */

void dht22_init(void)
{
    gpio_config_t config = {
        .pin_bit_mask = (1ULL << DHT22_PIN),
        .mode = GPIO_MODE_INPUT_OUTPUT_OD,
        .pull_down_en = GPIO_PULLDOWN_DISABLE,
        .pull_up_en = GPIO_PULLUP_ENABLE,
        .intr_type = GPIO_INTR_DISABLE
    };

    gpio_config(&config);
}

/* =========================================================
 * DVP CAMERA GPIO
 * ========================================================= */

void camera_gpio_init(void)
{
    gpio_config_t camera_data = {
        .pin_bit_mask =
            (1ULL << CAM_D0) |
            (1ULL << CAM_D1) |
            (1ULL << CAM_D2) |
            (1ULL << CAM_D3) |
            (1ULL << CAM_D4) |
            (1ULL << CAM_D5) |
            (1ULL << CAM_D6) |
            (1ULL << CAM_D7),

        .mode = GPIO_MODE_INPUT,
        .pull_down_en = GPIO_PULLDOWN_DISABLE,
        .pull_up_en = GPIO_PULLUP_DISABLE,
        .intr_type = GPIO_INTR_DISABLE
    };

    gpio_config(&camera_data);

    gpio_config_t camera_sync = {
        .pin_bit_mask =
            (1ULL << CAM_PCLK) |
            (1ULL << CAM_HREF) |
            (1ULL << CAM_VSYNC),

        .mode = GPIO_MODE_INPUT,
        .pull_down_en = GPIO_PULLDOWN_DISABLE,
        .pull_up_en = GPIO_PULLUP_DISABLE,
        .intr_type = GPIO_INTR_DISABLE
    };

    gpio_config(&camera_sync);
}

/* =========================================================
 * SENSOR TASK
 * ========================================================= */

void sensor_task(void *arg)
{
    while (1) {

        int gas_value = mq5_read();

        ESP_LOGI(
            TAG,
            "MQ5: %d",
            gas_value
        );

        /*
         * Ultrasonic:
         * trigger + echo timing goes here
         */

        /*
         * DHT22:
         * temperature + humidity reading goes here
         */

        /*
         * Camera:
         * frame capture goes here
         */

        vTaskDelay(pdMS_TO_TICKS(1000));
    }
}

/* =========================================================
 * MAIN
 * ========================================================= */

void app_main(void)
{
    motor_init();

    ultrasonic_init();

    mq5_init();

    dht22_init();

    camera_gpio_init();

    xTaskCreate(
        sensor_task,
        "sensor_task",
        4096,
        NULL,
        5,
        NULL
    );

    motor_stop();

    ESP_LOGI(TAG, "SIH BOT STARTED");
}
