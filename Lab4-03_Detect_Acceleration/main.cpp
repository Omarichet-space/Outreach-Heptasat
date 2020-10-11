#include "mbed.h"
#include "HeptaBattery.h"
#include "Hepta9axis.h"

Serial pc(USBTX,USBRX);
HeptaBattery battery(p16,p26);
Hepta9axis n_axis(p28,p27,0xD0,0x18);

int main()
{
    pc.baud(9600);
    float ax,ay,az;
    pc.printf("Accel Sensor Mode\r\n");
    for(int i = 0; i<50; i++) {
        n_axis.sen_acc(&ax,&ay,&az);
        pc.printf("%f,%f,%f\r\n",ax,ay,az);
        wait(1.0);
    }
}