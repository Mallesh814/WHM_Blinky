/*
 * ram_23lcv1024.c
 *
 * Created: 2014-04-06 09:29:45
 *  Author: Virtmedia
 */ 

#include "ram_23lcv1024.h"


void ram_init(void)
{
	SysCtlPeripheralEnable(LCV_FSS_PERIPH);
	GPIOPinTypeGPIOOutput(LCV_FSS_GPIO, LCV_FSS_PIN);
	GPIOPinWrite(LCV_FSS_GPIO,LCV_FSS_PIN, 0XFF);			// Pull Up FSS PIN of 23LCV1024
    InitSPI(LCV_SSI_BASE, SSI_FRF_MOTO_MODE_0,SSI_MODE_MASTER, 1000000, 8);
}


void ram_send_address(uint32_t address)
{
	SSIDataPut(LCV_SSI_BASE, (address>>16) & 0xFF);
	SSIDataPut(LCV_SSI_BASE, (address>>8) & 0xFF);
	SSIDataPut(LCV_SSI_BASE, (address) & 0xFF);
}

void ram_set_mode(uint8_t mode)
{
	GPIOPinWrite(LCV_FSS_GPIO,LCV_FSS_PIN, 0X00);			// Pull Down FSS PIN of 23LCV1024
	SSIDataPut(LCV_SSI_BASE, RAM_INS_WRMR);
	SSIDataPut(LCV_SSI_BASE, mode);
	GPIOPinWrite(LCV_FSS_GPIO,LCV_FSS_PIN, 0XFF);			// Pull Down FSS PIN of 23LCV1024
}

uint8_t ram_read_mode(void)
{
	uint8_t data;
	GPIOPinWrite(LCV_FSS_GPIO,LCV_FSS_PIN, 0X00);			// Pull Down FSS PIN of 23LCV1024
	SSIDataPut(LCV_SSI_BASE, RAM_INS_RDMR);
	data = SSIDataGet(LCV_SSI_BASE, 0);
	GPIOPinWrite(LCV_FSS_GPIO,LCV_FSS_PIN, 0XFF);			// Pull Down FSS PIN of 23LCV1024
	return data;
}

void ram_write_byte(uint8_t data, uint32_t address)
{
	GPIOPinWrite(LCV_FSS_GPIO,LCV_FSS_PIN, 0X00);			// Pull Down FSS PIN of 23LCV1024
	SSIDataPut(LCV_SSI_BASE, RAM_INS_WRITE);
	ram_send_address(address);
	SSIDataPut(LCV_SSI_BASE, data);
	GPIOPinWrite(LCV_FSS_GPIO,LCV_FSS_PIN, 0XFF);			// Pull Down FSS PIN of 23LCV1024
}

void ram_write_data(uint8_t *buffer, uint8_t buffer_size, uint32_t address)
{
	GPIOPinWrite(LCV_FSS_GPIO,LCV_FSS_PIN, 0X00);			// Pull Down FSS PIN of 23LCV1024
	SSIDataPut(LCV_SSI_BASE, RAM_INS_WRITE);
	ram_send_address(address);
	for(uint8_t i=0;i<buffer_size;++i)
	{
		SSIDataPut(LCV_SSI_BASE, *(buffer+i));
	}
	GPIOPinWrite(LCV_FSS_GPIO,LCV_FSS_PIN, 0XFF);			// Pull Down FSS PIN of 23LCV1024
}

void ram_write_data_pgm(const uint8_t *buffer, uint8_t buffer_size, uint32_t address)
{
	GPIOPinWrite(LCV_FSS_GPIO,LCV_FSS_PIN, 0X00);			// Pull Down FSS PIN of 23LCV1024
	SSIDataPut(LCV_SSI_BASE, RAM_INS_WRITE);
	ram_send_address(address);
	for(uint8_t i=0;i<buffer_size;++i)
	{
		SSIDataPut(LCV_SSI_BASE, pgm_read_byte(buffer+i));
	}
	GPIOPinWrite(LCV_FSS_GPIO,LCV_FSS_PIN, 0XFF);			// Pull Down FSS PIN of 23LCV1024
}


uint8_t ram_read_byte(uint32_t address)
{
	uint8_t data;
	GPIOPinWrite(LCV_FSS_GPIO,LCV_FSS_PIN, 0X00);			// Pull Down FSS PIN of 23LCV1024
	SSIDataPut(LCV_SSI_BASE, RAM_INS_READ);
	ram_send_address(address);
	SSIDataGet(LCV_SSI_BASE, &data);
	GPIOPinWrite(LCV_FSS_GPIO,LCV_FSS_PIN, 0XFF);			// Pull Down FSS PIN of 23LCV1024
	return data;
}

void ram_read_data(uint8_t *buffer, uint8_t buffer_size, uint32_t address)
{
	GPIOPinWrite(LCV_FSS_GPIO,LCV_FSS_PIN, 0X00);			// Pull Down FSS PIN of 23LCV1024
	SSIDataPut(LCV_SSI_BASE, RAM_INS_READ);
	ram_send_address(address);
	while(buffer_size)
	{
		SSIDataGet(LCV_SSI_BASE, buffer);
		++buffer;
		--buffer_size;
	}
	GPIOPinWrite(LCV_FSS_GPIO,LCV_FSS_PIN, 0XFF);			// Pull Down FSS PIN of 23LCV1024
}

void ram_fill_data(uint8_t value, uint32_t lenght, uint32_t address)
{
	GPIOPinWrite(LCV_FSS_GPIO,LCV_FSS_PIN, 0X00);			// Pull Down FSS PIN of 23LCV1024
	SSIDataPut(LCV_SSI_BASE, RAM_INS_WRITE);
	ram_send_address(address);
	while(--lenght)
	{
		SSIDataPut(LCV_SSI_BASE, value);
	}
	GPIOPinWrite(LCV_FSS_GPIO,LCV_FSS_PIN, 0XFF);			// Pull Down FSS PIN of 23LCV1024
}
