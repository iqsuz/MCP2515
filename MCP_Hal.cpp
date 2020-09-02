#include "MCP_Hal.h"

void SPIInit(uint8_t _CS){
    //Setup ChipSelect pin.
    /*SPICS = _CS;
    pinMode(SPICS, OUTPUT);
    digitalWrite(SPICS, HIGH);

    //Run SPI with defined settings in MCP_Hal.h library.
    SPI.begin();
    SPI.beginTransaction(SPI_SPEED, SPI_DATA_ORDER, SPI_MODE);
    SPI.endTransaction();   //SPI reserves its previous settings even if this function is called.*/
}

void CSLow(){
    //digitalWrite(SPICS, LOW);
}

void CSHigh(){
    //digitalWrite(SPICS, HIGH);
}

uint8_t SPIWrite(uint8_t data){
    //return SPI.transfer(data);
}




