#include "mbed.h"
#include "HeptaBattery.h"
#include "SDFileSystem.h"
#include "HeptaCamera_GPS.h"

Serial pc(USBTX,USBRX);
HeptaBattery battery(p16,p26);
SDFileSystem sd(p5, p6, p7, p8, "sd");
HeptaCamera_GPS cam_gps(p13, p14,p25,p24);

int main()
{
    cam_gps.gps_setting();
    pc.printf("GPS Raw Data Mode\r\n");
    while(1) pc.putc(cam_gps.getc());
}