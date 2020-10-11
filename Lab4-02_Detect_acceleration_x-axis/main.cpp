#include "mbed.h"
#include "HeptaBattery.h"
I2C i2c(p28, p27);
Serial pc(USBTX, USBRX);
HeptaBattery battery(p16,p26);
const int addr_accel_gyro = 0xD0;
char cmd[2];
short int xl,xh,yl,yh,zl,zh;
int main() {
    i2c.frequency(100000);
    cmd[0]=0x6B;
    cmd[1]=0x00;
    i2c.write(addr_accel_gyro,cmd,2);
    cmd[0] = 0x37;
    cmd[1] = 0x02;
    i2c.write(addr_accel_gyro,cmd,2);
    i2c.stop();
       while(1){ 
        i2c.start();
        i2c.write(addr_accel_gyro);
        i2c.write(0x3b);//read adress
        i2c.start();
        i2c.write(addr_accel_gyro|0x01);
        xh = i2c.read(0);
        i2c.stop();
        i2c.start();
        i2c.write(addr_accel_gyro);
        i2c.write(0x3c);
        i2c.start();
        i2c.write(addr_accel_gyro|0x01);
        xl = i2c.read(0);
        i2c.stop();
        double  acc_ax = short((xh<<8) | (xl));
        double AX = (acc_ax)*2/32764*9.81;
        pc.printf("AX = %f\r\n",AX);
        wait(0.5);
       }
}