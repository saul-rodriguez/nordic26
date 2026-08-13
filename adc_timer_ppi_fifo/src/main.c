/*
 * Copyright (c) 2023 Nordic Semiconductor ASA
 *
 * SPDX-License-Identifier: LicenseRef-Nordic-5-Clause
 */

#include <zephyr/kernel.h>
#include <zephyr/logging/log.h>

#include <stdio.h>


#include "adc_timer_ppi.h"

#define SLEEP_TIME_MS   1000

//The following defines the stack size and priority for the ADC consumer thread
#define ADC_STACKSIZE		 2048
#define ADC_CONSUMER_THREAD_PRIORITY 7

static void receive_adc(void)
{
	
	while (1) {
		struct data_item_t *rec_item;
		rec_item = k_fifo_get(&ADCfifo, K_FOREVER);
		//printk("Consumer: %s\tSize: %u\n", rec_item->data, rec_item->len);
        printk("Consumer: size: %u\n", rec_item->len);

        for (int i = 0; i < rec_item->len; i++) {
            printk("Data[%d]: %u\n", i, rec_item->data[i]);
        }

		k_free(rec_item);
	}
}

int main(void)
{
    //test();
    configure_timer();
    configure_saadc();  
    configure_ppi();

    //k_sleep(K_FOREVER);

    while (1) {
		
		k_msleep(SLEEP_TIME_MS);	
        //sendADCdata();

	}
}

K_THREAD_DEFINE(receive_adc_thread, ADC_STACKSIZE, receive_adc, NULL, NULL, NULL, ADC_CONSUMER_THREAD_PRIORITY, 0,0);
