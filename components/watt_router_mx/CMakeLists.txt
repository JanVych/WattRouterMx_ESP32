idf_component_register(SRCS "rs485_modbus_rtu.c" "watt_router_mx.c"
                    INCLUDE_DIRS "include"
                    REQUIRES esp_timer
                    REQUIRES driver)
