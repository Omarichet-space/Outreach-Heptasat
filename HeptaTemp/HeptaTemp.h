#ifndef MBED_HEPTA2TEMP_H
#define MBED_HEPTA2TEMP_H
#include"mbed.h"

class HeptaTemp
{
public:
    HeptaTemp(PinName pin);
    void temp_sense(float* temp);
    void temp_sense_u16(char* temp_u16);
    void temp_sensing_vol(float* voltage);

private:
//resistance
    float R1 ;
    float R2 ;
    float R3;
    float R4;
    float R5;
    float Pt;
    float R_1;
    float R_2;
//current
    float I;
//voltage
    float Vref;
    
//temperature coefficient
    float ce;
    
    AnalogIn _pin;

};

#endif
