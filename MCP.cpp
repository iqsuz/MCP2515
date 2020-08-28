#include "MCP.h"

/********************************************************************/
MCP::MCP(uint8_t _CS){
    SPIInit(_CS);
}

/********************************************************************/
/*Reset internal registers to default states, 
this function run chip in Configuration Mode.*/
void MCP::reset(){
    CSLow();
    SPIWrite(SPI_RST);
    CSHigh();
}

/********************************************************************/
/*Reads register of given address.*/
uint8_t MCP::readRegister(uint8_t address){
    uint8_t retVal;

    CSLow();
    SPIWrite(SPI_RD);   //Telling chip that we want to read address.
    SPIWrite(address);
    retVal = SPIWrite(SPI_DUMMY);   //Send dummy to keep reading.
    CSHigh();

    return retVal;
}

/********************************************************************/
/*Reads consecutive registers with length of len, 
returned values stored in buff pointer array. */
void MCP::readRegister(uint8_t address, uint8_t len, uint8_t *buff){
    uint8_t status;

    CSLow();
    SPIWrite(SPI_RD);
    SPIWrite(address);

    while(len--){
        *buff++ = SPIWrite(SPI_DUMMY);
    }

    CSHigh();

    return status;
}

/********************************************************************/
/*Writes given data to given register address.*/
void MCP::write(uint8_t address, uint8_t data){
    CSLow();
    SPIWrite(SPI_WR);
    SPIWrite(address);
    SPIWrite(data);
    CSHigh();
}

/********************************************************************/
/*Write consecutive dataset to given address with lenght of len.*/
void MCP::write(uint8_t address, uint8_t len, uint8_t *data){
    CSLow();
    SPIWrite(SPI_WR);
    SPIWrite(address);

    while(len--){
        SPIWrite(*data++);
    }
    CSHigh();
}

/********************************************************************/
/*Reads status of chip.
Bit 0: RX0IF
Bit 1: RX1IF
Bit 2: TX0REQ
Bit 3: TX0IF
Bit 4: TX1REQ
Bit 5: TX1IF
Bit 6: TX2REQ
Bit 7: TX2IF
*/
uint8_t MCP::readStatus(){
    uint8_t retVal;
    
    CSLow();
    SPIWrite(SPI_RD_STAT);
    retVal = SPIWrite(SPI_DUMMY);
    CSHigh();
}

/********************************************************************/
/*
This function returns received data information.
For more info about this function return value, 
refer Figure 12-9 of this chip datasheet.*/
uint8_t MCP::readRXStat(){
    uint8_t retVal;
    
    CSLow();
    SPIWrite(SPI_RX_STAT);
    retVal = SPIWrite(SPI_DUMMY);
    CSHigh();
}

/********************************************************************/
/*This function manipulates bits of given register address.
Data in the register is masked with mask parameter 
then masked bits are changed with data.*/
void MCP::bitModify(uint8_t address, uint8_t mask, uint8_t data){
    CSLow();
    SPIWrite(SPI_BIT_MOD);
    SPIWrite(address);
    SPIWrite(mask);
    SPIWrite(data);
    CSHigh();
}

/********************************************************************/
/*This function changes chip mode based on constant defined in MCP.h under CONSTANT FOR OP MODE. 
This function returns with CANSTAT value*/
uint8_t MCP::changeMode(uint8_t mode){
    bitModify(CANCTRL, MODE_MASK, mode);
    return readRegister(CANSTAT);
    }
}



