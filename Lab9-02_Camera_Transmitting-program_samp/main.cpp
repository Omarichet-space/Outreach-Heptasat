#include "mbed.h"
#include "SDFileSystem.h"
#include "HeptaCamera_GPS.h"

Serial pc(USBTX,USBRX);
SDFileSystem sd(p5, p6, p7, p8, "sd");
HeptaCamera_GPS cam_gps(p13, p14,p25,p24);//(tx,rx,camera switch,gps switch)

int main()
{
    pc.printf("Camera Snapshot Mode\r\n");
    pc.printf("Hit Any Key To Take Picture\r\n");
    while(!pc.readable()) {}
    cam_gps.Sync();
    cam_gps.initialize(HeptaCamera_GPS::Baud115200, HeptaCamera_GPS::JpegResolution320x240);
    cam_gps.test_jpeg_snapshot_data("/sd/test.txt");
    FILE *fp = fopen("/sd/test.txt", "r");
    if(fp == NULL) {
        pc.printf("Could not open file for write\r\n");
    } else {
        char str[1024];
        while((fgets(str,256,fp))!=NULL) {
            pc.printf("%s",str);
            //xbee.printf("%s",str);
            wait(0.001);
        }
        pc.printf("Data transmitting finished\r\n");
        fclose(fp);
    }
}