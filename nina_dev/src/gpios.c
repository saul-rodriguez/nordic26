#include <zephyr/kernel.h>
#include <zephyr/device.h>
#include <zephyr/logging/log.h>
#include <zephyr/devicetree.h>
#include <stdio.h>
#include <zephyr/drivers/gpio.h>
#include "gpios.h"

LOG_MODULE_REGISTER(gpios, LOG_LEVEL_DBG);

static const struct gpio_dt_spec resetn     =   GPIO_DT_SPEC_GET(RESETN_NODE, gpios);
static const struct gpio_dt_spec status_led =   GPIO_DT_SPEC_GET(STATUS_LED_NODE, gpios);
static const struct gpio_dt_spec enhv       =   GPIO_DT_SPEC_GET(ENHV_NODE, gpios);


int gpio_configure(){
	int err;

	if (!gpio_is_ready_dt(&resetn)) {
        LOG_ERR("Resetn not ready");    
        return -1;
    }

    if (!gpio_is_ready_dt(&enhv)) {
        LOG_ERR("Enable high voltage not ready");  
        return -1;
    }

    
	if (!gpio_is_ready_dt(&status_led)) {
        LOG_ERR("Status LED not ready");  
        return -1;
    }

	err = gpio_pin_configure_dt(&resetn, GPIO_OUTPUT_ACTIVE);
	if (err < 0) {
        LOG_ERR("Resetn failed to configure");    
        return err;
    }

	err = gpio_pin_configure_dt(&status_led, GPIO_OUTPUT_ACTIVE);
	if (err < 0) {
        LOG_ERR("Status LED failed to configure");            
        return err;
    }

	err = gpio_pin_configure_dt(&enhv, GPIO_OUTPUT_ACTIVE);
	if (err < 0) {
        LOG_ERR("Enable high voltage failed to configure");            
        return err;
    }
    	
	return err;
}



int gpio_test_run(){
    int ret;

    ret = gpio_pin_toggle_dt(&resetn);
	if (ret < 0) {return 0;}

    ret = gpio_pin_toggle_dt(&enhv);
    if (ret < 0) {return 0;}

    ret = gpio_pin_toggle_dt(&status_led);
    if (ret < 0) {return 0;}

    return 0;
}

int en_hv_on(){
    int err;
    err = gpio_pin_set_dt(&enhv, 0);

    if (err){
        LOG_WRN("Cannot set Enable high voltage (%d)", err);            
    }
    return err;
}

int en_hv_off(){
    int err;
    err = gpio_pin_set_dt(&enhv, 1);

    if (err){
        LOG_WRN("Cannot reset Enable high voltage (%d)", err);            
    }
    return err;
}

int status_led_set(uint32_t val)
{
	int err;

	err = gpio_pin_set_dt(&status_led, val);
	if (err) {
		LOG_ERR("Cannot write LED gpio");
	}
	return err;
}

int resetn_high(){
    int err;
    err = gpio_pin_set_dt(&resetn, 0);
    //err = gpio_pin_toggle_dt(&resetn);

    if (err){
        LOG_WRN("Cannot set Resetn low (%d)", err);            
    }
    return err;
}

int resetn_low(){
    int err;
    err = gpio_pin_set_dt(&resetn, 1);

    if (err){
        LOG_WRN("Cannot set Resetn high (%d)", err);            
    }
    return err;
}

int toggle_resetn(){
    int ret;

    ret = resetn_low();
	if (ret < 0) {return ret;}

    k_msleep(10); 
    
    ret = resetn_high();
	if (ret < 0) {return ret;}

    return ret;
}

int gpio_init(){
    int ret;
    ret = gpio_configure();
    ret = en_hv_off();
    //ret = resetn_high();
    return ret;
}



