#include "mbed.h"
#include "SDFileSystem.h"
#include "HeptaBattery.h"

Serial pc(USBTX,USBRX);
SDFileSystem sd(p5, p6, p7, p8, "sd");
HeptaBattery battery(p16,p26);

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
    float bt;
    int flag = 0;
    char str[100];
    receive(&rcmd,&cmdflag);
    while(1) {
        battery.vol(&bt);
        if(bt <= 3.5) {
            battery.shut_down_regulator();
            flag = 1;
        } else {
            battery.turn_on_regulator();
            flag = 0;
        }
        pc.printf("Vol = %.2f [V]\r\n",bt);
        if(flag == 1) {
            pc.printf("Power saving mode ON\r\n");
        }
        wait(0.5);
        if (cmdflag == 1) {
            pc.printf("Command Get = %d\r\n",rcmd);
            if (rcmd == 'a') {
                for(int i = 0; i < 10; i++) {
                    pc.printf("Count = %d\r\n",i);
                    wait(1.0);
                }
            }
            if (rcmd == 'b') {
                
                
                
                
                
                
            }
            initialize();
        }
    }
}