#include "aska.h"

LOG_MODULE_REGISTER(aska, LOG_LEVEL_DBG);


struct spi_dt_spec spispec = SPI_DT_SPEC_GET(DT_NODELABEL(aska), SPIOP, 0);
//struct nvs_fs fs;

const uint8_t ASKA_IC_add[4] = {
    IC_ADDRESS_0,
    IC_ADDRESS_1,
    IC_ADDRESS_2,
    IC_ADDRESS_3
}; 

ASKA_CONF ASKA_conf;

int ASKA_spi_setup()
{
    int err;

	err = spi_is_ready_dt(&spispec);
	if (!err) {
 		printk("Error: SPI device is not ready, err: %d\n", err);
 		return 0;
	} else {
        printk("SPI device is ready\n");    }

	return 1;
}

int ASKA_spi_write_buff(void *bufferData, size_t bufferSize)
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

void ASKA_write_reg(uint8_t IC_addr ,uint8_t add, uint32_t value)
{
    uint8_t tx_buffer[5];
	
    tx_buffer[0] = add | IC_addr;
    
    tx_buffer[1] = (uint8_t)(value >> 24) & 0xff;
    tx_buffer[2] = (uint8_t)(value >> 16) & 0xff;
    tx_buffer[3] = (uint8_t)(value >> 8) & 0xff;
    tx_buffer[4] = (uint8_t)(value >> 0) & 0xff;

    ASKA_spi_write_buff(tx_buffer,5);
	//SPI_transaction(tx_buffer,5);
    //CS_SetLow();    
    //SPI1_WriteBlock(tx_buffer,5);
    //SPI1_BufferWrite(tx_buffer,5);    
    //CS_SetHigh();

}


void ASKA_test1(uint8_t ic_add)
{
        //Disable 
        //ASKA_write_reg(ic_add, ASKA_CONF1,0x00000000); 
		//new program
        /*
		ASKA_write_reg(ic_add, ASKA_ELE1,0x00008000);
		ASKA_write_reg(ic_add, ASKA_ELE2,0x00004000);
		ASKA_write_reg(ic_add, ASKA_CONF0,0x19672190);
		ASKA_write_reg(ic_add, ASKA_CONF1,0x00906420);
         */
        ASKA_write_reg(ic_add, ASKA_CONF1,0x00000000); 
		//new program
		
		ASKA_write_reg(ic_add, ASKA_CONF0,0x0a0147d0);
		ASKA_write_reg(ic_add, ASKA_CONF1,0x00907800);
        ASKA_write_reg(ic_add, ASKA_ELE1,0x00000001);
		ASKA_write_reg(ic_add, ASKA_ELE2,0x00000002);
        
}

void ASKA_test2(uint8_t ic_add)
{
        ASKA_write_reg(ic_add, ASKA_CONF1,0x00000000); 
		//new program
		
		ASKA_write_reg(ic_add, ASKA_CONF0,0x0a1727d0);
		ASKA_write_reg(ic_add, ASKA_CONF1,0x009028a0);
        ASKA_write_reg(ic_add, ASKA_ELE1,0x00000001);
		ASKA_write_reg(ic_add, ASKA_ELE2,0x00000002);
}

void ASKA_test3(uint8_t ic_add)
{
        ASKA_write_reg(ic_add, ASKA_CONF1,0x00000000); 
		//new program
		
		ASKA_write_reg(ic_add, ASKA_CONF0,0x32cb2190);
		ASKA_write_reg(ic_add, ASKA_CONF1,0x00925810);
        ASKA_write_reg(ic_add, ASKA_ELE1,0x00000001);
		ASKA_write_reg(ic_add, ASKA_ELE2,0x00000002);
}

void ASKA_test4(uint8_t ic_add)
{
        ASKA_write_reg(ic_add, ASKA_CONF1,0x00000000); 
		//new program
		
		ASKA_write_reg(ic_add, ASKA_CONF0,0x32c99190);
		ASKA_write_reg(ic_add, ASKA_CONF1,0x0090c808);
        ASKA_write_reg(ic_add, ASKA_ELE1,0x00000001);
		ASKA_write_reg(ic_add, ASKA_ELE2,0x00000002);
}

