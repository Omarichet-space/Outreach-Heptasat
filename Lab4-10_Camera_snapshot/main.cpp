#include "mbed.h"
#include "SDFileSystem.h"
#include "HeptaCamera_GPS.h"
#include "HeptaBattery.h"

Serial pc(USBTX,USBRX);
SDFileSystem sd(p5, p6, p7, p8, "sd");
HeptaCamera_GPS cam_gps(p13, p14,p25,p24);//(tx,rx,camera switch,gps switch)
HeptaBattery battery(p16,p26);

int main()
{
    FILE *dummy = fopen("/sd/dummy.txt","w");
    fclose(dummy);
    
    pc.printf("Camera Snapshot Mode\r\n");
    pc.printf("Hit Any Key To Take Picture\r\n");
    while(!pc.readable()) {}
    cam_gps.Sync();
    cam_gps.initialize(HeptaCamera_GPS::Baud115200, HeptaCamera_GPS::JpegResolution320x240);
    cam_gps.test_jpeg_snapshot_picture("/sd/test.jpg");
}