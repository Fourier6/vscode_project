#include "ccd_kun.h"
ccd_kun::ccd_kun():exp_time(10000)
{

}
void ccd_kun::begin(uint8_t si_pin_temp,uint8_t clk_pin_temp,uint8_t ao_pin_temp)
{
    si_pin=si_pin_temp;
    clk_pin=clk_pin_temp;
    ao_pin=ao_pin_temp;
    pinMode(si_pin, OUTPUT);
    pinMode(clk_pin, OUTPUT);
}