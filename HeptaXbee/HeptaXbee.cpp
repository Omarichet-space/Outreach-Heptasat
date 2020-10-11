#include "HeptaXbee.h"
#include "mbed.h"
#include  <stdarg.h>

HeptaXbee::HeptaXbee(PinName tx, PinName rx) : xbee(tx,rx)
{
    rcmd = 0;
    cmdflag = 0;
}
void HeptaXbee::baud(int rate)
{
    xbee.baud(rate);
}
void HeptaXbee::xbee_receive(int *xrcmd, int *xcmdflag)
{
    xbee.attach(this,&HeptaXbee::commandget,Serial::RxIrq);
    //xbee.printf("ssXbeedata= %d,%d",HeptaXbee::rcmd,HeptaXbee::cmdflag);
    *xrcmd = HeptaXbee::rcmd;
    *xcmdflag = HeptaXbee::cmdflag;
}

void HeptaXbee::initialize()
{
    HeptaXbee::rcmd = 0;
    HeptaXbee::cmdflag = 0;
}

void HeptaXbee::commandget()
{
    HeptaXbee::rcmd=xbee.getc();
    HeptaXbee::cmdflag = 1;
    //xbee.printf("Xbeedata= %d,%d",rcmd,cmdflag);
}

//*gyro×3 = A,B,C *Accel×3 = D,E,F *magnetic×3 = G,H,I *latitude = J *longitude = K *altitude = L *battery = M *temperature = N 
void HeptaXbee::xbee_transmit(char dataA[],char dataB[],char dataC[],char dataD[],char dataE[],
                              char dataF[],char dataG[],char dataH[],char dataI[],char dataJ[],
                              char dataK[],char dataL[],char dataM[],char dataN[])
{
    char OutputData[255];
    int output_num,numA,numB,numC,numD,numE,numF,numG,numH,numI,numJ,numK,numL,numM,numN;
    int sum = 0;
    int check;
    numA = sizeof(dataA) / sizeof(dataA[0]);
    numB = sizeof(dataB) / sizeof(dataB[0]);
    numC = sizeof(dataC) / sizeof(dataC[0]);
    numD = sizeof(dataD) / sizeof(dataD[0]);
    numE = sizeof(dataE) / sizeof(dataE[0]);
    numF = sizeof(dataF) / sizeof(dataF[0]);
    numG = sizeof(dataG) / sizeof(dataG[0]);
    numH = sizeof(dataH) / sizeof(dataH[0]);
    numI = sizeof(dataI) / sizeof(dataI[0]);
    numJ = sizeof(dataJ) / sizeof(dataJ[0])*2;
    numK = sizeof(dataK) / sizeof(dataK[0])*2;
    numL = sizeof(dataL) / sizeof(dataL[0]);
    numM = sizeof(dataM) / sizeof(dataM[0]);
    numN = sizeof(dataN) / sizeof(dataN[0]);
    output_num=numA+numB+numC+numD+numE+numF+numG+numH+numI+numJ+numK+numL+numM+numN;
    int N=0,i=0,ii;
    for(i = 0; i <= numA-1; i++) {
        OutputData[N+i] = dataA[i];
    }
    N=i;
    for(i = 0; i <= numB-1; i++) {
        OutputData[N+i] = dataB[i];
    }
    N=N+i;
    for(i = 0; i <= numC-1; i++) {
        OutputData[N+i] = dataC[i];
    }
    N=N+i;
    for(i = 0; i <= numD-1; i++) {
        OutputData[N+i] = dataD[i];
    }
    N=N+i;
    for(i = 0; i <= numE-1; i++) {
        OutputData[N+i] = dataE[i];
    }
    N=N+i;
    for(i = 0; i <= numF-1; i++) {
        OutputData[N+i] = dataF[i];
    }
    N=N+i;
    for(i = 0; i <= numG-1; i++) {
        OutputData[N+i] = dataG[i];
    }
    N=N+i;
    for(i = 0; i <= numH-1; i++) {
        OutputData[N+i] = dataH[i];
    }
    N=N+i;
    for(i = 0; i <= numI-1; i++) {
        OutputData[N+i] = dataI[i];
    }
    N=N+i;
    for(i = 0; i <= numJ-1; i++) {
        OutputData[N+i] = dataJ[i];
    }
    N=N+i;
    for(i = 0; i <= numK-1; i++) {
        OutputData[N+i] = dataK[i];
    }
    N=N+i;
    for(i = 0; i <= numL-1; i++) {
        OutputData[N+i] = dataL[i];
    }
    N=N+i;
    for(i = 0; i <= numM-1; i++) {
        OutputData[N+i] = dataM[i];
    }
    N=N+i;
    for(i = 0; i <= numN-1; i++) {
        OutputData[N+i] = dataN[i];
    }
    //check sum
    for(ii=0; ii<output_num; ii++) {
        sum = sum + OutputData[ii];
    }
    check = (~sum)&0xFF;
    char check_sum[2],flame[2];
    sprintf( check_sum, "%02X", check);
    sprintf( flame, "%02X", output_num);
    // transmit data
    //for(int i = 0;i<2;i++){
      //  xbee.putc(flame[i]);//frame length
        //}
    for(ii=0; ii<output_num; ii++) {
        xbee.putc(OutputData[ii]);//data
    }
    for(int i = 0;i<2;i++){
        xbee.putc(check_sum[i]);
        }
    xbee.putc(0x0a);//endcode
}
void HeptaXbee::puts( char *s )
{
    while ( char c    = *s++ )
        xbee.putc( c );
}
void HeptaXbee::printf( char *format, ... )
{
    char        s[ 100 ];
    va_list        args;

    va_start( args, format );
    vsnprintf( s, 100, format, args );
    va_end( args );

    xbee.puts( s );
}
char HeptaXbee::getc()
{
    char character;
    character = xbee.getc();
    return character;
}

void HeptaXbee::putc(char data)
{
    xbee.putc(data);
}
