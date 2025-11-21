# Pico async context example
A simple example for the RPi Pico RP2040/2350 to demonstrate the use of an `async_context_at_time` worker with the C SDK (2.2.0).

The SDK [high level API](https://www.raspberrypi.com/documentation/pico-sdk/high_level.html#group_pico_async_context) documentation describes `pico_async_context` but appears to lack a practical explaination of how to initialise and use a worker.

The example creates an [async_context_threadsafe_background](https://www.raspberrypi.com/documentation/pico-sdk/high_level.html#group_async_context_threadsafe_background) and uses a worker to flash the default LED.
If you're using a Pico W (wifi) board you may prefer to use the `cyw43_arch` default context instead.

## board used
Note that the board used for this example is a Pico 2W, so the LED is connected to one of the WiFi module GPIOs. 

If you're using a non-wifi board you won't have to initialise the WiFi module and you can just control the GPIO directly.

## build environment
The example is configured as a VSCode project using CMakeTools and controls the target device using a SWD probe (PicoProbe).

You could equally well upload the `.uf2` executable via the USB port.