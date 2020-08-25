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

    class MCP{
    private:

    public:

    };

#endif // _MCP_H_
