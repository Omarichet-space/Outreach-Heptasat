#include "mbed.h"
#include "HeptaBattery.h"
I2C i2c(p28, p27);
Serial pc(USBTX, USBRX);
HeptaBattery battery(p16,p26);
const int addr_accel_gyro = 0xD0;
char cmd[2];
const int addr_compus = 0x18;
short int mxl,mxh,myl,myh,mzl,mzh,st2;
int main()
{
    i2c.frequency(100000);
    cmd[0]=0x6B;
    cmd[1]=0x00;
    i2c.write(addr_accel_gyro,cmd,2);
    cmd[0] = 0x37;
    cmd[1] = 0x02;
    i2c.write(addr_accel_gyro,cmd,2);
    i2c.stop();
    while(1) {
        i2c.start();
        i2c.write(addr_compus);
        i2c.write(0x0A);
        i2c.write(0x12);
        i2c.stop();
        i2c.start();
        i2c.write(addr_compus);
        i2c.write(0x03);
        i2c.start();
        i2c.write(addr_compus|0x01);
        mxl = i2c.read(0);
        i2c.stop();
        i2c.start();
        i2c.write(addr_compus);
        i2c.write(0x04);
        i2c.start();
        i2c.write(addr_compus|0x01);
        mxh = i2c.read(0);
        i2c.stop();
        i2c.start();
        i2c.write(addr_compus);
        i2c.write(0x09);
        i2c.start();
        i2c.write(addr_compus|0x01);
        st2 = i2c.read(0);
        i2c.stop();
        double  mg_x = short((mxh<<8) | (mxl));
        double MX = ( mg_x)*0.15;
        pc.printf("MX = %f\r\n",MX);
        wait(0.5);
    }
}