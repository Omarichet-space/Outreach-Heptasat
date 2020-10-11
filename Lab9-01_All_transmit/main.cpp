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
HeptaBattery bat(p16,p26);

int main()
{
    int rcmd=0,cmdflag=0;
    while(1) {
        wait(1.0);
        xbee.xbee_recieve(&rcmd,&cmdflag);
        pc.printf("rcmd=%d, cmdflag=%d\r\n",rcmd, cmdflag);
        if (cmdflag == 1) {
            pc.printf("Command Get %d\r\n",rcmd);
            pc.printf("HEPTA Uplink OK\r\n");
            if(rcmd == 'h') {
                cam_gps.gps_setting();
                char gx[4],gy[4],gz[4],ax[4],ay[4],az[4],mx[4],my[4],mz[4],lad[8],log[8],height[4],bt[4],temper[4];
                char ddata[64];
                int dsize[7];
                for(int i = 0;i < 50;i++) {
                    n_axis.sen_gyro_u16(gx,gy,gz,&dsize[0]);
                    n_axis.sen_acc_u16(ax,ay,az,&dsize[1]);
                    n_axis.sen_mag_u16(mx,my,mz,&dsize[2]);
                    bat.vol_u16(bt,&dsize[5]);
                    temp.temp_sense_u16(temper,&dsize[6]);
                    cam_gps.lat_log_sensing_u16(lad,log,height,&dsize[3],&dsize[4]);
                    xbee.putc('H');
                    xbee.xbee_transmit(ddata,64,gx,gy,gz,ax,ay,az,mx,my,mz,lad,log,height,bt,temper,dsize[0],dsize[0],dsize[0],dsize[1],dsize[1],dsize[1],dsize[2],dsize[2],dsize[2],dsize[3],dsize[3],dsize[4],dsize[5],dsize[6],14);
                }
            }
            xbee.initialize();
        }
    }
}