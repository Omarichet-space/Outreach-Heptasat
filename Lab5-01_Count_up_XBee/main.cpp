#include "mbed.h"
#include "HeptaXbee.h"

Serial pc(USBTX,USBRX);
HeptaXbee xbee(p9,p10);

int main()
{
    pc.baud(9600);
    xbee.baud(9600);
    pc.printf("Xbee Count Up Mode\r\n");
    for(int i = 0; i < 10; i++) {
        xbee.printf("num = %d\r\n",i);
        wait(1.0);
    }
}