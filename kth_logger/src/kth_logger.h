#ifndef KTH_LOGGER_H
#define LOGGER_H

#include <stdio.h>
#include <zephyr/kernel.h>

#define KTH_LOGGER_SAMPlE_OFF 0;
#define KTH_LOGGER_SAMPlE_ON 1;

extern int KTH_LOGGER_send_samples;

void KTH_LOGGER_init(void);
void KTH_LOGGER_getCommand(const uint8_t *data, size_t len);



#endif // KTH_LOGGER_H