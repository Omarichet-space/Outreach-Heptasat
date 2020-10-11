#include "mbed.h"
#include "SDFileSystem.h"
#include "HeptaBattery.h"

Serial pc(USBTX,USBRX);
SDFileSystem sd(p5, p6, p7, p8, "sd");
HeptaBattery battery(p16,p26);

int main()
{
    pc.baud(9600);
    float bt;
    char str[100];
    mkdir("/sd/mydir", 0777);
    FILE *fp = fopen("/sd/mydir/test.txt","w");
    if(fp == NULL) {
        error("Could not open file for write\r\n");
    }
    for(int i = 0; i < 10; i++) {
        battery.vol(&bt);
        fprintf(fp,"%f\r\n",bt);
        wait(1.0);
    }
    fclose(fp);
    fp = fopen("/sd/mydir/test.txt","r");
    for(int j = 0; j < 10; j++) {
        fgets(str,100,fp);
        puts(str);
    }
    fclose(fp);
    pc.printf("Goodbye!!\r\n");
}