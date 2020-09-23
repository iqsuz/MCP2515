#ifndef MCPHAL_H_
#define MCPHAL_H_
    #include <stdint.h>
    #include <SPI.h>

    #define SPI_SPEED 16000000
    #define SPI_DATA_ORDER MSBFIRST
    #define SPI_MODE SPI_MODE1

class MCPHal {
    protected:
        uint8_t chipSelect;

    protected:
        void SPIInit(uint8_t );
        void CSLow();
        void CSHigh();
        uint8_t SPIWrite(uint8_t );

    public:
        MCPHal(uint8_t );

};

#endif // MCPHAL_H_
