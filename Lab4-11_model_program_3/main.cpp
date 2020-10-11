#include "mbed.h"
#include "SDFileSystem.h"
#include "HeptaBattery.h"
#include "HeptaCamera_GPS.h"
#include "Hepta9axis.h"
#include "HeptaTemp.h"

Serial pc(USBTX,USBRX);
SDFileSystem sd(p5, p6, p7, p8, "sd");
HeptaBattery battery(p16,p26);
HeptaCamera_GPS cam_gps(p13, p14,p25,p24);
Hepta9axis n_axis(p28,p27,0xD0,0x18);
HeptaTemp temp(p17);

DigitalOut myleds[] = {LED1,LED2,LED3,LED4};;

int rcmd = 0,cmdflag = 0;

void commandget()
{
    rcmd=pc.getc();
    cmdflag = 1;
}
void receive(int *xrcmd, int *xcmdflag)
{
    pc.attach(commandget,Serial::RxIrq);
    *xrcmd = rcmd;
    *xcmdflag = cmdflag;
}

void initialize()
{
    rcmd = 0;
    cmdflag = 0;
}

int main()
{
    pc.baud(9600);
    float bt;
    float temper;
    char str[100];
    float ax,ay,az;
    float gx,gy,gz;
    float mx,my,mz;
    int quality=0,stnum=0,gps_check=0;
    char ns='A',ew='B',aunit='m';
    float time=0.0,latitude=0.0,longitude=0.0,hacu=0.0,altitude=0.0;
    int flag = 0;
    receive(&rcmd,&cmdflag);
    while(1) {
        battery.power_saving_mode(&flag,&bt);
        temp.temp_sense(&temper);
        wait(0.5);
        pc.printf("flag = %d, bt = %.2f [V], temp = %.1f [C]\r\n",flag,bt,temper);
        if(flag == 1) {
            pc.printf("Low Battery\r\n");
        } else if(temper > 35.0) {
            pc.printf("High Temperaturer\n");
        } else if((flag == 1) & (temper > 35.0)) {
            pc.printf("Low Battery and High Temperaturer\n");
        }
        if (cmdflag == 1) {
            pc.printf("Command Get = %d\r\n",rcmd);
            if (rcmd == 'a') {
                for(int i = 0; i < 10; i++) {
                    pc.printf("Count = %d\r\n",i);
                    wait(1.0);
                }
            }
            if (rcmd == 'b') {
                mkdir("/sd/mydir", 0777);
                FILE *fp = fopen("/sd/mydir/battery.txt","w");
                if(fp == NULL) {
                    error("Could not open file for write\r\n");
                } else {
                    for(int i = 0; i < 10; i++) {
                        battery.vol(&bt);
                        fprintf(fp,"%f\r\n",bt);
                        wait(1.0);
                    }
                    fclose(fp);
                }
                FILE *fp1 = fopen("/sd/mydir/battery.txt","r");
                if(fp1 == NULL) {
                    error("Could not open file for read\r\n");
                } else {
                    for(int i = 0; i < 10; i++) {
                        fgets(str,100,fp1);
                        puts(str);
                        wait(1.0);
                    }
                    fclose(fp1);
                }
            }//
            if (rcmd == 'c') {
                
                
                
                
            }
            if (rcmd == 'd') {
                
                
                
                
            }
            initialize();
        }
    }
}