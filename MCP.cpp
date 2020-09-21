#include "MCP.h"

MCP::MCP(uint8_t chipSelect):MCPHal(chipSelect){}

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
uint8_t MCP::readRegister(uint8_t reg_add){
    uint8_t retVal;

    CSLow();
    SPIWrite(SPI_RD);
    SPIWrite(reg_add);
    retVal = SPIWrite(SPI_DUMMY);
    CSHigh();

    return retVal;
}


/********************************************************************/
/*Reads consecutive registers with length of len,
returned values stored in buff pointer array. */
void MCP::readRegister(uint8_t reg_add, uint8_t len, uint8_t *buff){
    CSLow();
    SPIWrite(SPI_RD);
    SPIWrite(reg_add);

    while(len--){
        *buff++ = SPIWrite(SPI_DUMMY);
    }

    CSHigh();
}

/********************************************************************/
/*Writes given data to given register address.*/
void MCP::writeRegister(uint8_t reg_add, uint8_t data){
    CSLow();
    SPIWrite(SPI_WR);
    SPIWrite(reg_add);
    SPIWrite(data);
    CSHigh();
}

/********************************************************************/
/*Write consecutive dataset to given address with lenght of len.*/
void MCP::writeRegister(uint8_t reg_add, uint8_t len, uint8_t *data){
    CSLow();
    SPIWrite(SPI_WR);
    SPIWrite(reg_add);

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
uint8_t MCP::bitModify(uint8_t reg_add, uint8_t mask, uint8_t data){
    uint8_t retVal;

    CSLow();
    SPIWrite(SPI_BIT_MOD);
    SPIWrite(reg_add);
    SPIWrite(mask);
    SPIWrite(data);
    CSHigh();

    CSLow();
    retVal = readRegister(reg_add);
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

    bitModify(tx, MASK_PRIORITY, tx_priority);
    retVal = readRegister(tx);

    return retVal;
}

/********************************************************************/
/*This function set priority of TX buffer in four different levels.
tx should be MCP::TXBn type, tx_priority should be MCP::TXBn_PRIORITY.
*/
MCP::MCP_RETVAL MCP::sendMessage(MCP::TXBn tx, uint32_t can_id, uint8_t ext, uint8_t dlc, uint8_t *data){
    uint8_t tx_header[5];

    if(isTXPending(tx)){
        return MCP::MCP_TX_FULL;
    }

    if(ext > 1 || dlc > 8){
        return MCP::MCP_EXT_DLC_ERROR;
    }

    _flushTXBnData(tx);

    _parseID(tx_header, can_id, ext, dlc, 0);

    switch(tx){
        case MCP::TXB0:
            writeRegister(SPI_LDBF_TXB0SIDH, 5, tx_header);     //Prepared CAN ID.
            writeRegister(TXB0D0, dlc, data);                   //Messages that will be sent are being written on registers.
            bitModify(TXB0CTRL, MASK_TXREQ, MASK_TXREQ);        //Send message.
            break;
        case MCP::TXB1:
            writeRegister(SPI_LDBF_TXB1SIDH, 5, tx_header);     //Prepared CAN ID.
            writeRegister(TXB1D0, dlc, data);                   //Messages that will be sent is being written in registers.
            bitModify(TXB1CTRL, MASK_TXREQ, MASK_TXREQ);        //Send message.
            break;
        case MCP::TXB2:
            writeRegister(SPI_LDBF_TXB2SIDH, 5, tx_header);     //Prepared CAN ID.
            writeRegister(TXB2D0, dlc, data);                   //Messages that will be sent is being written in registers.
            bitModify(TXB2CTRL, MASK_TXREQ, MASK_TXREQ);        //Send message.
            break;
    }

    return MCP::MCP_OK;
}

/********************************************************************/
/*This function parse given ID based on  TX_SIDL, TX_SIDH, TX_EID8, TX_EID0,
TX_DLC registers structure and returns on array given in *parser_id.
*/
void MCP::_parseID(uint8_t *parsed_id, uint32_t can_id, uint8_t ext, uint8_t dlc, uint8_t rtr){
    if (ext == 0){
        parsed_id[SIDL] = can_id << 5; //SIDL
        parsed_id[SIDH] = can_id >> 3; //SIDG
        parsed_id[EID8] = 0;    //EID8
        parsed_id[EID0] = 0;    //EID0
    }else{
        parsed_id[SIDL] = can_id << 5;     //Not completed yet. EXT bit and EID17, EID16 bit should be added here.
        parsed_id[SIDH] = can_id >>= 3;    //SIDH
        parsed_id[EID0] = can_id >>= 8;    //EID0
        parsed_id[EID8] = can_id >>= 8;    //EID8
        can_id >>= 8;
        parsed_id[SIDL] = parsed_id[SIDL] + 8 + (can_id & MASK_CANID_EID1716);   // Added +8 to activate extended ID.    //SIDL
    }

    parsed_id[DLC] = dlc | (rtr << 6);   //DLC
}

/********************************************************************/
/*This function determines if given TX buffer has pending transmission.
If there is pending transmission this function returns 1.
If there is no pending transmission this function returns 0.
*/
uint8_t MCP::isTXPending(MCP::TXBn tx){
    return (readRegister(tx) & MASK_TXREQ) >> 3;
}

/********************************************************************/
/*This function flush data slot of given TX buffer.
*/
void MCP::_flushTXBnData(MCP::TXBn tx){
    uint8_t TXBnDm_flush[8]{};
    writeRegister((tx+6), 8, TXBnDm_flush); //tx + 6 = TXBnDm
}



//RXBnCTRL will indicate acceptance filter number that enabled reception and whether the
//received message is a RTR.
//roll over if a message comply acceotence rule of RXB0 but it is occupied with another valid data
//overflow error will occur. If you active roll over, regardless of RXB1 acceptence filter and mask
//data will be transfered to RXB1.
