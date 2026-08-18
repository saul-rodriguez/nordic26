#include "kth_logger.h"
#include <zephyr/sys/printk.h>

#include "adc_timer_ppi.h"

int KTH_LOGGER_send_samples;

void KTH_LOGGER_init(void)
{
    KTH_LOGGER_send_samples = KTH_LOGGER_SAMPlE_OFF;
}   

void KTH_LOGGER_getCommand(const uint8_t *data, size_t len)
{
        printk("Received command: %c", data[0]);

        switch (data[0]) {
            case 's':
                KTH_LOGGER_send_samples = KTH_LOGGER_SAMPlE_ON;
                enable_pii();
                printk(" - Sending samples enabled\n");
                break;
            case 'S':
                KTH_LOGGER_send_samples = KTH_LOGGER_SAMPlE_OFF;
                disable_pii();
                printk(" - Sending samples disabled\n");
                break;
            default:
                printk(" - Invalid command\n");
                break;
        }
}