
#include <zephyr/kernel.h>
#include <zephyr/device.h>
#include <zephyr/logging/log.h>
#include <zephyr/devicetree.h>
#include <stdio.h>
#include <zephyr/drivers/gpio.h>


#define OUT1_NODE	        DT_ALIAS(out1)
#define OUT2_NODE 	        DT_ALIAS(out2)
#define STATUS_LED_NODE 	DT_ALIAS(statusled)
#define ENHV_NODE 	        DT_ALIAS(enhv)


int configure_KTH_logger_gpio();

int out1_on();
int out1_off();

int out2_on();  
int out2_off(); 


/*
int en_hv_on();
int en_hv_off();

int status_led_set(uint32_t val);

int resetn_high();
int resetn_low();
int toggle_resetn();

int gpio_test_run();

*/
//int gpio_init();

