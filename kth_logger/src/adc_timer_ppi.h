#ifndef ADC_TIMER_PPI_H
#define ADC_TIMER_PPI_H

#include <zephyr/kernel.h>
#include <zephyr/logging/log.h>

//LOG_MODULE_REGISTER(Lesson6_Exercise3, LOG_LEVEL_DBG);

/* STEP 2 - Include header for nrfx drivers */
#include <nrfx_saadc.h>
#include <nrfx_timer.h>
#include <helpers/nrfx_gppi.h>

//Number of samples to be collected for the fifo
#define MAX_DATA_SIZE 32

//#define STACKSIZE		 2048
//#define CONSUMER_THREAD_PRIORITY 7

extern struct k_fifo ADCfifo;

struct data_item_t {
	void *fifo_reserved;
	uint16_t data[MAX_DATA_SIZE];
	uint16_t len;
};

void test();

void configure_timer(void);
void saadc_event_handler(nrfx_saadc_evt_t const * p_event);
void configure_saadc(void);
void configure_ppi(void);

//FIFO related code
void sendADCdata(int16_t adc_value);

#endif // ADC_TIMER_PPI_H
