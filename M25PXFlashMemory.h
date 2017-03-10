#ifndef M25PXFlashMemory_h
#define M25PXFlashMemory_h

#include <stdint.h>
#include <stdbool.h>

#include "driverlib/ssi.h"
#include "driverlib/gpio.h"
#include "inc/hw_memmap.h"
#include "inc/hw_gpio.h"

//Configuration
#define M25P_SSI_BASE SSI0_BASE
#define M25P_FSS_GPIO GPIO_PORTA_BASE
#define M25P_FSS_PIN GPIO_PIN_3

//Instructions
#define WRITE_ENABLE                    0x06
#define WRITE_DISABLE                   0x04
#define READ_IDENTIFICATION             0x9f
#define READ_IDENTIFICATION2            0x9e
#define READ_STATUS_REGISTER            0x05
#define WRITE_STATUS_REGISTER           0x01
#define WRITE_LOCK_REGISTER             0xe5
#define READ_LOCK_REGISTER              0xe8
#define READ_DATA_BYTES                 0x03
#define READ_DATA_BYTES_FAST            0x0b
#define DUAL_OUTPUT_FAST_READ           0x3b
#define READ_OTP                        0x4b
#define PROGRAM_OTP                     0x42
#define PAGE_PROGRAM                    0x02
#define DUAL_INPUT_FAST_PROGRAM         0xa2
#define SUBSECTOR_ERASE                 0x20
#define SECTOR_ERASE                    0xd8
#define BULK_ERASE                      0xc7
#define DEEP_POWER_DOWN                 0xb9
#define RELEASE_FROM_DEEP_POWER_DOWN    0xab


#define DUMMY_BYTE       			0x00

	uint32_t M25P_ReadID();
	uint32_t LCV_readMode();
	uint32_t M25P_readStatus();
    bool M25P_isBusy();
    bool M25P_isWritable();
    void M25P_programByte(uint32_t addr,uint8_t b);
    void M25P_programBytes(uint32_t addr, uint8_t * buf, int len);
    uint8_t M25P_readByte(uint32_t addr);
    void M25P_readBytes(uint32_t addr, uint8_t * buf, int len);

    void M25P_readOTP(uint32_t addr, uint8_t * buf, int len);
    void M25P_programOTP(uint32_t addr, uint8_t * buf, int len);
    void M25P_freezeOTP();

    void M25P_eraseSubsector(uint32_t addr);
    void M25P_bulkErase();

    void M25P_enableWrite();
    void M25P_disableWrite();
    void M25P_sendAddress(uint32_t addr);
    void M25P_programPage(uint32_t addr, uint8_t * buf, int len);
    void M25P_readPage(uint32_t addr, uint8_t * buf, int len);

#endif
