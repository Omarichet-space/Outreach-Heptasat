#include "mbed.h"
#include "HeptaBattery.h"
#include"HeptaTemp.h"
 
Serial pc(USBTX, USBRX);
HeptaBattery battery(p16,p26);
HeptaTemp temper(p17);
 
int main()
{
    pc.baud(9600);
    float temp;
    for (int i=0;i<10;i++) {
        temper.temp_sense(&temp);
        pc.printf("temp = %f\r\n",temp);
        wait(1.0);
    }
}