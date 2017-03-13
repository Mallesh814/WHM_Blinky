//*****************************************************************************
//
// blinky.c - Simple example to blink the on-board LED.
//
// Copyright (c) 2012-2014 Texas Instruments Incorporated. All rights reserved.
// Software License Agreement
//
// Texas Instruments (TI) is supplying this software for use solely and
// exclusively on TI's microcontroller products. The software is owned by
// TI and/or its suppliers, and is protected under applicable copyright
// laws. You may not combine this software with "viral" open-source
// software in order to form a larger program.
//
// THIS SOFTWARE IS PROVIDED "AS IS" AND WITH ALL FAULTS.
// NO WARRANTIES, WHETHER EXPRESS, IMPLIED OR STATUTORY, INCLUDING, BUT
// NOT LIMITED TO, IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
// A PARTICULAR PURPOSE APPLY TO THIS SOFTWARE. TI SHALL NOT, UNDER ANY
// CIRCUMSTANCES, BE LIABLE FOR SPECIAL, INCIDENTAL, OR CONSEQUENTIAL
// DAMAGES, FOR ANY REASON WHATSOEVER.
//
// This is part of revision 2.1.0.12573 of the EK-TM4C123GXL Firmware Package.
//
//*****************************************************************************
#include <stdint.h>
#include <stdbool.h>
#include "inc/tm4c123gh6pm.h"
#include "driverlib/sysctl.h"
#include "driverlib/gpio.h"
#include "inc/hw_memmap.h"
#include "driverlib/timer.h"
#include "driverlib/interrupt.h"
#include "driverlib/ssi.h"

#include "configs.h"
#include "parser.h"
#include "M25PXFlashMemory.h"
#include "SRAM_23LCV1024.h"



//*****************************************************************************
//
// Blink the on-board LED.
//
//*****************************************************************************

void Timer0AIntHandler(void);


uint8_t t0 = 0, t1 = 0, call_parser = 0, uart_char = 0;

int main(void) {

	uint32_t deci = 0;
	char ascii[6]="\0";
	uint8_t tx_buf[] = "0123456789ABCDEF";
	uint8_t rx_buf[] = {'\0','\0','\0','\0','\0','\0','\0','\0','\0','\0','\0','\0','\0','\0','\0','0','\0'};

    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOF);
	GPIOPinTypeGPIOOutput(GPIO_PORTF_BASE, GPIO_PIN_4);
	GPIOPinWrite(GPIO_PORTF_BASE,GPIO_PIN_4, 0XFF);	// Toggle LED0 everytime a key is pressed


    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOE);
	GPIOPinTypeGPIOOutput(GPIO_PORTE_BASE, GPIO_PIN_2 | GPIO_PIN_3);
	GPIOPinWrite(GPIO_PORTE_BASE,GPIO_PIN_2 | GPIO_PIN_3, 0XFF);	// Toggle LED0 everytime a key is pressed


    InitConsole();
	transfer("\033[2J\033[H");									// Clear Screen
    transfer("Console Initialized\n\r");


    SysCtlPeripheralEnable(SYSCTL_PERIPH_TIMER0);
//    SysCtlPeripheralEnable(SYSCTL_PERIPH_TIMER1);

    //
    // The Timer0 peripheral must be enabled for use.
    //
    TimerConfigure(TIMER0_BASE, TIMER_CFG_PERIODIC);

    //
    // Set the Timer0B load value to 0.625ms.
    //
    TimerLoadSet(TIMER0_BASE, TIMER_A, SysCtlClockGet() / 2000);

    //TimerControlWaitOnTrigger(TIMER0_BASE, TIMER_B, true);
    //
    // Configure the Timer0B interrupt for timer timeout.
    //
    TimerIntEnable(TIMER0_BASE, TIMER_TIMA_TIMEOUT);

    TimerIntRegister(TIMER0_BASE, TIMER_A, Timer0AIntHandler);
    //
    // Enable the Timer0B interrupt on the processor (NVIC).
    //
    IntEnable(INT_TIMER0A);

    //
    // Enable Timer0B.
    //
    TimerEnable(TIMER0_BASE, TIMER_A );

    transfer("Timer Started\n\r");
    InitFLASH();
    transfer("FLASH Initialized\n\r");

    deci = M25P_readStatus();
    dec_ascii(deci , ascii);
    transfer("FLASH Status : ");
    transfer(ascii);
    transfer("\n\r");





	while (1) {
		if(call_parser){
			call_parser = 0;
//			deci = M25P_readByte(0x0013);
//		    dec_ascii(deci, ascii);
//		    transfer("Data Read Successful : ");
//		    transfer(ascii);
//		    transfer("\n\r");

		    M25P_readBytes(rx_buf, 15, 0x0170);
		    transfer("Data Read Successful : ");
		    transfer(rx_buf);
		    transfer("\n\r");

		    M25P_programBytes(tx_buf, 15, 0x0170);

		    M25P_readBytes(rx_buf, 15, 0x0170);
		    transfer("Data Read Successful : ");
		    transfer(rx_buf);
		    transfer("\n\r");

		    M25P_eraseSector(0x0170);

		    M25P_readBytes(rx_buf, 15, 0x0170);
		    transfer("Data Read Successful : ");
		    transfer(rx_buf);
		    transfer("\n\r");

		    M25P_programBytes(tx_buf, 15, 0x0170);

		    M25P_readBytes(rx_buf, 15, 0x0170);
		    transfer("Data Read Successful : ");
		    transfer(rx_buf);
		    transfer("\n\r");



/*		    M25P_programByte(0x0002,83);


			deci = M25P_readByte(0x0002);
		    dec_ascii(deci, ascii);
		    transfer("Data Read Successful : ");
		    transfer(ascii);
		    transfer("\n\r");
*/		}
	}
}


//*****************************************************************************
//
// The interrupt handler for the Timer0B interrupt.
//
//*****************************************************************************
void
Timer0AIntHandler(void)
{
    //
    // Clear the timer interrupt flag.
    //
    TimerIntClear(TIMER0_BASE, TIMER_TIMA_TIMEOUT);
    t0 ^= 0xFF;
    GPIOPinWrite(GPIO_PORTE_BASE,GPIO_PIN_3, t0);
//    call_parser=1;
}


void isr_uart()
{
	uint32_t u0status;
	u0status = UARTIntStatus(UART0_BASE,true);
	UARTIntClear(UART0_BASE,u0status);
	if(UARTCharsAvail(UART0_BASE))
		{
			uart_char = UARTCharGet(UART0_BASE);
			UARTCharPut(UART0_BASE,uart_char);
			call_parser = 1;
		}
}


