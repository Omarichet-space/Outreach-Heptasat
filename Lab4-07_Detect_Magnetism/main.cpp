#include "mbed.h"
#include "Hepta9axis.h"
#include "HeptaBattery.h"

Serial pc(USBTX,USBRX);
Hepta9axis n_axis(p28,p27,0xD0,0x18);
HeptaBattery battery(p16,p26);
int main()
{
    pc.baud(9600);
    float mx,my,mz;
    pc.printf("Magnetometer Mode\r\n");
    for(int i = 0; i<50; i++) {
        n_axis.sen_mag(&mx,&my,&mz);
        pc.printf("%f,%f,%f\r\n",mx,my,mz);
        wait(1.0);
    }
}