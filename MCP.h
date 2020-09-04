#ifndef _MCP_H_
#define _MCP_H_

    #include "MCP_Hal.h"

    //Start SPI_HEADERS
    #define SPI_RST             0xC0
    #define SPI_RD              0x03
    #define SPI_RDBF_RXB0SIDH   0x90
    #define SPI_RDBF_RXB0D0     0x92
    #define SPI_RDBF_RXB1SIDH   0x94
    #define SPI_RDBF_RXB1D0     0x96
    #define SPI_WR              0x02
    #define SPI_LDBF_TXB0SIDH   0x40
    #define SPI_LDBF_TXB0D0     0x41
    #define SPI_LDBF_TXB1SIDH   0x42
    #define SPI_LDBF_TXB1D0     0x43
    #define SPI_LDBF_TXB2SIDH   0x44
    #define SPI_LDBF_TXB2D0     0x45
    #define SPI_RTS_TXB0        0x81
    #define SPI_RTS_TXB1        0x82
    #define SPI_RTS_TXB2        0x84
    #define SPI_RD_STAT         0xA0
    #define SPI_RX_STAT         0xB0
    #define SPI_BIT_MOD         0x05
    //End SPI_HEADERS

    //Start REGISTERS
    #define CANCTRL     0x0F
    #define CANSTAT     0x0E
    //TX Responsible Registers
    #define TXB0CTRL    0x30
    #define TXB1CTRL    0x40
    #define TXB2CTRL    0x50
    #define TXB0SIDH    0x31
    #define TXB1SIDH    0x41
    #define TXB2SIDH    0x51
    #define TXB0SIDL    0x32
    #define TXB1SIDL    0x42
    #define TXB2SIDL    0x52
    #define TXB0EID8    0x33
    #define TXB1EID8    0x43
    #define TXB2EID8    0x53
    #define TXB0EID0    0x34
    #define TXB1EID0    0x44
    #define TXB2EID0    0x54
    #define TXB0D0      0x36
    #define TXB0D1      0x37
    #define TXB0D2      0x38
    #define TXB0D3      0x39
    #define TXB0D4      0x3A
    #define TXB0D5      0x3B
    #define TXB0D6      0x3C
    #define TXB0D7      0x3D
    #define TXB1D0      0x46
    #define TXB1D1      0x47
    #define TXB1D2      0x48
    #define TXB1D3      0x49
    #define TXB1D4      0x4A
    #define TXB1D5      0x4B
    #define TXB1D6      0x4C
    #define TXB1D7      0x4D
    #define TXB2D0      0x56
    #define TXB2D1      0x57
    #define TXB2D2      0x58
    #define TXB2D3      0x59
    #define TXB2D4      0x5A
    #define TXB2D5      0x5B
    #define TXB2D6      0x5C
    #define TXB2D7      0x5D
    #define TXB0DLC     0x35
    #define TXB1DLC     0x45
    #define TXB2DLC     0x55
    #define TXTRSCTRL   0x0D
    //End REGISTER

    //Start REGISTER BITS
    #define ABTF        0x40
    #define MLAO        0x20
    #define TXERR       0x10
    //End REGISTER BITS

    //Start MASK CONSTANT
    #define MASK_MODE               0xE0
    #define MASK_PRIORITY           0x03
    #define MASK_CANID_SIDL         (uint32_t) 0x00000007
    #define MASK_CANID_EID1716      (uint32_t) 0x00000003
    #define MASK_TXREQ             0x08
    //End MASK CONSTANT

    //Start CONSTANT
    #define MAX_TX_DATA_LEN     8
    #define SIDH                0
    #define SIDL                1
    #define EID8                2
    #define EID0                3
    #define DLC                 4
    //End CONSTANT

    class MCP{
    private:

    public:
        enum CHIP_MODE : uint8_t {
            NORMAL_MODE = 0x00,
            SLEEP_MODE = 0x20,
            LOOPBACK_MODE = 0x40,
            LISTEN_ONLY_MODE = 0x60,
            CONFIG_MODE = 0x80
        };

        enum TXBn : uint8_t {
            TXB0,
            TXB1,
            TXB2
        };

        enum TXBn_PRIORITY : uint8_t {
            PRIORITY_TRIVIAL,
            PRIORITY_LOW,
            PRIORITY_INTERMEDIATE,
            PRIORITY_HIGH
        };

        struct CanFrame {
            uint32_t can_id;
            uint8_t ext;
            uint8_t rtr;
            uint8_t dlc;
            uint8_t data[MAX_TX_DATA_LEN];
        };

        enum MCP_RETVAL : uint8_t {
            MCP_ERROR,
            MCP_OK     //delete this

        };

    public:
        MCP(uint8_t );
        void reset();
        uint8_t readRegister(uint8_t );
        void readRegister(uint8_t, uint8_t, uint8_t *);
        void writeRegister(uint8_t, uint8_t );
        void writeRegister(uint8_t, uint8_t, uint8_t *);
        uint8_t readStatus();
        uint8_t readRXStat();
        uint8_t bitModify(uint8_t, uint8_t, uint8_t );
        uint8_t changeMode(MCP::CHIP_MODE );
        uint8_t setPriority(MCP::TXBn, MCP::TXBn_PRIORITY );
        MCP_RETVAL sendMessage(MCP::TXBn, uint32_t, uint8_t, uint8_t, uint8_t *);

    };

#endif // _MCP_H_
