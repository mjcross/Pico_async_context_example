#include <stdio.h>
#include "pico/stdlib.h"
#include "pico/cyw43_arch.h"
#include "pico/async_context_threadsafe_background.h"

#ifndef PICO_DEFAULT_LED_PIN
#define PICO_DEFAULT_LED_PIN CYW43_WL_GPIO_LED_PIN
#endif

// simple user data structure to hold the LED state
typedef struct {
    bool is_on;
} led_state_t;


// callback function for the async-at-time worker (this MUST be safe to call from an IRQ)
void async_at_time_worker_cb(async_context_t *p_ctx, async_at_time_worker_t *p_worker) {
    // fetch user data from worker
    led_state_t *p_led = (led_state_t *)(p_worker->user_data);

    // toggle the LED
    p_led->is_on = !p_led->is_on;
    cyw43_arch_gpio_put(PICO_DEFAULT_LED_PIN, p_led->is_on);

    // re-schedule the worker to run again 500ms from now
    async_context_add_at_time_worker_in_ms(p_ctx, p_worker, 500);
}


int main()
{
    stdio_init_all();

    // Initialise the Wi-Fi chip
    if (cyw43_arch_init()) {
        printf("Wi-Fi init failed\n");
        return -1;
    }

    // initialise the LED state
    led_state_t led = {
        .is_on = false
    };

    // initialise an async-at-time worker (other members are initialised by the context)
    async_at_time_worker_t worker = {
        .do_work = async_at_time_worker_cb,
        .user_data = &led
    };

    // initialise an asynchronous threadsafe background context - see SDK high level APIs
    async_context_threadsafe_background_t ctx;
    if (!async_context_threadsafe_background_init_with_defaults(&ctx)) {
        printf("couldn't initialise async context\n");
        return -2;
    }

    // add the worker to the async context to run 500ms from now
    async_context_add_at_time_worker_in_ms(&ctx.core, &worker, 500);

    while (true) {
        printf("Hello, world!\n");
        sleep_ms(5000);
    }
}
