#include "mbed.h"
#include "HeptaXbee.h"
#include "Hepta9axis.h"
#include "HeptaBattery.h"

Serial pc(USBTX,USBRX);




int main()
{
    int i = 0,rcmd=0,cmdflag=0;
    
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
                    
                    
                    
                    
                    wait(0.5);
                }
            }
            xbee.initialize();
        }
    }
}