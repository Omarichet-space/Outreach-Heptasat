#include "mbed.h"
#include "HeptaXbee.h"

Serial pc(USBTX,USBRX);
HeptaXbee xbee(p9,p10);

int main()
{
    pc.baud(9600);
    xbee.baud(9600);
    pc.printf("Xbee Uplink Ok Mode\r\n");
    int rcmd=0,cmdflag=0;
    for(int i = 0; i < 10; i++) {
        xbee.printf("num = %d\r\n",i);
        xbee.xbee_receive(&rcmd,&cmdflag);
        pc.printf("rcmd=%d, cmdflag=%d\r\n",rcmd, cmdflag);
        if (cmdflag == 1) {
            if (rcmd == 'a') {
                pc.printf("Command Get %d\r\n",rcmd);
                xbee.printf("HEPTA Uplink OK\r\n");
            }
            xbee.initialize();
        }
        wait(1.0);
    }
}