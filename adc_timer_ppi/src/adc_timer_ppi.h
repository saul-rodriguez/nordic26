#ifndef ADC_TIMER_PPI_H
#define ADC_TIMER_PPI_H

#include <zephyr/kernel.h>
#include <zephyr/logging/log.h>

//LOG_MODULE_REGISTER(Lesson6_Exercise3, LOG_LEVEL_DBG);

/* STEP 2 - Include header for nrfx drivers */
#include <nrfx_saadc.h>
#include <nrfx_timer.h>
#include <helpers/nrfx_gppi.h>


void test();

void configure_timer(void);
void saadc_event_handler(nrfx_saadc_evt_t const * p_event);
void configure_saadc(void);
void configure_ppi(void);

#endif // ADC_TIMER_PPI_H
