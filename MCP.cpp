#include "MCP.h"

/********************************************************************/
/*Construction function initialize SPI by using HAL lib.*/
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
    CSLow();
    SPIWrite(SPI_RD);
    SPIWrite(address);

    while(len--){
        *buff++ = SPIWrite(SPI_DUMMY);
    }

    CSHigh();
}

/********************************************************************/
/*Writes given data to given register address.*/
void MCP::writeRegister(uint8_t address, uint8_t data){
    CSLow();
    SPIWrite(SPI_WR);
    SPIWrite(address);
    SPIWrite(data);
    CSHigh();
}

/********************************************************************/
/*Write consecutive dataset to given address with lenght of len.*/
void MCP::writeRegister(uint8_t address, uint8_t len, uint8_t *data){
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

    return retVal;
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

    return retVal;
}

/********************************************************************/
/*This function manipulates bits of given register address.
Data in the register is masked with mask parameter
then masked bits are changed with data.*/
uint8_t MCP::bitModify(uint8_t address, uint8_t mask, uint8_t data){
    uint8_t retVal;

    CSLow();
    SPIWrite(SPI_BIT_MOD);
    SPIWrite(address);
    SPIWrite(mask);
    SPIWrite(data);
    CSHigh();

    CSLow();
    retVal = readRegister(address);
    CSHigh();

    return retVal;
}

/********************************************************************/
/*This function changes chip mode based on constant defined in MCP.h under CONSTANT FOR OP MODE.
This function returns with CANSTAT value
!!!!!!!!!!!!THIS FUNCTION WILL NOT CHANGE OPERATION MODE
UNTIL ALL PENDING TRANSMISSION COMPLETED!!!!!!!!!!!
*/
uint8_t MCP::changeMode(MCP::CHIP_MODE mode){
    bitModify(CANCTRL, MASK_MODE, mode);
    return readRegister(CANSTAT);
}

/********************************************************************/
/*This function set priority of TX buffer in four different levels.
tx should be MCP::TXBn type, tx_priority should be MCP::TXBn_PRIORITY.
*/
uint8_t MCP::setPriority(MCP::TXBn tx, MCP::TXBn_PRIORITY tx_priority){
    uint8_t retVal;

    switch(tx){
        case MCP::TXB0:
            bitModify(TXB0CTRL, MASK_PRIORITY, tx_priority);
            retVal = readRegister(TXB0CTRL);
            break;
        case MCP::TXB1:
            bitModify(TXB1CTRL, MASK_PRIORITY, tx_priority);
            retVal = readRegister(TXB0CTRL);
            break;
        case MCP::TXB2:
            bitModify(TXB2CTRL, MASK_PRIORITY, tx_priority);
            retVal = readRegister(TXB1CTRL);
            break;
    }

    return retVal;
}


/********************************************************************/
/*This function set priority of TX buffer in four different levels.
tx should be MCP::TXBn type, tx_priority should be MCP::TXBn_PRIORITY.
*/

MCP::MCP_RETVAL MCP::sendMessage(MCP::TXBn tx, uint32_t can_id, uint8_t ext, uint8_t dlc, uint8_t *data){
    uint8_t tx_header[5], sidl, sidh, eid0, eid8;

    if(ext > 1 || ext < 0){
        return MCP::MCP_ERROR;
    }

    if(dlc > 8){
        return MCP::MCP_ERROR;
    }

    tx_header[0] = can_id >> 3;
    tx_header[1] = (ext == 0) ? (can_id & MASK_CANID_SIDL) << 5 : (can_id & MASK_CANID_SIDL) << 5 +

    if(ext == 0){
        tx_header[0] = can_id >> 3;
        tx_header[1] = (can_id & MASK_CANID_SIDL) << 5;
        tx_header[2] = 0;
        tx_header[3] = 0;
        tx_header[4] = dlc;

        switch(tx){
            case MCP::TXB0:
                writeRegister(SPI_LDBF_TXB0SIDH, 5, tx_header);
                break;
            case MCP::TXB1:
                writeRegister(SPI_LDBF_TXB1SIDH, 5, tx_header);
                break;
            case MCP::TXB2:
                writeRegister(SPI_LDBF_TXB2SIDH, 5, tx_header);
                break;
        }
    }

}



