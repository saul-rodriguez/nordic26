
#include <zephyr/kernel.h>
#include <zephyr/device.h>
#include <zephyr/logging/log.h>
#include <zephyr/devicetree.h>
#include <stdio.h>
#include <zephyr/drivers/gpio.h>


#define RESETN_NODE	        DT_ALIAS(resetn)
#define STATUS_LED_NODE 	DT_ALIAS(statusled)
#define ENHV_NODE 	        DT_ALIAS(enhv)

/**
 * @brief Configures the GPIO peripheral
 * 
 * @details Initates and validates that the GPIO device is ready
 * 
 * @return Error code
 */
int gpio_configure();

/**
 * @brief Turns on the DC-DC coverter
 * 
 * @return Error code
 */
int en_hv_on();

/**
 * @brief Turns off the DC-DC coverter
 * 
 * @return Error code
 */
int en_hv_off();

/**
 * @brief Controls the LED indicating the connection status
 * 
 * @param val 0 for LOW, 1 for HIGH
 * 
 * @return Error code
 */
int status_led_set(uint32_t val);

/**
 * @brief Sets ASIC reset pin HIGH
 * 
 * @return Error code
 */
int resetn_high();

/**
 * @brief Sets ASIC reset pin LOW 
 * 
 * @return Error code
 */
int resetn_low();


/**
 * @brief Used to reset the ASIC
 * 
 * @return Error code
 */
int toggle_resetn();


/**
 * @brief Testing funciton for GPIOS
 * 
 * @return Error code
 */
int gpio_test_run();

//int gpio_init();

