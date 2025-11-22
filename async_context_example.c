#include <stdio.h>
#include "pico/stdlib.h"
#include "pico/async_context_threadsafe_background.h"

// for Pico W devices the LED is on the WiFi module
#ifdef CYW43_WL_GPIO_LED_PIN
#include "pico/cyw43_arch.h"
#endif


// initialise the LED
int pico_led_init(void) {
#if defined(PICO_DEFAULT_LED_PIN)
    gpio_init(PICO_DEFAULT_LED_PIN);
    gpio_set_dir(PICO_DEFAULT_LED_PIN, GPIO_OUT);   // Pico
    return PICO_OK;
#elif defined(CYW43_WL_GPIO_LED_PIN)
    return cyw43_arch_init();   // Pico W
#endif
}


// turn the LED on or off
void pico_led_set(bool led_on) {
#if defined(PICO_DEFAULT_LED_PIN)
    gpio_put(PICO_DEFAULT_LED_PIN, led_on); // Pico
#elif defined(CYW43_WL_GPIO_LED_PIN)
    cyw43_arch_gpio_put(CYW43_WL_GPIO_LED_PIN, led_on); // Pico W
#endif    
}


// simple user data structure to hold the LED state
typedef struct {
    bool is_on;
} led_state_t;


// callback function for the async-at-time worker 
// ** this MUST be safe to call from an IRQ **
void worker_cb(async_context_t *p_ctx, 
                             async_at_time_worker_t *p_worker) {
    // read user data from worker
    led_state_t *p_led = (led_state_t *)(p_worker->user_data);

    // toggle the LED
    p_led->is_on = !p_led->is_on;
    pico_led_set(p_led->is_on);

    // re-schedule the worker to run again 500ms from now
    async_context_add_at_time_worker_in_ms(p_ctx, p_worker, 500);
}


int main()
{
    stdio_init_all();

    if (pico_led_init() != PICO_OK) {
        printf("LED init failed\n");
        return -1;
    }

    // create and initialise our user data
    led_state_t led = {
        .is_on = false
    };

    // create and initialise an asynchronous threadsafe background context
    async_context_threadsafe_background_t ctx;
    if (!async_context_threadsafe_background_init_with_defaults(&ctx)) {
        printf("couldn't initialise async context\n");
        return -2;
    }

    // create and initialise an async-at-time worker
    // (other members of the structure are initialised by the context)
    async_at_time_worker_t worker = {
        .do_work = worker_cb,
        .user_data = &led
    };

    // add the worker to the async context to run 500ms from now
    // note that ctx.core is the async_context_t of the background context
    async_context_add_at_time_worker_in_ms(&ctx.core, &worker, 500);

    while (true) {
        printf("Hello, world!\n");
        sleep_ms(5000);
    }
}
