#include "mbed.h"

Serial pc(USBTX,USBRX);
Serial sig(p13,p14);

int main() {
    char c;
    while(1) {
        c = pc.getc();
        pc.putc(c);
        sig.putc(c);
    }
}