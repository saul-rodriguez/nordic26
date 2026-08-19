#ifndef KTH_LOGGER_H
#define LOGGER_H

#include <stdio.h>
#include <zephyr/kernel.h>
#include <zephyr/device.h>
#include <zephyr/devicetree.h>
#include <zephyr/drivers/gpio.h>
#include <zephyr/drivers/spi.h>

// SPI related
#define SPIOP	SPI_WORD_SET(8) | SPI_TRANSFER_MSB


// flag to indicate if samples should be sent or not
#define KTH_LOGGER_SAMPlE_OFF 0;
#define KTH_LOGGER_SAMPlE_ON 1;

extern int KTH_LOGGER_send_samples;

int KTH_LOGGER_init(void);
int KTH_LOGGER_spi_write_buff(void *bufferData, size_t bufferSize);
void KTH_LOGGER_write_reg(uint8_t add, uint32_t value);
void KTH_LOGGER_testSPI(void);

void KTH_LOGGER_getCommand(const uint8_t *data, size_t len);
void KTH_LOGGER_enableSampling(void);
void KTH_LOGGER_disableSampling(void);



#endif // KTH_LOGGER_H