#include "mbed.h"
#include "SDFileSystem.h"
#include "HeptaXbee.h"
#include "HeptaTemp.h"

Serial pc(USBTX,USBRX);
SDFileSystem sd(p5, p6, p7, p8, "sd");
HeptaXbee xbee(p9,p10);
HeptaTemp temper(p17);
Timer tt;

int main()
{
    int rcmd=0,cmdflag=0,i=0;
    float voltage,timer,temp;
    mkdir("/sd/mydir", 0777);
    xbee.printf("Count Up!\r\n");
    while(1) {
        xbee.printf("num = %d\r",i);
        i++;
        xbee.xbee_recieve(&rcmd,&cmdflag);
        //pc.printf("rcmd=%d, cmdflag=%d\r\n",rcmd, cmdflag);
        if (cmdflag == 1) {
            //pc.printf("Command Get %d\r\n",rcmd);
            if(rcmd == 'a') {
                FILE *fp = fopen("/sd/mydir/test_thermal_10_deg.csv", "w");
                for(int i = 0; i<120; i++) {
                    tt.start();
                    temper.temp_sensing_vol(&voltage);
                    temper.temp_sense(&temp);
                    xbee.printf("Volt = %f,Temp = %f\r\n",voltage,temp);
                    wait(0.5);
                    timer = tt.read();
                    fprintf(fp,"%f,%f,%f\r\n",timer,voltage,temp);
                }
                tt.stop();
                tt.reset();
                fclose(fp);
            }

            if(rcmd == 'b') {
                FILE *fp = fopen("/sd/mydir/test_thermal_40_deg.csv", "w");
                for(int i = 0; i<120; i++) {
                    tt.start();
                    temper.temp_sensing_vol(&voltage);
                    temper.temp_sense(&temp);
                    xbee.printf("Volt = %f,Temp = %f\r\n",voltage,temp);
                    wait(0.5);
                    timer = tt.read();
                    fprintf(fp,"%f,%f,%f\r\n",timer,voltage,temp);
                }
                tt.stop();
                tt.reset();
                fclose(fp);
            }

            if(rcmd == 'c') {
                pc.printf("SD test Mode\r\n");
                FILE *fp = fopen("/sd/mydir/myfile.txt", "w");
                if(fp == NULL) {
                    xbee.printf("Could not open file for write\r\n");
                } else {
                    fprintf(fp, "\n\rHello World!\n\r");
                    xbee.printf("SD Check Complete!!\r\n");
                    fclose(fp);
                }
            }

            if(rcmd == 'd') {
                ///////////////////////////
                //For user's mission




                ///////////////////////////
            }
            xbee.initialize();
        }//cmdflag = 1
        wait(1.0);
    }//while
}//main