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
    uint8_t ret_val;

    CSLow();
    SPIWrite(SPI_RD);
    SPIWrite(reg_add);
    ret_val = SPIWrite(SPI_DUMMY);
    CSHigh();

    return ret_val;
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
    uint8_t ret_val;

    CSLow();
    SPIWrite(SPI_RD_STAT);
    ret_val = SPIWrite(SPI_DUMMY);
    CSHigh();

    return ret_val;
}

/********************************************************************/
/*
This function returns received data information.
For more info about this function return value,
refer Figure 12-9 of this chip datasheet.*/
uint8_t MCP::readRXStat(){
    uint8_t ret_val;

    CSLow();
    SPIWrite(SPI_RX_STAT);
    ret_val = SPIWrite(SPI_DUMMY);
    CSHigh();

    return ret_val;
}

/********************************************************************/
/*This function manipulates bits of given register address.
Data in the register is masked with mask parameter
then masked bits are changed with data.*/
uint8_t MCP::bitModify(uint8_t reg_add, uint8_t mask, uint8_t data){
    uint8_t ret_val;

    CSLow();
    SPIWrite(SPI_BIT_MOD);
    SPIWrite(reg_add);
    SPIWrite(mask);
    SPIWrite(data);
    CSHigh();

    CSLow();
    ret_val = readRegister(reg_add);
    CSHigh();

    return ret_val;
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
    uint8_t ret_val;

    bitModify(tx, MASK_PRIORITY, tx_priority);
    ret_val = readRegister(tx);

    return ret_val;
}

/********************************************************************/
/*This function returns false if given TX buffer has pending transmission.
Otherwise it returns true.
*/
bool MCP::isTXAvailable(MCP::TXBn tx){
    uint8_t tx_stat {readStatus()};

    switch(tx){
        case MCP::TXB0:
            return (((tx_stat & MASK_TXB0_STAT) >> 2) > 0) ? true : false;
        case MCP::TXB1:
            return (((tx_stat & MASK_TXB1_STAT) >> 4) > 0) ? true : false;
        case MCP::TXB2:
            return (((tx_stat & MASK_TXB1_STAT) >> 6) > 0) ? true : false;
    }

    return false;
}

/********************************************************************/
/*This function is to send CAN message.
tx = desired TX buffer.
can_id = desired CAN ID
ext = extended id. if it is true, it activates 29 bits extended id. if it is false, it utilizes standard 11 bits id.
dlc = data length
*data = message that is wanted to send.
*/
MCP::MCP_RETVAL MCP::sendMessage(MCP::TXBn tx, uint32_t can_id, bool ext, uint8_t dlc, uint8_t *data){
    return _sendMessage(tx, can_id, ext, dlc, data, false);
}

/********************************************************************/
/*This function determines its own TX buffer based on vacant TX buffers.
if there is vacant TX buffer function utilizes otherwise it returns TX_BUFF_FULL.
can_id = desired CAN ID
ext = extended id. if it is true, it activates 29 bits extended id. if it is false, it utilizes standard 11 bits id.
dlc = data length
*data = message that is wanted to send.
*/
MCP::MCP_RETVAL MCP::sendMessage(uint32_t can_id, bool ext, uint8_t dlc, uint8_t *data){
    MCP::TXBn txs[] {MCP::TXB0, MCP::TXB1, MCP::TXB2};
    MCP::TXBn available_tx;
    bool ctrl {false};

    for(int i = 0; i <= 2; i++){
        if(isTXAvailable(txs[i]) != false){
            available_tx = txs[i];
            ctrl = true;
            break;
        }
    }

    if(ctrl == false){
        return MCP::MCP_TX_FULL;
    }

    return _sendMessage(available_tx, can_id, ext, dlc, data, 0);

}

/********************************************************************/
/*This function transmit remote transmission request with given TX Buffer.
can_id = desired CAN ID
ext = extended id. if it is true, it activates 29 bits extended id. if it is false, it utilizes standard 11 bits id.
*/
MCP::MCP_RETVAL MCP::sendRTR(MCP::TXBn tx, uint32_t can_id, bool ext){
    return _sendMessage(tx, can_id, ext, 0, nullptr, 1);
}

