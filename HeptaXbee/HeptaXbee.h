#ifndef MBED_HEPTAXBEE_H
#define MBED_HEPTAXBEE_H
#include "mbed.h"

//Xbee Series2

class HeptaXbee
{
public:
    Serial xbee;
    int rcmd;
    int cmdflag;
    HeptaXbee(
        PinName tx,
        PinName rx
    );
    void baud(int rate);
    void xbee_receive(int *xrcmd, int *xcmdflag);
    void initialize();
    void commandget();
    void xbee_transmit(char dataA[],char dataB[],char dataC[],char dataD[],char dataE[],
                       char dataF[],char dataG[],char dataH[],char dataI[],char dataJ[],
                       char dataK[],char dataL[],char dataM[],char dataN[]);
    void puts( char *s );
    void printf(char *format, ... );
    void putc(char data);
    char getc();

private:
};
#endif