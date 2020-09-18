#ifndef MCPHAL_H_
#define MCPHAL_H_
    #include <stdint.h>
    //#include <SPI.h>



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
