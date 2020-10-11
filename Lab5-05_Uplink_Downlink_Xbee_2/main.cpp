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

int main()
{
    int rcmd=0,cmdflag=0;
    while(1) {
        wait(1.0);
        xbee.xbee_receive(&rcmd,&cmdflag);
        pc.printf("rcmd=%d, cmdflag=%d\r\n",rcmd, cmdflag);
        if (cmdflag == 1) {
            pc.printf("Command Get %d\r\n",rcmd);
            if(rcmd == 'h') {
                cam_gps.gps_setting();
                char gx[4],gy[4],gz[4],ax[4],ay[4],az[4],mx[4],my[4],mz[4],lad[8],log[8],height[4],bt[4],temper[4];
                float gx1,gy1,gz1;
                for(int i = 0; i < 1000000; i++) {
                    n_axis.sen_gyro_u16(gx,gy,gz);
                    n_axis.sen_gyro(&gx1,&gy1,&gz1);
                    n_axis.sen_acc_u16(ax,ay,az);
                    n_axis.sen_mag_u16(mx,my,mz);
                    battery.vol_u16(bt);
                    temp.temp_sense_u16(temper);
                    cam_gps.lat_log_sensing_u16(lad,log,height);
                    xbee.putc('H');
                    pc.printf("gx=%f,gx16=%c%c%c%c\r\n",gx1,gx[0],gx[1],gx[2],gx[3]);
                    xbee.xbee_transmit(gx,gy,gz,ax,ay,az,mx,my,mz,lad,log,height,bt,temper);
                    
                }
                xbee.initialize();
            }
        }
    }
}