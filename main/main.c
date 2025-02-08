#include <stdio.h>
#include <string.h>

#include "freertos/FreeRTOS.h"
#include "esp_log.h"
#include "driver/uart.h"
#include "watt_router_mx.h"

void Test()
{
    InitPort(UART_NUM_2, 17, 16);
    int32_t power;

    vTaskDelay(1000 / portTICK_PERIOD_MS);
    SetFeedingPower(300);

    while (true)
    {
        vTaskDelay(4000 / portTICK_PERIOD_MS);
        GetFeedingPower(&power);
        ESP_LOGI("UART", "Feeding power: %ld", power);
        // SetFeedingPower(300);
        // // vTaskDelay(1000 / portTICK_PERIOD_MS);

        // // SetBoilerState(WATT_ROUTER_STATE_AUTO);
        // // vTaskDelay(1000 / portTICK_PERIOD_MS);

        // // SetTuvState(WATT_ROUTER_STATE_AUTO);
        
        // vTaskDelay(1000 / portTICK_PERIOD_MS);
        // GetBoilerEnergy(&power);
        // ESP_LOGI("UART", "Boiler energy: %lld", power);

        // vTaskDelay(1000 / portTICK_PERIOD_MS);
        // GetBoilerPower(&power);
        // ESP_LOGI("UART", "Boiler power: %lld", power);

        // vTaskDelay(1000 / portTICK_PERIOD_MS);
        // GetTuvEnergy(&power);
        // ESP_LOGI("UART", "Tuv energy: %lld", power);

        // vTaskDelay(1000 / portTICK_PERIOD_MS);
        // GetTuvPower(&power);
        // ESP_LOGI("UART", "Tuv power: %lld", power);

        // vTaskDelay(6000 / portTICK_PERIOD_MS);
    }
}

void app_main(void)
{
    Test();
}