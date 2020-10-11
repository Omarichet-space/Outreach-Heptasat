#include "mbed.h"
#include "HeptaBattery.h"
#include "Hepta9axis.h"
Serial pc(USBTX,USBRX);
HeptaBattery battery(p16,p26);
Hepta9axis n_axis(p28,p27,0xD0,0x18);

int main()
{
    pc.baud(9600);
    float gx,gy,gz;
    pc.printf("Gyro Sensor Mode\r\n");
    for(int i = 0; i<50; i++) {
        n_axis.sen_gyro(&gx,&gy,&gz);
        pc.printf("%f,%f,%f\r\n",gx,gy,gz);
        wait(1.0);
    }
}