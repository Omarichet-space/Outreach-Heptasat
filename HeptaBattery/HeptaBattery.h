#ifndef MBED_HEPTABATTERY_H
#define MBED_HEPTABATTERY_H
#include "mbed.h"
#include "PowerControl/PowerControl.h"
#include "PowerControl/EthernetPowerControl.h"

class HeptaBattery{
public:
    HeptaBattery(
            PinName bat,
            PinName bat_ct
    );
    void vol(float* bt);//バッテリー電圧監視
    void shut_down_regulator();//3.3V供給停止
    void turn_on_regulator();//3.3V供給
    void vol_u16(char* bt_u16);//Battery電圧監視(16進数)
    void power_saving_mode(int *flag,float* Bat);//省電力モード
  
private:  
    DigitalOut _bat_ct;
    AnalogIn _bat; 
};
 
#endif