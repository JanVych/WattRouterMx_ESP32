#include <stdio.h>
#include <string.h>

#include "freertos/FreeRTOS.h"
#include "esp_log.h"
#include "driver/uart.h"

#define ESP_RETURN_ERROR(x) do {    \
    esp_err_t __err_rc = (x);       \
    if (__err_rc != ESP_OK) {       \
        return __err_rc;            \
    }                               \
} while(0)

uint16_t crc16(uint8_t *data, uint16_t size) 
{
    uint16_t crc = 0xFFFF;
    for (uint16_t i = 0; i < size; i++) 
    {
        crc ^= data[i]; 
        for (uint8_t j = 0; j < 8; j++) 
        { 
            if (crc & 0x0001) 
            {
                crc = (crc >> 1) ^ 0xA001;
            } 
            else 
            {
                crc = crc >> 1;
            }
        }
    }
    return crc;
}

int32_t sendFrame(uint8_t device_address, uint8_t function, uint16_t registr_address, uint16_t size)
{
    uint8_t data[8];
    data[0] = device_address;
    data[1] = function;
    data[2] = registr_address >> 8;
    data[3] = registr_address & 0xFF;
    data[4] = size >> 8;
    data[5] = size & 0xFF;
    uint16_t crc = crc16(data, 6);
    data[6] = crc & 0xFF;
    data[7] = crc >> 8;

    for (int i = 0; i < 8; i++) {
        ESP_LOGI("UART", "Data byte %d: 0x%02X", i, data[i]);
    }
    
    return uart_write_bytes(UART_NUM_2, data, 8);
}

int32_t ReadInputRegisters(uint16_t address, uint16_t* outBuffer, uint16_t size)
{
    uint32_t result = sendFrame(1, 4, address, size);
    if(result != 8)
    {
        return result;
    }
    return uart_read_bytes(UART_NUM_2, outBuffer, size, 2000 / portTICK_PERIOD_MS);
}

int32_t ReadHoldingRegisters(uint16_t address, uint16_t* outBuffer, uint16_t size)
{
    uint32_t result = sendFrame(1, 3, address, size);
    if(result != 8)
    {
        return result;
    }
    return uart_read_bytes(UART_NUM_2, outBuffer, size, 2000 / portTICK_PERIOD_MS);
}

// int32_t WriteSingleRegister(uint16_t address, uint16_t* data, uint16_t size)
// {
//     uint32_t result = sendFrame(1, 6, address, size);
// }

// int32_t WriteMultipleRegisters(uint16_t address, uint16_t* data, uint16_t size)
// {
//     uint32_t result = sendFrame(1, 16, address, size);
// }

esp_err_t init(uart_port_t uart_num, int16_t tx, int32_t rx)
{
    esp_err_t result;
    uart_config_t uart_config = 
    {
        .baud_rate = 115200,
        .data_bits = UART_DATA_8_BITS,
        .parity = UART_PARITY_DISABLE,
        .stop_bits = UART_STOP_BITS_1,
        .flow_ctrl = UART_HW_FLOWCTRL_DISABLE
    };

    result = uart_param_config(uart_num, &uart_config);
    result = uart_set_pin(uart_num, tx, rx, UART_PIN_NO_CHANGE, UART_PIN_NO_CHANGE);
    result = uart_driver_install(uart_num, 1024, 1024, 0, NULL, 0);
    return ESP_OK;
}

void app_main(void)
{
    init(UART_NUM_2, 17, 16);
    uint16_t data[2];

    while (true)
    {
        vTaskDelay(200 / portTICK_PERIOD_MS);

        ReadHoldingRegisters(20, data, 2);
        ESP_LOGI("UART", "Data: %d %d", data[0], data[1]);
    }
}