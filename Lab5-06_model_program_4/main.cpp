#include "mbed.h"
#include "SDFileSystem.h"
#include "HeptaXbee.h"
#include "HeptaCamera_GPS.h"
#include "Hepta9axis.h"
#include "HeptaTemp.h"
#include "HeptaBattery.h"

Serial pc(USBTX,USBRX);
SDFileSystem sd(p5, p6, p7, p8, "sd");
HeptaXbee xbee(p9,p10);
HeptaCamera_GPS cam_gps(p13, p14,p25,p24);
Hepta9axis n_axis(p28,p27,0xD0,0x18);
HeptaTemp temp(p17);
HeptaBattery battery(p16,p26);

DigitalOut myleds[] = {LED1,LED2,LED3,LED4};;

int main()
{
    pc.baud(9600);
    float bt;
    float temper;
    const char* warning;
    char str[100];
    float gx,gy,gz,mx,my,mz;
    
    int quality=0,stnum=0,gps_check=0;
    char ns='A',ew='B',aunit='m';
    float time=0.0,latitude=0.0,longitude=0.0,hacu=0.0,altitude=0.0;
    
    int flag = 0;
    int rcmd=0,cmdflag=0;
    FILE *dummy = fopen("/sd/MissionData/dum.txt","w");
    fclose(dummy);
    while(1) {
        battery.power_saving_mode(&flag,&bt);
        temp.temp_sense(&temper);
        wait(0.5);
        xbee.printf("flag = %d, bt = %.2f [V], temp = %.1f [C]\r\n",flag,bt,temper);
        xbee.xbee_receive(&rcmd,&cmdflag);
        
        if(flag == 1) {
            warning = "Low Battery\r\n";
            xbee.printf("%s",warning);
        } else if(temper > 35.0) {
            warning = "High Temperature\r\n";
            xbee.printf("%s",warning);
        } else if((flag == 1) & (temper > 35.0)) {
            warning = "Low Battery and High Temperature\r\n";
            xbee.printf("%s",warning);
        }else{}
        
        if (cmdflag == 1) {
            xbee.printf("Command Get = %d\r\n",rcmd);
            
            if (rcmd == 'a') {
                mkdir("/sd/MissionData", 0777);
                FILE *fp = fopen("/sd/MissionData/GyroMag.txt","w");
                if(fp == NULL) {
                    error("Could not open file for write\r\n");
                } else {
                    for(int i = 0; i < 10; i++) {
                        n_axis.sen_gyro(&gx,&gy,&gz);
                        n_axis.sen_mag(&mx,&my,&mz);
                        fprintf(fp,"Wx = %f, Wy = %f, Wz = %f, mx = %f, my = %f, mz = %f\r\n",gx,gy,gz,mx,my,mz);
                        wait(1.0);
                    }
                    fclose(fp);
                }
                FILE *fp1 = fopen("/sd/MissionData/GyroMag.txt","r");
                if(fp1 == NULL) {
                    error("Could not open file for read\r\n");
                } else {
                    for(int i = 0; i < 10; i++) {
                        fgets(str,100,fp1);
                        xbee.printf("%s",str);
                        wait(1.0);
                    }
                    fclose(fp1);
                }
                
            }
            if (rcmd == 'b') {
                mkdir("/sd/mydir", 0777);
                FILE *fp = fopen("/sd/MissionData/Position.txt","w");
                if(fp == NULL) {
                    error("Could not open file for write\r\n");
                } else {
                    cam_gps.gps_setting();
                    for(int i = 0; i < 10; i++) {
                        cam_gps.gga_sensing(&time, &latitude, &ns, &longitude, &ew, &quality, &stnum, &hacu, &altitude, &aunit, &gps_check);
                        fprintf(fp,"time: %f, lat: %f,%c, long: %f,%c, altitude:%f\r\n",time,latitude,ns,longitude,ew,altitude);
                        wait(1.0);
                    }
                    fclose(fp);
                }
                
                FILE *fp1 = fopen("/sd/MissionData/Position.txt","r");
                if(fp1 == NULL) {
                    error("Could not open file for read\r\n");
                } else {
                    for(int i = 0; i < 10; i++) {
                        fgets(str,100,fp1);
                        xbee.printf("%s",str);
                        wait(1.0);
                    }
                    fclose(fp1);
                }
            }//b
            
            if (rcmd == 'c') {
                cam_gps.Sync();
                cam_gps.initialize(HeptaCamera_GPS::Baud115200, HeptaCamera_GPS::JpegResolution320x240);
                cam_gps.test_jpeg_snapshot_picture("/sd/MissionData/picture.jpg");
                
            }//c
            xbee.initialize();
        }
    }
}
