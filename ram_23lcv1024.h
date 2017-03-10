/*
 * ram_23lcv1024.h
 *
 * Created: 2014-04-06 09:30:05
 *  Author: Virtmedia
 */ 

#ifndef RAM_23LCV1024_H_
#define RAM_23LCV1024_H_

#include <stdint.h>
#include <stdbool.h>

#include "driverlib/ssi.h"
#include "driverlib/gpio.h"
#include "inc/hw_memmap.h"
#include "inc/hw_gpio.h"
#include "driverlib/sysctl.h"


//SSI Base and FSS Pin Info
#define LCV_SSI_BASE SSI0_BASE
#define LCV_FSS_PERIPH SYSCTL_PERIPH_GPIOC
#define LCV_FSS_GPIO GPIO_PORTC_BASE
#define LCV_FSS_PIN GPIO_PIN_6

//Instruction set
#define RAM_INS_READ	0x03	//Read data from memory array beginning at selected address
#define RAM_INS_WRITE	0x02	//Write data to memory array beginning at selected address
#define RAM_INS_EDIO	0x3B	//Enter Dual I/O access
#define RAM_INS_RSTIO	0xFF	//Reset Dual I/O access
#define RAM_INS_RDMR	0x05	//Read Mode Register
#define RAM_INS_WRMR	0x01	//Write Mode Register

//Mode Register
#define RAM_MODE_BYTE	0x00
#define RAM_MODE_PAGE	0x80
#define RAM_MODE_SEQUENTIAL	0x40


uint32_t ram_address;

#define RAM_MAX_ADDRESS	0x1FFFFh


void ram_init(void);

void ram_select(void);

void ram_deselect(void);

void ram_send_address(uint32_t address);

void ram_set_mode(uint8_t mode);

uint8_t ram_read_mode(void);

void ram_write_byte(uint8_t data, uint32_t address);

void ram_write_data(uint8_t *buffer, uint8_t buffer_size, uint32_t address);

void ram_write_data_pgm(const uint8_t *buffer, uint8_t buffer_size, uint32_t address);

uint8_t ram_read_byte(uint32_t address);

void ram_read_data(uint8_t *buffer, uint8_t buffer_size, uint32_t address);

void ram_fill_data(uint8_t value, uint32_t lenght, uint32_t address);

#endif /* RAM_23LCV1024_H_ */