void ASKA_setProgram( 
        uint8_t ic_add, // 0x00,0x40,0x80,0xc0 (0-3)
        uint32_t freq, // [Hz] 5 - 50 Hz
        uint32_t amplitude, // [mA] 0 - 50 mA
        uint32_t ON_time, // [ds] 0 - 40 ds
        uint32_t OFF_time, //[ds] 0 - 120 ds
        uint32_t ramp_time, // ramptime in [ds]
        uint32_t enable,  // 1 or 0
        uint32_t phaseDuration, // 50 us (1) - 350 us (7) 
        uint32_t ele1,
        uint32_t ele2)
{
    ASKA_NMES_struct ASKA;   
    ASKA_CONF ASIC;
   
    
    // Number of 20 kHz cycles to generate freq
    ASKA.freq = (uint16_t)(BASE_FREQ/freq); 
    
    ASKA.amplitude = amplitude;
    
    // number of ON/OFF pulses 
    ASKA.ON_time = (freq*ON_time)/10;
    
    ASKA.OFF_time = (freq*OFF_time)/10;
    
    // number of pulses during ramp UP/DOWN 
    ASKA.ramp = (freq*ramp_time)/10;
            
    // ramp_factor
    if (ASKA.ramp > 0) {
        ASKA.ramp_factor = ((amplitude*16)/ASKA.ramp);
    } else {
        ASKA.ramp_factor = 0;
    }
        
    ASKA.enable = enable;    
    ASKA.phaseDuration = phaseDuration;    
    ASKA.ele1 = ele1;    
    ASKA.ele2 = ele2;
    
    ASIC.conf0 = 0;
    ASIC.conf1 = 0;
    ASIC.ele1 = 0;
    ASIC.ele2 = 0;
    
    ASIC.conf0 |= (ASKA.freq              & 0x00000fff);
    ASIC.conf0 |= ((ASKA.amplitude << 12) & 0x0003f000);
    ASIC.conf0 |= ((ASKA.ramp << 18)      & 0x00fc0000);
    ASIC.conf0 |= ((ASKA.ON_time << 24)   & 0xff000000);
    
    ASIC.conf1 |= (ASKA.ramp_factor             & 0x000003ff);
    ASIC.conf1 |= ((ASKA.OFF_time << 10)        & 0x000ffc00);
    ASIC.conf1 |= ((ASKA.enable << 20)          & 0x00100000);
    ASIC.conf1 |= ((ASKA.phaseDuration << 21)   & 0x00e00000);
    
    ASIC.ele1 = ASKA.ele1;
    ASIC.ele2 = ASKA.ele2;
    
    
    //Reprogram asic
    ASKA_write_reg(ic_add, ASKA_CONF1,0x00000000); 
    
    ASKA_write_reg(ic_add, ASKA_CONF0,ASIC.conf0);
	ASKA_write_reg(ic_add, ASKA_CONF1,ASIC.conf1);
    ASKA_write_reg(ic_add, ASKA_ELE1,ASIC.ele1);
	ASKA_write_reg(ic_add, ASKA_ELE2,ASIC.ele2);

    //printk("conf0 = 0x%X\n",ASIC.conf0);
    //printk("conf1 = 0x%X\n",ASIC.conf1);

    
}

void ASKA_test()
{
    uint8_t ic_add;

	ic_add = IC_ADDRESS_0;

    ASKA_setProgram(ic_add,50,50,10,30,10,1,4,1,2); //50 Hz, 50mA,1s/3s,ramp 1s, pd 200
    k_msleep(10000);

    ASKA_setProgram(ic_add,10,50,10,10,5,1,4,1,2); //10 Hz, 50mA,1s/1s,ramp 0.5s, pd 200
    k_msleep(10000);

    ASKA_setProgram(ic_add,50,25,10,30,10,1,4,1,2); //50 Hz, 50mA,1s/3s,ramp 1s, pd 200
    k_msleep(10000);

    ASKA_setProgram(ic_add,10,50,10,10,0,1,4,1,2); //10 Hz, 50mA,1s/1s,ramp 0s, pd 200
    k_msleep(10000);

}

