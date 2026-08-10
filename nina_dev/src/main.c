/*
 * Copyright (c) 2016 Intel Corporation
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include <zephyr/kernel.h>
#include <zephyr/device.h>
#include <zephyr/devicetree.h>
//#include <zephyr/drivers/gpio.h>
#include <zephyr/sys/printk.h>

#include "gpios.h"
#include "pwm_clk.h"
#include "aska.h"

#define SLEEP_TIME_MS   1000

int main(void)
{
	int err = 0;
	
	printk("NINA Breakout device tests\n");
	
	//Initialize digital outputs
	err = gpio_configure();
	if(err){
		printk("Error: GPIOS failed to initialise\n");
	} 
	printk("GPIOS initialized\n");

	//Initialize PWM clock output
	err = pwm_clk_setup();
	if(!err){
		printk("Error: PWM clock failed to initialise\n");
	} else {
		printk("PWM clock initialized\n");
	}
	
	//Start pwc clock at 20 kHz
	err = pwm_clk_start();
		if(!err){
			printk	("PWM started succesfully\n");
	   } else {			
			printk("Error: PWM_CLK could not start: %d\n", err);
	}

	//Initialize SPI for ASKA
	err = ASKA_spi_setup();
                             
	// Toggle leds every 1 second and send a test command to the ASKA IC
	while (1) {
		gpio_test_run();
		//printk("toggling leds\n");
		k_msleep(SLEEP_TIME_MS);	
		ASKA_write_reg(0, ASKA_CONF1,0xaabbeecc); 	
	}
}