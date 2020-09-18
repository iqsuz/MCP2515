#include "MCPHal.h"

MCPHal::MCPHal(uint8_t chipSelect):chipSelect {chipSelect}{
    SPIInit(chipSelect);
}

void MCPHal::SPIInit(uint8_t chipSelect){
    //Setup ChipSelect pin.
    pinMode(chipSelect, OUTPUT);
    digitalWrite(chipSelect, HIGH);

    //Run SPI with defined settings in MCP_Hal.h library.
    SPI.begin();
    SPI.beginTransaction(SPISettings(SPI_SPEED, SPI_DATA_ORDER, SPI_MODE));
    SPI.endTransaction();   //SPI reserves its previous settings even if this function is called.
}

void MCPHal::CSLow(){
    digitalWrite(chipSelect, LOW);
}

void MCPHal::CSHigh(){
    digitalWrite(chipSelect, HIGH);
}

uint8_t MCPHal::SPIWrite(uint8_t data){
    return SPI.transfer(data);
}


