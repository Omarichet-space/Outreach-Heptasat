#ifndef MBED_HEPTA9AXIS_H
#define MBED_HEPTA9AXIS_H
#include "mbed.h"

class Hepta9axis
{
public:
    I2C n_axis;
    int addr_accel_gyro;
    int addr_compus;
    Hepta9axis(
        PinName sda,
        PinName scl,
        int aaddr,
        int agaddr
    );
    void setup();
    void sen_acc(float *ax,float *ay,float *az);
    void sen_gyro(float *gx,float *gy,float *gz);
    void sen_mag(float *mx,float *my,float *mz);
    void sen_gyro_u16(char* gx_u16,char* gy_u16,char* gz_u16);
    void sen_acc_u16(char* ax_u16,char* ay_u16,char* az_u16);
    void sen_mag_u16(char* mx_u16,char* my_u16,char* mz_u16);

private:
    char cmd[2];
    char g1[8],g2[8];
    char a1[8],a2[8];
    char m1[8],m2[8];
    short int st2;
    short int xl,xh,yl,yh,zl,zh;
    short int gxl,gxh,gyl,gyh,gzl,gzh;
    short int mxl,mxh,myl,myh,mzl,mzh;

};

#endif