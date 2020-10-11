#include "mbed.h"

Serial pc(USBTX,USBRX);

DigitalOut myleds[] = {LED1,LED2,LED3,LED4};;

int rcmd = 0,cmdflag = 0;

void commandget()
{
    rcmd=pc.getc();
    cmdflag = 1;
}
void receive(int *xrcmd, int *xcmdflag)
{
    pc.attach(commandget,Serial::RxIrq);
    *xrcmd = rcmd;
    *xcmdflag = cmdflag;
}

void initialize()
{
    rcmd = 0;
    cmdflag = 0;
}

int main()
{
    pc.baud(9600);
    receive(&rcmd,&cmdflag);
    while(1) {
        myleds[0] = 1;
        wait(0.5);
        myleds[0] = 0;
        wait(0.5);
        if (cmdflag == 1) {
            if (rcmd == 'a') {
               pc.printf("Command Get = %d\r\n",rcmd);
               for(int i = 0;i < 10;i++){
               pc.printf("Count = %d\r\n",i);
               wait(1.0);
               }
            }
            initialize();
        }
    }
}