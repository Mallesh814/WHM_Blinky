#include "M25PXFlashMemory.h"

//Public Methods


//*****************************************************************************
//
// This function sets up SSI External Flash ID Read
//
//*****************************************************************************
uint32_t M25P_ReadID()
{
   uint32_t ui32Receive, ui32ReadIDFlash;

//   HWREG(M25P_FSS_GPIO + (GPIO_O_DATA + (M25P_FSS_PIN << 2))) = 0x00;


	GPIOPinWrite(M25P_FSS_GPIO,M25P_FSS_PIN, 0X00);	// PULL DOWN Slave Slect PIN

	SSIDataPut(M25P_SSI_BASE, READ_IDENTIFICATION);
	SSIDataPut(M25P_SSI_BASE, 0x00);
	SSIDataPut(M25P_SSI_BASE, 0x00);
	SSIDataPut(M25P_SSI_BASE, 0x00);

	while(SSIBusy(M25P_SSI_BASE));

	GPIOPinWrite(M25P_FSS_GPIO,M25P_FSS_PIN, 0XFF);	// PULL UP Slave Slect PIN

	SSIDataGet(M25P_SSI_BASE, &ui32Receive);
	SSIDataGet(M25P_SSI_BASE, &ui32Receive);
	ui32ReadIDFlash = ui32Receive;
	SSIDataGet(M25P_SSI_BASE, &ui32Receive);
	ui32ReadIDFlash  = (ui32ReadIDFlash << 8) | ui32Receive;
	SSIDataGet(M25P_SSI_BASE, &ui32Receive);
	ui32ReadIDFlash  = (ui32ReadIDFlash << 8) | ui32Receive;

	return ui32ReadIDFlash;
}



uint32_t LCV_readMode() {
	   uint32_t ui32Status;

	//   HWREG(M25P_FSS_GPIO + (GPIO_O_DATA + (M25P_FSS_PIN << 2))) = 0x00;

		GPIOPinWrite(GPIO_PORTC_BASE,GPIO_PIN_6, 0X00);	// PULL DOWN Slave Slect PIN

		SSIDataPut(M25P_SSI_BASE, READ_STATUS_REGISTER);
		SSIDataPut(M25P_SSI_BASE, 0x00);
		while(SSIBusy(M25P_SSI_BASE));

		GPIOPinWrite(GPIO_PORTC_BASE,GPIO_PIN_6, 0XFF);	// PULL UP Slave Slect PIN

		SSIDataGet(M25P_SSI_BASE, &ui32Status);
		SSIDataGet(M25P_SSI_BASE, &ui32Status);

		return ui32Status;
}

uint32_t M25P_readStatus() {
	   uint32_t ui32Status;

	//   HWREG(M25P_FSS_GPIO + (GPIO_O_DATA + (M25P_FSS_PIN << 2))) = 0x00;

		GPIOPinWrite(M25P_FSS_GPIO,M25P_FSS_PIN, 0X00);	// PULL DOWN Slave Slect PIN

		SSIDataPut(M25P_SSI_BASE, READ_STATUS_REGISTER);
		SSIDataPut(M25P_SSI_BASE, 0x00);
		while(SSIBusy(M25P_SSI_BASE));

		GPIOPinWrite(M25P_FSS_GPIO,M25P_FSS_PIN, 0XFF);	// PULL UP Slave Slect PIN

		SSIDataGet(M25P_SSI_BASE, &ui32Status);
		SSIDataGet(M25P_SSI_BASE, &ui32Status);

		return ui32Status;
}

bool M25P_isBusy() {
    return M25P_readStatus() & (1 << 0); //LSB is WIP
}

bool M25P_isWritable() {
    return M25P_readStatus() & (1 << 1); //bit2 is WEL
}

void M25P_programByte(uint32_t addr,uint8_t b) {
    while(M25P_isBusy());

    M25P_enableWrite(); //write is disabled automatically afterwards

    SSIDataPut(SSI0_BASE, PAGE_PROGRAM);
    M25P_sendAddress(addr);
    SSIDataPut(SSI0_BASE, b);
}

void M25P_programBytes(uint32_t addr, uint8_t * buf, int len) {
    int i=0;
    while (i < len) {
        int writeSize = 0x100 - (addr & 0xff);
        if (writeSize > len - i) writeSize = len - i;
        //Serial.print("Writing: 0x");
        //Serial.print(addr,HEX);
        //Serial.print("(");
        //Serial.print(writeSize);
        //Serial.println(")");
//        for (int l=0; l<10; l++) {
//            Serial.print("0x");
//            Serial.print(buf[l]);
//            Serial.print(" ");
//        }
//        Serial.println();
        M25P_programPage(addr,buf,writeSize);
        addr &= 0xffffff00; //start at the beginning of the page
        addr += 0x00000100; //increment page by one
        buf += writeSize;
        i += writeSize;
    }
}


