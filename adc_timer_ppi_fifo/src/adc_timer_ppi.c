#include "adc_timer_ppi.h"
#include <zephyr/sys/printk.h> 
#include <stdio.h>

/* STEP 3.1 - Define the SAADC sample interval in microseconds */
#define SAADC_SAMPLE_INTERVAL_US 50

/* STEP 4.1 - Define the buffer size for the SAADC */
#define SAADC_BUFFER_SIZE 8000

/* STEP 4.6 - Declare the struct to hold the configuration for the SAADC channel used to sample the battery voltage */
#define SAADC_INPUT_PIN NRFX_ANALOG_EXTERNAL_AIN0
static nrfx_saadc_channel_t channel = NRFX_SAADC_DEFAULT_CHANNEL_SE(SAADC_INPUT_PIN, 0);

/* STEP 3.2 - Declaring an instance of nrfx_timer for TIMER2. */
#define TIMER_INSTANCE_NUMBER NRF_TIMER2
static nrfx_timer_t timer_instance = NRFX_TIMER_INSTANCE(TIMER_INSTANCE_NUMBER);

/* STEP 4.2 - Declare the buffers for the SAADC */
static int16_t saadc_sample_buffer[2][SAADC_BUFFER_SIZE];

/* STEP 4.3 - Declare variable used to keep track of which buffer was last assigned to the SAADC driver */
static uint32_t saadc_current_buffer = 0;

/**
 * ADC FIFO RELATED CODE
 */
 
 K_FIFO_DEFINE(ADCfifo);

 uint16_t ADC_data[MAX_DATA_SIZE];
 uint16_t ADC_data_counter = 0;

void test()
{
    //LOG_ERR("TEST");
    printk("TEST\n");
}

void configure_timer(void)
{
    int err;

    /* STEP 3.3 - Declaring timer config and intialize nrfx_timer instance. */
    nrfx_timer_config_t timer_config = NRFX_TIMER_DEFAULT_CONFIG(1000000);

    err = nrfx_timer_init(&timer_instance, &timer_config, NULL);
    if (err != 0) {
	    printk("nrfx_timer_init error: %08x\n", err);
	    return;
    }

    /* STEP 3.4 - Set compare channel 0 to generate event every SAADC_SAMPLE_INTERVAL_US. */
    uint32_t timer_ticks = nrfx_timer_us_to_ticks(&timer_instance, SAADC_SAMPLE_INTERVAL_US);
    nrfx_timer_extended_compare(&timer_instance, NRF_TIMER_CC_CHANNEL0, timer_ticks, NRF_TIMER_SHORT_COMPARE0_CLEAR_MASK, false);

}

void saadc_event_handler(nrfx_saadc_evt_t const * p_event)
{
    int err;
    switch (p_event->type)
    {
        case NRFX_SAADC_EVT_READY:
        
           /* STEP 5.1 - Buffer is ready, timer (and sampling) can be started. */
            nrfx_timer_enable(&timer_instance);

            break;                        
            
        case NRFX_SAADC_EVT_BUF_REQ:
        
            /* STEP 5.2 - Set up the next available buffer. Alternate between buffer 0 and 1 */
        err = nrfx_saadc_buffer_set(saadc_sample_buffer[(saadc_current_buffer++)%2], SAADC_BUFFER_SIZE);
        if (err != 0) {
            printk("nrfx_saadc_buffer_set error: %08x\n", err);
            return;
        }

            break;

        case NRFX_SAADC_EVT_DONE:

            /* STEP 5.3 - Buffer has been filled. Do something with the data and proceed */
            int64_t average = 0;
            int16_t max = INT16_MIN;
            int16_t min = INT16_MAX;
            int16_t current_value;
            for (int i = 0; i < p_event->data.done.size; i++) {
	            current_value = ((int16_t *)(p_event->data.done.p_buffer))[i];
	            average += current_value;
	            if (current_value > max) {
		            max = current_value;
	            }
	            if (current_value < min) {
		            min = current_value;
	            }
            }
            average = average / p_event->data.done.size;
            //printk("SAADC buffer at 0x%x filled with %d samples\n", (uint32_t)p_event->data.done.p_buffer,
	        //    p_event->data.done.size);
            //printk("AVG=%d, MIN=%d, MAX=%d\n", (int16_t)average, min, max);

            /***************************
             * SAULS CODE STARTS HERE            
             */

            sendADCdata((int16_t)average);

            /************************
             * SAULS CODE ENDS HERE
             */

            break;

        default:
            printk("Unhandled SAADC evt %d\n", p_event->type);
            break;
    }
}

