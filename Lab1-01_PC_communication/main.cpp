#include "mbed.h"

Serial pc(USBTX,USBRX);

int main()
{

    pc.baud(9600);
    int i1 = 7;
    float i2 = 7.5;
    char i3 = 'H';
    pc.printf("Hello World!\r\n");
    pc.printf("i1 = %d\r\n",i1);
    pc.printf("i2 = %f\r\n",i2);
    pc.printf("i3 = %c\r\n",i3);
    pc.putc(i3);

    for(i1 = 0; i1 < 10; i1++) {
        pc.printf("i1 = %d\r\n",i1);
        pc.putc(pc.getc());
    }

}