void ASKA_getCommand(const uint8_t* data, uint16_t len)
{
    
    
    if(data[0] == 's'){
        //ASKA_NVSWrite();
        return;
    }

    //New packet size
    if (len != 27) {
        printk("ASKA_getCommand - incorrect packet size: %d\n",len);
        return; 
    }

    // Check that command has been sent
    if (data[0] != 'w') {
        return;
    }

    uint8_t ic_add;

    // NOTE: Address should have been checked in app to be between 0-3!
    ic_add = data[1];

    ASKA_conf.ic_addr = ic_add;

    ASKA_conf.conf0 = 0;
    ASKA_conf.conf0 |= (uint32_t)(data[2]<<0);
    ASKA_conf.conf0 |= (uint32_t)(data[3]<<8);
    ASKA_conf.conf0 |= (uint32_t)(data[4]<<16);
    ASKA_conf.conf0 |= (uint32_t)(data[5]<<24);
    
    ASKA_conf.conf1 = 0;
    ASKA_conf.conf1 |= (uint32_t)(data[6]<<0);
    ASKA_conf.conf1 |= (uint32_t)(data[7]<<8);
    ASKA_conf.conf1 |= (uint32_t)(data[8]<<16);
    ASKA_conf.conf1 |= (uint32_t)(data[9]<<24);
    
    ASKA_conf.ele1 = 0;
    ASKA_conf.ele1 |= (uint32_t)(data[10]<<0);
    ASKA_conf.ele1 |= (uint32_t)(data[11]<<8);
    ASKA_conf.ele1 |= (uint32_t)(data[12]<<16);
    ASKA_conf.ele1 |= (uint32_t)(data[13]<<24);
    
    ASKA_conf.ele2 = 0;
    ASKA_conf.ele2 |= (uint32_t)(data[14]<<0);
    ASKA_conf.ele2 |= (uint32_t)(data[15]<<8);
    ASKA_conf.ele2 |= (uint32_t)(data[16]<<16);
    ASKA_conf.ele2 |= (uint32_t)(data[17]<<24);

    // Receive the duration of the programm 
    ASKA_conf.duration = 0;
    ASKA_conf.duration |= (uint32_t)(data[18]<<0);
    ASKA_conf.duration |= (uint32_t)(data[19]<<8);
    ASKA_conf.duration |= (uint32_t)(data[20]<<16);
    ASKA_conf.duration |= (uint32_t)(data[21]<<24);

    // Receive the incativity timer 
    ASKA_conf.inactivity = 0;
    ASKA_conf.inactivity |= (uint32_t)(data[22]<<0);
    ASKA_conf.inactivity |= (uint32_t)(data[23]<<8);
    ASKA_conf.inactivity |= (uint32_t)(data[24]<<16);
    ASKA_conf.inactivity |= (uint32_t)(data[25]<<24);

    // Receive threshold
    ASKA_conf.threshold_db = data[26];

    LOG_INF("ic add = 0x%x",ASKA_conf.ic_addr);
    LOG_INF("conf0 = 0x%x",ASKA_conf.conf0);
    LOG_INF("conf1 = 0x%x",ASKA_conf.conf1);
    LOG_INF("ele1 = 0x%x",ASKA_conf.ele1);
    LOG_INF("ele2 = 0x%x",ASKA_conf.ele2);
    LOG_INF("duration = 0x%x",ASKA_conf.duration);
    LOG_INF("inactivity = 0x%x",ASKA_conf.inactivity);
    LOG_INF("threshold_db = 0x%x",ASKA_conf.threshold_db);


}

void ASKA_sendCommand()
{
     //Reprogram asic
    ASKA_write_reg(ASKA_conf.ic_addr, ASKA_CONF1,0x00000000); 

    //Set the enable bit on
    ASKA_conf.conf1 |= 0x00100000;
    LOG_INF("conf1 = 0x%x",ASKA_conf.conf1);

    
    ASKA_write_reg(ASKA_conf.ic_addr, ASKA_CONF0,ASKA_conf.conf0);
	ASKA_write_reg(ASKA_conf.ic_addr, ASKA_CONF1,ASKA_conf.conf1);
    ASKA_write_reg(ASKA_conf.ic_addr, ASKA_ELE1,ASKA_conf.ele1);
	ASKA_write_reg(ASKA_conf.ic_addr, ASKA_ELE2,ASKA_conf.ele2);

}


void ASKA_disable()
{
     //Reprogram asic
    ASKA_write_reg(ASKA_conf.ic_addr, ASKA_CONF1,0x00000000); 
    
    //Set the enable bit off
    ASKA_conf.conf1 &= ~0x00100000;
    LOG_INF("conf1 = 0x%x",ASKA_conf.conf1);
    //Write to register
	ASKA_write_reg(ASKA_conf.ic_addr, ASKA_CONF1,ASKA_conf.conf1);

}

//Description: Helper function to convert an IC address 0-3 to
//             bit values ready to send via SPI.
//input: IC address 0-3, 
//output: converted address (1byte)
uint8_t ASKA_getICaddress(uint32_t address)
{
      if (address > 3) {
        return 0;
    } else {
        return ASKA_IC_add[address];
    }
}

// Duration of the program. Not implemented yet
uint32_t ASKA_getDuration()
{
    return ASKA_conf.duration;
}

uint32_t ASKA_getInactivity()
{
    return ASKA_conf.inactivity;
}

uint8_t ASKA_getThreshold()
{
    return ASKA_conf.threshold_db;
}


int ASKA_print_current_command(){
    LOG_INF("ic add = 0x%x",ASKA_conf.ic_addr);
    LOG_INF("conf0 = 0x%x",ASKA_conf.conf0);
    LOG_INF("conf1 = 0x%x",ASKA_conf.conf1);
    LOG_INF("ele1 = 0x%x",ASKA_conf.ele1);
    LOG_INF("ele2 = 0x%x",ASKA_conf.ele2);
    LOG_INF("duration = 0x%x",ASKA_conf.duration);
    LOG_INF("inactivity = 0x%x",ASKA_conf.inactivity);
    LOG_INF("threshold_db = 0x%x",ASKA_conf.threshold_db);
    return 0;
}

/*
uint8_t ASKA_enabled(){
    uint8_t bit;
    bit = (ASKA_conf.conf1 & 0x00100000) >> 20;
    printk("The enable bit in now %d\n", bit);
    return bit;
}*/


