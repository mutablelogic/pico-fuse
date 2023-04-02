
#include <stdio.h>
#include <pico/stdlib.h>
#include <hardware/uart.h>
#include <hardware/irq.h>
#include "opts.h"

// RX interrupt handler
void on_uart_rx() {
    while (uart_is_readable(UART)) {
        uint8_t ch = uart_getc(UART);
        printf("Got '%c' (0x%02X)", ch, ch);
    }
}

int main()
{
    // Initialize the board
    stdio_init_all();
    sleep_ms(250);

    // Set up our UART with a basic baud rate.
    uart_init(UART, 2400);

    // Set the TX and RX pins by using the function select on the GPIO
    // Set datasheet for more information on function select
    gpio_set_function(UART_TX_PIN, GPIO_FUNC_UART);
    gpio_set_function(UART_RX_PIN, GPIO_FUNC_UART);

    // Actually, we want a different speed
    // The call will return the actual baud rate selected, which will be as close as
    // possible to that requested
    uart_set_baudrate(UART, UART_BAUD);
    uart_set_hw_flow(UART, false, false);

    // Set our data format
    uart_set_format(UART, UART_DATA_BITS, UART_STOP_BITS, UART_PARITY);


    // And set up and enable the interrupt handlers
    irq_set_exclusive_handler(UART == uart0 ? UART0_IRQ : UART1_IRQ, on_uart_rx);
    irq_set_enabled(UART == uart0 ? UART0_IRQ : UART1_IRQ, true);

    // Now enable the UART to send interrupts - RX only
    uart_set_irq_enables(UART, true, false);

    // Wait for interrupts
    while (1)
        tight_loop_contents();
}
