#include <SPI.h>

#define SPI_SPEED 16000000
#define SPI_DATA_ORDER MSBFIRST
#define SPI_MODE SPI_MODE1
#define SPI_DUMMY 0x00

uint8_t SPICS;
void SPIInit(uint8_t );
uint8_t SPIWrite(uint8_t );
