#include "mbed.h"
#include "SDFileSystem.h"

Serial pc(USBTX,USBRX);
SDFileSystem sd(p5, p6, p7, p8, "sd");

int main()
{
    pc.baud(9600);
    pc.printf("Hello world!\r\n");
    char str[100];
    mkdir("/sd/mydir", 0777);
    FILE *fp = fopen("/sd/mydir/test.txt","w");
    if(fp == NULL) {
        error("Could not open file for write\r\n");
    }
    for(int i=0; i<10; i++)fprintf(fp,"Hello my name is HEPTA!\r\n");
    fclose(fp);
    fp = fopen("/sd/mydir/test.txt","r");
    for(int j = 0; j < 10; j++) {
        fgets(str,100,fp);
        puts(str);
    }
    fclose(fp);
    pc.printf("Goodbye!!\r\n");
}