void configure_saadc(void)
{
    int err;

    /* STEP 4.4 - Connect ADC interrupt to nrfx interrupt handler */
    IRQ_CONNECT(DT_IRQN(DT_NODELABEL(adc)),
            DT_IRQ(DT_NODELABEL(adc), priority),
            nrfx_isr, nrfx_saadc_irq_handler, 0);

    /* STEP 4.5 - Initialize the nrfx_SAADC driver */
    err = nrfx_saadc_init(DT_IRQ(DT_NODELABEL(adc), priority));
    if (err != 0) {
        printk("nrfx_saadc_init error: %08x\n", err);
        return;
    }


    /* STEP 4.7 - Change gain config in default config and apply channel configuration */
    channel.channel_config.gain = NRF_SAADC_GAIN1_6;
    err = nrfx_saadc_channels_config(&channel, 1);
    if (err != 0) {
        printk("nrfx_saadc_channels_config error: %08x\n", err);
        return;
    }

    /* STEP 4.8 - Configure channel 0 in advanced mode with event handler (non-blocking mode) */
    nrfx_saadc_adv_config_t saadc_adv_config = NRFX_SAADC_DEFAULT_ADV_CONFIG;
    err = nrfx_saadc_advanced_mode_set(BIT(0),
                                    NRF_SAADC_RESOLUTION_12BIT,
                                    &saadc_adv_config,
                                    saadc_event_handler);
    if (err != 0) {
        printk("nrfx_saadc_advanced_mode_set error: %08x\n", err);
        return;
    }

    /* STEP 4.9 - Configure two buffers to make use of double-buffering feature of SAADC */
    err = nrfx_saadc_buffer_set(saadc_sample_buffer[0], SAADC_BUFFER_SIZE);
    if (err != 0) {
        printk("nrfx_saadc_buffer_set error: %08x\n", err);
        return;
    }
    err = nrfx_saadc_buffer_set(saadc_sample_buffer[1], SAADC_BUFFER_SIZE);
    if (err != 0) {
        printk("nrfx_saadc_buffer_set error: %08x\n", err);
        return;
    }

    /* STEP 4.10 - Trigger the SAADC. This will not start sampling, but will prepare buffer for sampling triggered through PPI */
    err = nrfx_saadc_mode_trigger();
    if (err != 0) {
      printk("nrfx_saadc_mode_trigger error: %08x\n", err);
    return;
    }

}

void configure_ppi(void)
{
    int err;
    /* STEP 6.1 - Declare variables used to hold the (D)PPI channel number */
    nrfx_gppi_handle_t gppi_handle_sample;
    nrfx_gppi_handle_t gppi_handle_start;

    /* STEP 6.2 - Trigger task sample from timer */
    err = nrfx_gppi_conn_alloc(nrfx_timer_compare_event_address_get(&timer_instance, NRF_TIMER_CC_CHANNEL0),
                              nrf_saadc_task_address_get(NRF_SAADC, NRF_SAADC_TASK_SAMPLE), &gppi_handle_sample);
    if (err != 0) {
        printk("nrfx_gppi_conn_alloc error: %08x\n", err);
        return;
    }


    /* STEP 6.3 - Trigger task start from end event */
        err = nrfx_gppi_conn_alloc(nrf_saadc_event_address_get(NRF_SAADC, NRF_SAADC_EVENT_END),
                               nrf_saadc_task_address_get(NRF_SAADC, NRF_SAADC_TASK_START), &gppi_handle_start);
    if (err != 0) {
        printk("nrfx_gppi_conn_alloc error: %08x\n", err);
        return;
    }

    /* STEP 6.4 - Enable both (D)PPI channels */ 
    nrfx_gppi_conn_enable(gppi_handle_sample);
    nrfx_gppi_conn_enable(gppi_handle_start);

}

/**
 * FIFO RELATED CODE
 */

void sendADCdata(int16_t adc_value)
{
    struct data_item_t *item;    

    //Data is converted to uint16_t and stored in the ADC_data array. If the value is negative, it is stored as 0.
    if (adc_value > 0) {
        ADC_data[ADC_data_counter++] = (uint16_t)adc_value;
    } else {
        ADC_data[ADC_data_counter++] = 0;                  
    }

    //printk("ADC_data_counter = %u, adc_value = %d\n", ADC_data_counter, adc_value);
    
    if (ADC_data_counter >= MAX_DATA_SIZE) {
        //struct data_item_t *item;
        item = k_malloc(sizeof(struct data_item_t));
        if (!item) {
            printk("Failed to allocate memory for item\n");
            return;
        }
        item->len = ADC_data_counter;
        memcpy(item->data, ADC_data, sizeof(uint16_t) * ADC_data_counter);
        printk("Sending ADC data to FIFO: size: %u\n", item->len);
        k_fifo_put(&ADCfifo, item);
        ADC_data_counter = 0;
    }
}

 /*

 
void sendADCdata()
{
    struct data_item_t *item;    

    for (int i = 0; i < MAX_DATA_SIZE; i++) {
        ADC_data[i] = i;
        ADC_data_counter++;
        //printk("ADC_data[%d] = %u\n", i, ADC_data[i]);
    }
    
    if (ADC_data_counter >= MAX_DATA_SIZE) {
        //struct data_item_t *item;
        item = k_malloc(sizeof(struct data_item_t));
        if (!item) {
            printk("Failed to allocate memory for item\n");
            return;
        }
        item->len = ADC_data_counter;
        memcpy(item->data, ADC_data, sizeof(uint16_t) * ADC_data_counter);
        k_fifo_put(&ADCfifo, item);
        ADC_data_counter = 0;
    }
}
*/




