#ifndef PWM_CLK_H
#define PWM_CLK_H

#include <zephyr/kernel.h>
#include <zephyr/logging/log.h>
#include <zephyr/device.h>
#include <zephyr/drivers/pwm.h>

#define CLK_OUT     DT_NODELABEL(clkout) 
//const struct pwm_dt_spec pwm_clk_out = PWM_DT_SPEC_GET(CLK_OUT);

#define PWM_CLK_MIN_PULSE_WIDTH    DT_PROP(CLK_OUT, min_pulse)
#define PWM_CLK_MAX_PULSE_WIDTH    DT_PROP(CLK_OUT, max_pulse)

#define PWM_CLK_PULSE_WIDTH PWM_USEC(25)
#define PWM_CLK_PERIOD   PWM_USEC(50)

int pwm_clk_setup();

int pwm_clk_start();

int pwm_clk_stop();


#endif