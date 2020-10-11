#include "mbed.h"
#include "HeptaBattery.h"

Serial pc(USBTX,USBRX);
HeptaBattery battery(p16,p26);

int main()
{
    pc.baud(9600);
    float bt_dec;
    char bt_hex[4];
    for(int i = 0; i < 10; i++) {
        battery.vol(&bt_dec);
        battery.vol_u16(bt_hex);
        pc.printf("Vol = %f\r\n",bt_dec);
        pc.printf("Vol_hex = %c%c%c%c\r\n",bt_hex[0],bt_hex[1],bt_hex[2],bt_hex[3]);
        wait(1.0);
    }
}