uint8_t M25P_readByte(uint32_t addr) {
	uint8_t value;

	while(M25P_isBusy()) ;

    SSIDataPut(SSI0_BASE, READ_DATA_BYTES);
    M25P_sendAddress(addr);
    SSIDataGet(SSI0_BASE, &value);

    return value;
}

void M25P_readBytes(uint32_t addr, uint8_t * buf, int len) {
    int i=0;
    while (i < len) {
        int readSize = 0x100 - (addr & 0xff);
        if (readSize > len - i) readSize = len - i;
        M25P_readPage(addr,buf,readSize);
        addr &= 0xffffff00; //start at the beginning of the page
        addr += 0x00000100; //increment page by one
        buf += readSize;
        i += readSize;
    }
}

void M25P_readOTP(uint32_t addr, uint8_t * buf, int len) {
    while(M25P_isBusy()) ;

    SSIDataPut(SSI0_BASE, READ_OTP);
    M25P_sendAddress(addr);

    SSIDataGet(SSI0_BASE, &buf[0]); //pass over the dummy uint8_t

    for (int i=0; i<len; i++) {
    	SSIDataGet(SSI0_BASE, &buf[i]);
    }
}

void M25P_programOTP(uint32_t addr, uint8_t * buf, int len) {
    while(M25P_isBusy()) ;

    M25P_enableWrite(); //write is disabled automatically afterwards

    SSIDataPut(SSI0_BASE, PROGRAM_OTP);
    M25P_sendAddress(addr);

    for (int i=0; i<len; i++) {
    	SSIDataPut(SSI0_BASE, buf[i]);
    }
}

void M25P_freezeOTP() {
    while(M25P_isBusy()) ;

    M25P_enableWrite(); //write is disabled automatically afterwards

    SSIDataPut(SSI0_BASE, PROGRAM_OTP);

    M25P_sendAddress(64); //the write lock

    SSIDataPut(SSI0_BASE, 0b000); //write 0s to prevent writing
}

void M25P_eraseSubsector(uint32_t addr) {
    addr = addr & 0xfffff000; //target subsector

    while(M25P_isBusy()) ;

    M25P_enableWrite(); //write is disabled automatically afterwards

    SSIDataPut(SSI0_BASE, SUBSECTOR_ERASE);
    M25P_sendAddress(addr);
}

void M25P_bulkErase() {
    while(M25P_isBusy()) ;

    M25P_enableWrite(); //write is disabled automatically afterwards

    SSIDataPut(SSI0_BASE, BULK_ERASE);

    while(M25P_isBusy()) ; //BULK_ERASE can take awhile, lets just wait until it completes
}

//Private Methods
void M25P_enableWrite() {
	SSIDataPut(SSI0_BASE, WRITE_ENABLE);
}

void M25P_disableWrite() {
    SSIDataPut(SSI0_BASE, WRITE_DISABLE);
}

void M25P_sendAddress(uint32_t addr) {
	SSIDataPut(SSI0_BASE, (addr & 0xff0000) >> 16);	//addr 0
	SSIDataPut(SSI0_BASE, (addr & 0xff00) >> 8);	//addr 1
	SSIDataPut(SSI0_BASE, (addr & 0xff));			//addr 2
}

void M25P_programPage(uint32_t addr, uint8_t * buf, int len) {
    while(M25P_isBusy()) ;

    M25P_enableWrite(); //write is disabled automatically afterwards
//    Serial.print("PAGE_WRITE: ");
//    Serial.print(addr,HEX);
//    Serial.print(" ");
//    Serial.print(len);
//    Serial.print(" [");
//    Serial.print(buf[0]);
//    Serial.print(" ");
//    Serial.print(buf[1]);
//    Serial.print(" ");
//    Serial.print(buf[2]);
//    Serial.println("]");

    SSIDataPut(SSI0_BASE, PAGE_PROGRAM);
    M25P_sendAddress(addr);
    for (int i=0; i<len; i++) {
    	SSIDataPut(SSI0_BASE, buf[i]);
    }
}

void M25P_readPage(uint32_t addr, uint8_t * buf, int len) {
    while(M25P_isBusy()) ;

    SSIDataPut(SSI0_BASE, READ_DATA_BYTES);
    M25P_sendAddress(addr);

    for (int i=0; i<len; i++) {
        SSIDataGet(SSI0_BASE, &buf[i]);
    }
}
