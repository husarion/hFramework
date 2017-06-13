/**
 * Copyright (c) 2013-2017 Husarion Sp. z o.o.
 * Distributed under the MIT license.
 * For full terms see the file LICENSE.md.
 */

#include "hSerial.h"

extern "C" {
#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_system.h"
#include "driver/uart.h"
#include "freertos/queue.h"
#include "soc/uart_struct.h"

uint32_t msToTicks(uint32_t ms);
};

namespace hFramework {

hSerial::hSerial(hSerial_ID no): no(no) {

}

void hSerial::init(uint32_t baudrate, Parity parity, StopBits stopBits) {
    uart_port_t uart_num = (uart_port_t)no;
    uart_config_t uart_config = {
        .baud_rate = (int)baudrate,
        .data_bits = UART_DATA_8_BITS,
        .parity = (parity == Parity::None ? UART_PARITY_DISABLE :
                   (parity == Parity::Odd ? UART_PARITY_ODD : UART_PARITY_EVEN)),
        .stop_bits = stopBits == StopBits::One ? UART_STOP_BITS_1 : UART_STOP_BITS_2,
        .flow_ctrl = UART_HW_FLOWCTRL_DISABLE,
        .rx_flow_ctrl_thresh = 122,
    };

    uart_param_config(uart_num, &uart_config);
    // (TX, RX, RTS, CTS)
    uart_set_pin(uart_num, 18, 19, 0, 0);

    uart_driver_install(uart_num, /* read buffer = */ 2048, /* write buffer = */ 2048, 0, NULL, 0); // this returns error message, but it doesn't seem critical
}

int hSerial::write(const void* data, int len, uint32_t timeout) {
    // timeout ignored
    return uart_write_bytes((uart_port_t)no, (const char*)data, (uint32_t)len);
}

int hSerial::read(void* data, int len, uint32_t timeout) {
    // TODO: timeout overflow?
    return uart_read_bytes((uart_port_t)no, (uint8_t*)data, (uint32_t)len, msToTicks(timeout));
}

hSerial Serial1 (hSerial_uart1);

}
