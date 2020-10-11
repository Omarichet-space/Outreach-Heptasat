#include "mbed.h"
#include "HeptaXbee.h"
#include "Hepta9axis.h"
#include "HeptaBattery.h"

Serial pc(USBTX,USBRX);
HeptaXbee xbee(p9,p10);
Hepta9axis n_axis(p28,p27,0xD0,0x18);
HeptaBattery battery(p16,p26);

int main()
{
    int i = 0,rcmd=0,cmdflag=0;
    float bt,ax,ay,az;
    xbee.printf("Count Up!\r\n");
    while(1) {
        xbee.printf("num = %d\r\n",i);
        i++;
        wait(1.0);
        xbee.xbee_receive(&rcmd,&cmdflag);
        pc.printf("rcmd=%d, cmdflag=%d\r\n",rcmd, cmdflag);
        if (cmdflag == 1) {
            if (rcmd == 'a') {
                pc.printf("Command Get %d\r\n",rcmd);
                xbee.printf("HEPTA Uplink OK\r\n");
                pc.printf("===================\r\n");
                pc.printf("Accel sensing Mode\r\n");
                pc.printf("===================\r\n");
                for(int ii = 0; ii < 10; ii++) {
                    n_axis.sen_acc(&ax,&ay,&az);
                    battery.vol(&bt);
                    xbee.printf("AX = %f\r\n",ax);
                    xbee.printf("AY = %f\r\n",ay);
                    xbee.printf("AZ = %f\r\n",az);
                    xbee.printf("V = %f\r\n",bt);
                    wait(0.5);
                }
            }
            xbee.initialize();
        }
    }
}