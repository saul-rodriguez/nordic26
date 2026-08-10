/* 
 * File:   aska.h
 * Author: saul
 *
 * Created on September 9, 2024, 12:40 PM
 */

#ifndef ASKA_H
#define	ASKA_H

#include <stdio.h>
#include <zephyr/kernel.h>
#include <zephyr/device.h>
#include <zephyr/devicetree.h>
#include <zephyr/drivers/gpio.h>
#include <zephyr/drivers/spi.h>
#include <zephyr/sys/printk.h>
#include <zephyr/logging/log.h>

//#include "nvs.h"
//#include <zephyr/fs/nvs.h>
//#include <zephyr/storage/flash_map.h>




#define SPIOP	SPI_WORD_SET(8) | SPI_TRANSFER_MSB

    
#define ASKA_CONF0 0x00
#define ASKA_CONF1 0x01
#define ASKA_ELE1  0x02
#define ASKA_ELE2  0x03

typedef enum {
    IC_ADDRESS_0 = 0x00,
    IC_ADDRESS_1 = 0x40,
    IC_ADDRESS_2 = 0x80,
    IC_ADDRESS_3 = 0xc0  
} ic_address_t;

/*    
#define IC_ADDRESS_0 0x00
#define IC_ADDRESS_1 0x40
#define IC_ADDRESS_2 0x80
#define IC_ADDRESS_3 0xc0  
 */  
    
#define BASE_FREQ 20000

/**********************************************/
    /* Configuration words bit mapping            */
    /*                                            */
    /* conf0:                                     */
    /*                                            */
    /* bits 31-24: [7:0] ON_time                  */
    /* bits 23-18: [5:0] ramp                     */
    /* bits 17-12: [5:0] amplitude                */
    /* bits 11-0:  [11:0] freq                    */
    /*                                            */
    /* conf1:                                     */
    /*                                            */
    /* bits 9-0: [9:0] ramp_factor                */
    /* bits 19-10: [9:0] OFF_time                 */
    /* bit 20: enable                             */
    /* bits 23-21: [2:0] phaseDuration            */
    /*                                            */
    /**********************************************/
    
    /*
    wire [5:0] amplitude; //0 - 50 mA
    wire [11:0] freq; // 4.88 Hz (4095) - 50 Hz (400) 
    wire [2:0] phaseDuration; // 50 us (1) - 350 us (7) 
    wire [5:0] ramp; // up to 1s (50 for 50 Hz) 
    wire [9:0] ramp_factor; //[1 - 1024] (amplitude/ramp*2^4)
    wire [7:0] ON_time; // up to 4s (in pulses, 200 for 50 Hz) 
    wire [9:0] OFF_time; // up to 12s (in pulses 600 for 50 Hz) 
     
    assign amplitude = conf0[17:12];
    assign freq = conf0[11:0];
    assign phaseDuration = conf1[23:21];
    assign ramp = conf0[23:18];
    assign ramp_factor = conf1[9:0];
    assign ON_time = conf0[31:24];
    assign OFF_time = conf1[19:10];
    assign enable = conf1[20];      
    
    
typedef struct ASKA_bits_struct {
    unsigned freq           :12;
    unsigned amplitude      :6;
    unsigned ramp           :6;
    unsigned ON_time        :8;
    
    unsigned ramp_factor    :10;
    unsigned OFF_time       :10;
    unsigned enable         :1;
    unsigned phaseDuration  :3;
    unsigned not_used       :8;
    
    unsigned ele1_bits      :32;
    
    unsigned ele2_bits      :32;
    
} ASKA_bits;   

 */
    
typedef struct ASKA_CONF_struct {
    uint32_t conf0;
    uint32_t conf1;
    uint32_t ele1;
    uint32_t ele2;
    uint8_t ic_addr;
    uint32_t duration;
    uint32_t inactivity;
    uint8_t threshold_db;   

} ASKA_CONF;

extern ASKA_CONF ASKA_conf;
/*
typedef struct ASKA_NMES_struct {
    uint16_t freq; //12 bits
    uint8_t amplitude; // 6 bits
    uint8_t ramp; // 6 bits
    uint8_t ON_time; // 8 bits
    
    uint16_t ramp_factor; //10 bits
    uint16_t OFF_time; //10 bits
    uint8_t enable; //1 bit
    uint8_t phaseDuration; //3bits
    
    uint32_t ele1; //32 bits
    uint32_t ele2; //32 bits
        
} ASKA_NMES_struct;
*/

typedef struct ASKA_NMES_struct {
    uint32_t freq; //12 bits
    uint32_t amplitude; // 6 bits
    uint32_t ramp; // 6 bits
    uint32_t ON_time; // 8 bits
    
    uint32_t ramp_factor; //10 bits
    uint32_t OFF_time; //10 bits
    uint32_t enable; //1 bit
    uint32_t phaseDuration; //3bits
    
    uint32_t ele1; //32 bits
    uint32_t ele2; //32 bits
        
} ASKA_NMES_struct;

//extern struct nvs_fs fs;

int ASKA_spi_setup();
int ASKA_spi_write_buff(void *bufferData, size_t bufferSize);

void ASKA_write_reg(uint8_t IC_addr ,uint8_t add, uint32_t value);

void ASKA_test1(uint8_t ic_add);  // 10 Hz, 0s Ramp, ON 1s, OFF 3s, PD 200  
void ASKA_test2(uint8_t ic_add);  // 10 Hz, 0.5s Ramp, ON 1s, OFF 1s, PD 200
void ASKA_test3(uint8_t ic_add);  // 50H Hz, 1s ramp, ON 1s, OFF 3s, PD 200   
void ASKA_test4(uint8_t ic_add);  // 50 Hz, 1s ramp, ON 1s, OFF 1s, PD 200

void ASKA_test();

void ASKA_setProgram(
        uint8_t ic_add,
        uint32_t freq, 
        uint32_t amplitude, 
        uint32_t ON_time, // [ds]
        uint32_t OFF_time, //[ds]
        uint32_t ramp_time, // ramptime in [ds]
        uint32_t enable,  // 1 or 0
        uint32_t phaseDuration, // 50 us (1) - 350 us (7) 
        uint32_t ele1,
        uint32_t ele2);



void ASKA_getCommand(const uint8_t* data, uint16_t len);
void ASKA_sendCommand();


/**
 * @brief Disables the ASIC 
 * 
 * @details Sets the Enable bit low
 */
void ASKA_disable();




uint8_t ASKA_getICaddress(uint32_t address);

/** 
 * @brief Gets the duration of the stimulation program
 * 
 * @return Program duraiton in seconds
 */
uint32_t ASKA_getDuration();

/** 
 * @brief Gets the duration of the stimulation interval
 * 
 * @return Stimulation interval in seconds
 */
uint32_t ASKA_getInactivity();

/**
 * @brief Gets the enegry threshold
 * 
 * @return Threshold in dB
 */
uint8_t ASKA_getThreshold();


int ASKA_print_current_command();
#endif	/* ASKA_H */

