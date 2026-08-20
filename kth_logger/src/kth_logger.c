#include "kth_logger.h"
#include <zephyr/sys/printk.h>

#include "adc_timer_ppi.h"
#include "gpios.h"
#include "pwm_clk.h"

// flag to indicate if samples should be sent or not
int KTH_LOGGER_send_samples;

// SPI related
struct spi_dt_spec spispec = SPI_DT_SPEC_GET(DT_NODELABEL(aska), SPIOP, 0);

int KTH_LOGGER_init(void)
{
    KTH_LOGGER_send_samples = KTH_LOGGER_SAMPlE_OFF;

    int err;

	err = spi_is_ready_dt(&spispec);
	if (!err) {
 		printk("Error: SPI device is not ready, err: %d\n", err);
 		return 0;
	} else {
        printk("SPI device is ready\n");    }

	return 1;
}   

int KTH_LOGGER_spi_write_buff(void *bufferData, size_t bufferSize)
{
    int err;

	struct spi_buf	tx_spi_buf 		= {.buf = bufferData, .len = bufferSize};
	struct spi_buf_set tx_spi_buf_set	= {.buffers = &tx_spi_buf, .count = 1};

	err = spi_write_dt(&spispec, &tx_spi_buf_set);
	if (err < 0) {
 		printk("spi_write_dt() failed, err %d", err);
 		return err;
	}

	return 0;
}

void KTH_LOGGER_write_reg(uint8_t add, uint32_t value)
{
    uint8_t tx_buffer[5];
	
    tx_buffer[0] = add;
    
    tx_buffer[1] = (uint8_t)(value >> 24) & 0xff;
    tx_buffer[2] = (uint8_t)(value >> 16) & 0xff;
    tx_buffer[3] = (uint8_t)(value >> 8) & 0xff;
    tx_buffer[4] = (uint8_t)(value >> 0) & 0xff;

    KTH_LOGGER_spi_write_buff(tx_buffer,5);

	//SPI_transaction(tx_buffer,5);
    //CS_SetLow();    
    //SPI1_WriteBlock(tx_buffer,5);
    //SPI1_BufferWrite(tx_buffer,5);    
    //CS_SetHigh();

}

void KTH_LOGGER_testSPI(void)
{
    uint8_t tx_buffer[5];

    tx_buffer[0] = 0x01; // Register address
    tx_buffer[1] = 0xaa; // Data byte 1
    tx_buffer[2] = 0xbb; // Data byte 2
    tx_buffer[3] = 0xcc; // Data byte 3
    tx_buffer[4] = 0xdd; // Data byte 4

    KTH_LOGGER_spi_write_buff(tx_buffer,5);

}

void KTH_LOGGER_getCommand(const uint8_t *data, size_t len)
{
        printk("Received command: %c", data[0]);

        switch (data[0]) {
            case 's':
                KTH_LOGGER_enableSampling();
                break;

            case 'S':
                KTH_LOGGER_disableSampling();
                break;

            case 'w':
                if (len == 6) {

                    uint8_t reg_addr = data[1];
                    uint32_t reg_value = 0;
                    
                    reg_value |= ((uint32_t)data[2] << 0);
                    reg_value |= ((uint32_t)data[3] << 8);
                    reg_value |= ((uint32_t)data[4] << 16);
                    reg_value |= ((uint32_t)data[5] << 24);     
                    KTH_LOGGER_write_reg(reg_addr, reg_value);
                    printk(" - Wrote value 0x%08X to register 0x%02X\n", reg_value, reg_addr);
                } else {
                    printk(" - Invalid write command format\n");
                }
                break;

            case 'o':
                out1_on();
                printk(" - Output 1 turned ON\n");
                break;

            case 'O':
                out1_off();
                printk(" - Output 1 turned OFF\n");
                break;

            case 'p':
                out2_on();
                printk(" - Output 1 turned ON\n");
                break;

            case 'P':
                out2_off();
                printk(" - Output 1 turned OFF\n");
                break;

            case 'c':
                if (len == 2) {
                    uint32_t freqKhz = (uint32_t)data[1];
                    pwm_clk_start(freqKhz); // Start PWM clock at specified frequency
                    printk(" - PWM clock started at %u kHz\n", freqKhz);
                } else {
                    printk(" - Invalid clock command format\n");
                }
                
                break;

            case 'C':
                pwm_clk_stop(); // Stop PWM clock
                printk(" - PWM clock stopped\n");
                break;
                
            default:
                printk(" - Invalid command\n");
                break;
        }
}

void KTH_LOGGER_enableSampling(void)
{
    KTH_LOGGER_send_samples = KTH_LOGGER_SAMPlE_ON;
    enable_pii();
    printk(" - Sending samples enabled\n");

}

void KTH_LOGGER_disableSampling(void)
{
    KTH_LOGGER_send_samples = KTH_LOGGER_SAMPlE_OFF;
    disable_pii();
    printk(" - Sending samples disabled\n");
}   

