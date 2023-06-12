#ifndef ccd_kun_h
#define ccd_kun_h
#include "Arduino.h"
class ccd_kun
{
    private:
        uint32_t exp_time;
        uint8_t si_pin,ao_pin,clk_pin,piexl[128];
    public:
    ccd_kun();
    void begin(uint8_t si_pin_temp,uint8_t clk_pin_temp,uint8_t ao_pin_temp);
    
};
#endif