/********************************************************************/
/*This function determines its own TX buffer based on vacant TX buffers.
This function transmit remote transmission request with given TX Buffer.
can_id = desired CAN ID
ext = extended id. if it is true, it activates 29 bits extended id. if it is false, it utilizes standard 11 bits id.
*/
MCP::MCP_RETVAL MCP::sendRTR(uint32_t can_id, bool ext){
    MCP::TXBn txs[] {MCP::TXB0, MCP::TXB1, MCP::TXB2};
    MCP::TXBn available_tx;
    bool ctrl {false};

    for(int i = 0; i <= 2; i++){
        if(isTXAvailable(txs[i]) != false){
            available_tx = txs[i];
            ctrl = true;
            break;
        }
    }

    if(ctrl == false){
        return MCP::MCP_TX_FULL;
    }

    return _sendMessage(available_tx, can_id, ext, 0, nullptr, 1);
}


/********************************************************************/
/*This function is a private function where other overloaded sendMessage functions uses.
It may be considered as root sendMessage function.
can_id = desired CAN ID
ext = extended id. if it is true, it activates 29 bits extended id. if it is false, it utilizes standard 11 bits id.
*/
MCP::MCP_RETVAL MCP::_sendMessage(MCP::TXBn tx, uint32_t can_id, bool ext, uint8_t dlc, uint8_t *data, bool rtr){
    uint8_t tx_header[5] {};

    if(!isTXAvailable(tx)){     //Does given TX Buffer have pending message?
        return MCP::MCP_TX_FULL;
    }

    if(dlc > 8){    //Failed if DLC is greater than 8.
        return MCP::MCP_DLC_ERROR;
    }

    _parseID(tx_header, can_id, ext, dlc, rtr);     //Parse given can ID into SIDH, SIDL, EID8, EID0 and DLC.
    _flushTXBnData(tx);     //Flush data slot for given TX buffer.

    writeRegister((tx + 1), 5, tx_header);  //tx + 1 = TXSIDH.... Write parsed id to related registers.
    if(rtr == 0){   //Is that a remote transmission request?
        writeRegister((tx + 6), dlc, data);    //If not, write data to related registers... tx + 6 = TXBnD0
    }
    bitModify(tx, MASK_TX_REQ, MASK_TX_REQ); //Start transmission.

    return MCP::MCP_OK;
}

/********************************************************************/
/*This function parse given id into tx_header array based on chips register instruction.
*/
void MCP::_parseID(uint8_t *parsed_id, uint32_t can_id, bool ext, uint8_t dlc, bool rtr){
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

    parsed_id[DLC] = dlc | (static_cast<uint8_t>(rtr) << 6);   //DLC
}

/********************************************************************/
/*This function flush data slots with given TX Buffer.
*/
void MCP::_flushTXBnData(MCP::TXBn tx){
    uint8_t TXBnDm_flush[8]{};
    writeRegister((tx + 6), 8, TXBnDm_flush); //tx + 6 = TXBnDm
}


MCP::MCP_RETVAL MCP::readMessage(MCP::RXBn rx, uint32_t *can_id, uint8_t *data, bool *ext, uint8_t *dlc, bool *rtr){
    uint8_t msg[5] {};

    //Check if message is available for given RX Buffer.
    if(!isRXAvailable(rx)){
        return MCP::MCP_NO_MSG_AVAILABLE;
    }

    //receive RX header info. this includes id, ext, rtr, dlc informations.
    readRegister((rx + 0x01), 5, msg);

    //merge dispersed uint8_t data into uin32_t can_id
    *ext = static_cast<bool>((msg[SIDL] & MASK_EXT) > 0);
    *rtr = *ext ? (msg[DLC] & MASK_EXT_RTR) > 0 : (msg[SIDL] & MASK_SRR) > 0
    *dlc = msg[DLC] & MASK_RX_DLC;

    *can_id = 0;
    *can_id = (msg[SIDL] >> 5) + (msg[SIDH] << 3);

    if(ext){
        *can_id += (msg[EID8] << 8);
        *can_id += (msg[EID0] << 8);
    }



    //receive data if message is not rtr.
    //clear interrupt flags to get empty buffer.
}

bool MCP::isRXAvailable(MCP::RXBn rx){
    switch(rx){
        case MCP::RXB0:
            return ((readStatus() & MASK_RXB0_STAT) > 0) ? true : false;
        case MCP::RXB1:
            return (((readStatus() & MASK_RXB1_STAT) >> 1) > 0) ? true : false;
    }

    return false;
}
/*
//RXBnCTRL will indicate acceptance filter number that enabled reception and whether the
//received message is a RTR.
//roll over if a message comply acceotence rule of RXB0 but it is occupied with another valid data
//overflow error will occur. If you active roll over, regardless of RXB1 acceptence filter and mask
//data will be transfered to RXB1.
//CANINTF should be removed after message reading. Otherwise MAB will not move accepted messages to
//RX Buffers.
*/
