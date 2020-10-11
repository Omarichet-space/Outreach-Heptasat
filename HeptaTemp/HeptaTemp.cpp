#include"HeptaTemp.h"
#include"mbed.h"

HeptaTemp::HeptaTemp(PinName pin) : _pin(pin)
{

}

void HeptaTemp::temp_sense(float* temp)
{
//resistance
    R1 = 2500;
    R2 = 2500;
    R3 = 110;
    R4 = 1000;
    R5 = 68000;
    Pt = 100;
    R_1 = 3;
    R_2 = 2;

//current
    I = 0.001;

//voltage
    Vref = 2.5;

//Gain&Offset
    float gain = -R5*I/R4;
    float off = Vref+I*R3;

//temperature coefficient
    ce = 0.003851;
    float volt = (_pin.read())*3.3*(R_1 + R_2)/R_1;
    float Rth = (volt-off)/gain+R3;
    *temp = (Rth-Pt)/(ce*Pt);
    
// delay some time before reading again
    //wait(0.5);

}

void HeptaTemp::temp_sense_u16(char* temp_u16)
{
    unsigned short temp_datas;
    char temp1[8]= {0x00},temp2[8]= {0x00};
    temp_datas=_pin.read_u16()>>4;
    sprintf( temp1, "%02X", (temp_datas >> 8) & 0x0F);
    sprintf( temp2, "%02X", (temp_datas) & 0xFF);
    temp_u16[0]=temp1[0];
    temp_u16[1]=temp1[1];
    temp_u16[2]=temp2[0];
    temp_u16[3]=temp2[1];
    //*dsize = 4;
}

void HeptaTemp::temp_sensing_vol(float* voltage)
{
    R_1 = 3;
    R_2 = 2;
    *voltage = (_pin.read())*3.3*(R_1 + R_2)/R_1;

}