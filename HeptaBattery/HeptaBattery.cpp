#include "HeptaBattery.h"
#include "mbed.h"

HeptaBattery::HeptaBattery(PinName bat, PinName bat_ct) : _bat(bat),_bat_ct(bat_ct)
{
    _bat_ct = 1;
    PHY_PowerDown();//stop Ethernet PHYchip
}

void HeptaBattery::vol(float* bt)
{
    *bt = (_bat.read()*1.431*3.3);//read the battery voltage
}

void HeptaBattery::shut_down_regulator()
{
    _bat_ct = 0;
}

void HeptaBattery::turn_on_regulator()
{
    _bat_ct = 1;
}

void HeptaBattery::vol_u16(char* bt_u16)
{
    unsigned short bt_datas;
    char bt1[8]= {0x00},bt2[8]= {0x00};
    bt_datas=_bat.read_u16()>>4;
    sprintf( bt1, "%02X", (bt_datas >> 8) & 0x0F);
    sprintf( bt2, "%02X", (bt_datas) & 0xFF);
    bt_u16[0]=bt1[0];
    bt_u16[1]=bt1[1];
    bt_u16[2]=bt2[0];
    bt_u16[3]=bt2[1];
    //*dsize = 4;
}

void HeptaBattery::power_saving_mode(int *flag,float* Bat)
{
    *Bat = _bat.read()*1.431*3.3;
    //Above 4.2 or more
    if(*Bat>=4.2) {
        _bat_ct = 1;
       // LPC_SC->CCLKCFG = 2;
        //SystemCoreClockUpdate();
        *flag = 0;
    }
    //3.7 < Voltage < 4.1
    else if(*Bat<=4.1 && *Bat>=3.7) {
        _bat_ct = 1;
        //LPC_SC->CCLKCFG = 2;
        //SystemCoreClockUpdate();
        *flag = 0;
    }
    //3.5V or less
    else if(*Bat<=3.5) {
        _bat_ct = 0;
        //LPC_SC->CCLKCFG = 5;
        //SystemCoreClockUpdate();
        *flag = 1;
    }

}