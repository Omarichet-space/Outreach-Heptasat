#include "mbed.h"
#include "SDFileSystem.h"
#include "HeptaBattery.h"

Serial pc(USBTX,USBRX);



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
       
       
       
    }
    fclose(fp);
    fp = fopen("/sd/mydir/test.txt","r");
    for(int j = 0; j < 10; j++) {
        
        
    }
    fclose(fp);
    pc.printf("Goodbye!!\r\n");
}