#include <zephyr/kernel.h>
//#include <zephyr/logging/log.h>
#include <zephyr/device.h>
#include <zephyr/drivers/pwm.h>
#include "pwm_clk.h"

LOG_MODULE_REGISTER(pwm_clk, LOG_LEVEL_DBG);

static const struct pwm_dt_spec pwm_clk_out = PWM_DT_SPEC_GET(CLK_OUT);

int pwm_clk_setup(){
     int err;
    
    err = pwm_is_ready_dt(&pwm_clk_out);

    if (!err) {
        LOG_ERR("Error: PWM device %s is not ready", pwm_clk_out.dev->name);
        return 0;
    }
    
    return err;
}

/*
int pwm_clk_start()
{
    int err;
    
    err = pwm_set_dt(&pwm_clk_out, PWM_CLK_PERIOD, PWM_CLK_PULSE_WIDTH);
    if (err) {
        LOG_ERR("Error: pwm_set_dt_returned %d", err);
    }
    return err;
}
*/

int pwm_clk_stop()
{
    int err;
    
    //err = pwm_set_dt(&pwm_clk_out, PWM_CLK_PERIOD, 0);
    err = pwm_set_dt(&pwm_clk_out, 0, 0);
    if (err) {
        LOG_ERR("Error: pwm_set_dt_returned %d", err);
    }
    return err;
}

int pwm_clk_start(uint32_t freqKhz)
{
    int err;
    uint32_t period = 1000000 / freqKhz; // Calculate period in nanoseconds
    uint32_t pulse_width = period / 2; // Set pulse width to 50% duty cycle
 

    err = pwm_set_dt(&pwm_clk_out, period, pulse_width);
    if (err) {
        LOG_ERR("Error: pwm_set_dt returned %d", err);
    }
    return err;

}

