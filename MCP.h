#ifndef MCP_H_
#define MCP_H_

    #include "MCPHal.h"

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
    #define SPI_DUMMY           0x00
    //End SPI_HEADERS

    //Start REGISTERS
    #define CANCTRL     0x0F
    #define CANSTAT     0x0E
    //TX Registers
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
    #define TXRTSCTRL   0x0D
    //End TX REGISTER

    //Start RX REGISTERS
    #define RXB0CTRL    0x60
    #define RXB1CTRL    0x70
    #define BFPCTRL     0x0C
    #define RXB0SIDH    0x61
    #define RXB1SIDH    0x71
    #define RXB0SIDL    0x62
    #define RXB1SIDL    0x72
    #define RXB0EID8    0x63
    #define RXB1EID8    0x73
    #define RXB0EID0    0x64
    #define RXB1EID0    0x74
    #define RXB0DLC     0x65
    #define RXB1DLC     0x75
    #define RXB0D0      0x66
    #define RXB0D1      0x67
    #define RXB0D2      0x68
    #define RXB0D3      0x69
    #define RXB0D4      0x6A
    #define RXB0D5      0x6B
    #define RXB0D6      0x6C
    #define RXB0D7      0x6D
    #define RXB1D0      0x76
    #define RXB1D1      0x77
    #define RXB1D2      0x78
    #define RXB1D3      0x79
    #define RXB1D4      0x7A
    #define RXB1D5      0x7B
    #define RXB1D6      0x7C
    #define RXB1D7      0x7D
    //END RX REGISTERS

    //Start CAN INT SOURCES and FLAGS
    #define CANINTE     0x2B
    #define CANINTF     0x2C
    //End CAN INT SOURCES and FLAGS

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
    #define MASK_TX_REQ             0x08
    #define MASK_RTR                0x40
    #define MASK_RXB0_STAT          0x01
    #define MASK_RXB1_STAT          0x02
    #define MASK_TXB0_STAT          0x04
    #define MASK_TXB1_STAT          0x10
    #define MASK_TXB2_STAT          0x40
    #define MASK_EXT                0x08
    #define MASK_EXT_RTR            0x40
    #define MASK_SRR                0x10
    #define MASK_RX_DLC             0x0F
    //End MASK CONSTANT

    //Start CONSTANT
    #define MAX_TX_DATA_LEN     8
    #define SIDH                0
    #define SIDL                1
    #define EID8                2
    #define EID0                3
    #define DLC                 4
    //End CONSTANT


/*Class is inherited from MCPHal to maintain hardware abstraction.
If you are using different development kit, you might need to change MCPHal class.*/
class MCP : public MCPHal {
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
            TXB0 = TXB0CTRL,
            TXB1 = TXB1CTRL,
            TXB2 = TXB2CTRL
        };

        enum RXBn : uint8_t {
            RXB0 = RXB0CTRL,
            RXB1 = RXB1CTRL
        };

        enum TXBn_PRIORITY : uint8_t {
            PRIORITY_TRIVIAL,
            PRIORITY_LOW,
            PRIORITY_INTERMEDIATE,
            PRIORITY_HIGH
        };

        enum MCP_RETVAL : uint8_t {
            MCP_ERROR,
            MCP_TX_FULL,
            MCP_DLC_ERROR,
            MCP_NO_MSG_AVAILABLE,
            MCP_OK
        };


    private:
        MCP::MCP_RETVAL _sendMessage(MCP::TXBn , uint32_t , bool , uint8_t , uint8_t *, bool );
        void _parseID(uint8_t *, uint32_t , bool , uint8_t , bool );
        void _flushTXBnData(MCP::TXBn );


    public:
        MCP(uint8_t );
        void reset();
        uint8_t readRegister(uint8_t );
        void readRegister(uint8_t , uint8_t , uint8_t *);
        void writeRegister(uint8_t , uint8_t );
        void writeRegister(uint8_t , uint8_t , uint8_t *);
        uint8_t readStatus();
        uint8_t readRXStat();
        uint8_t bitModify(uint8_t , uint8_t , uint8_t );
        uint8_t changeMode(MCP::CHIP_MODE );
        uint8_t setPriority(MCP::TXBn , MCP::TXBn_PRIORITY );
        bool isTXAvailable(MCP::TXBn );
        MCP::MCP_RETVAL sendMessage(MCP::TXBn , uint32_t , bool , uint8_t , uint8_t *);
        MCP::MCP_RETVAL sendMessage(uint32_t , bool , uint8_t , uint8_t *);
        MCP::MCP_RETVAL sendRTR(MCP::TXBn , uint32_t , bool );
        MCP::MCP_RETVAL sendRTR(uint32_t , bool );
};
#endif // MCP_H_
