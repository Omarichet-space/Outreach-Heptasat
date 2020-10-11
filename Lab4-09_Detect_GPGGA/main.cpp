#include "mbed.h"
#include "HeptaBattery.h"
#include "SDFileSystem.h"
#include "HeptaCamera_GPS.h"

Serial pc(USBTX,USBRX);
SDFileSystem sd(p5, p6, p7, p8, "sd");
HeptaCamera_GPS cam_gps(p13, p14,p25,p24);//(tx,rx,camera switch,gps switch)
HeptaBattery battery(p16,p26);
int main()
{
    pc.baud(9600);
    cam_gps.gps_setting();
    int quality=0,stnum=0,gps_check=0;
    char ns='A',ew='B',aunit='m';
    float time=0.0,latitude=0.0,longitude=0.0,hacu=0.0,altitude=0.0;
    for(int i=1; i<10; i++) {
        cam_gps.gga_sensing(&time, &latitude, &ns, &longitude, &ew, &quality, &stnum, &hacu, &altitude, &aunit, &gps_check);
        if((gps_check==0)|(gps_check==1)) {
            pc.printf("GPGGA,%f,%f,%c,%f,%c,%d,%d,%f,%f,%c\r\n",time,latitude,ns,longitude,ew,quality,stnum,hacu,altitude,aunit);
        }
    }
}