#include"Hepta9axis.h"
#include"mbed.h"

Hepta9axis::Hepta9axis(PinName sda, PinName scl, int aaddr, int agaddr) : n_axis(sda,scl),addr_accel_gyro(aaddr),addr_compus(agaddr)
{
    n_axis.frequency(100000);
    cmd[0]=0x6B;
    cmd[1]=0x00;
    n_axis.write(addr_accel_gyro,cmd,2);
    cmd[0] = 0x37;
    cmd[1] = 0x02;
    n_axis.write(addr_accel_gyro,cmd,2);
    n_axis.stop();
}

void Hepta9axis::setup()
{

    n_axis.frequency(100000);
    cmd[0]=0x6B;
    cmd[1]=0x00;
    n_axis.write(addr_accel_gyro,cmd,2);
    cmd[0] = 0x37;
    cmd[1] = 0x02;
    n_axis.write(addr_accel_gyro,cmd,2);
    n_axis.stop();
}

void Hepta9axis::sen_acc(float *ax,float *ay,float *az)
{
//x軸加速度
    n_axis.start();
    n_axis.write(addr_accel_gyro);
    n_axis.write(0x3B);
    n_axis.start();
    n_axis.write(addr_accel_gyro|0x01);
    xh = n_axis.read(0);
    n_axis.stop();

    n_axis.start();
    n_axis.write(addr_accel_gyro);
    n_axis.write(0x3C);
    n_axis.start();
    n_axis.write(addr_accel_gyro|0x01);
    xl = n_axis.read(0);
    n_axis.stop();

    double  acc_ax = short((xh<<8) | (xl));
    *ax = (acc_ax)*2/32764*9.81;


//Y軸加速度
    n_axis.start();
    n_axis.write(addr_accel_gyro);
    n_axis.write(0x3D);
    n_axis.start();
    n_axis.write(addr_accel_gyro|0x01);
    yh = n_axis.read(0);

    n_axis.stop();
    n_axis.start();
    n_axis.write(addr_accel_gyro);
    n_axis.write(0x3E);
    n_axis.start();
    n_axis.write(addr_accel_gyro|0x01);
    yl = n_axis.read(0);

    n_axis.stop();
    double  acc_ay = short((yh<<8) | (yl));
    *ay = (acc_ay)*2/32764*9.81;


//z軸加速度
    n_axis.start();
    n_axis.write(addr_accel_gyro);
    n_axis.write(0x3F);
    n_axis.start();
    n_axis.write(addr_accel_gyro|0x01);
    zh = n_axis.read(0);
    n_axis.stop();

    n_axis.start();
    n_axis.write(addr_accel_gyro);
    n_axis.write(0x40);
    n_axis.start();
    n_axis.write(addr_accel_gyro|0x01);
    zl = n_axis.read(0);
    n_axis.stop();

    double  acc_az = short((zh<<8) | (zl));
    *az = (acc_az)*2/32764*9.81;

}

void Hepta9axis::sen_gyro(float *gx,float *gy,float *gz)
{
//x軸GYRO
    n_axis.start();
    n_axis.write(addr_accel_gyro);
    n_axis.write(0x43);
    n_axis.start();
    n_axis.write(addr_accel_gyro|0x01);
    gxh = n_axis.read(0);
    n_axis.stop();

    n_axis.start();
    n_axis.write(addr_accel_gyro);
    n_axis.write(0x44);
    n_axis.start();
    n_axis.write(addr_accel_gyro|0x01);
    gxl = n_axis.read(0);
    n_axis.stop();

    double  gyro_ax = short((gxh<<8) | (gxl));
    *gx = (gyro_ax)*0.00763;

//y軸GYRO
    n_axis.start();
    n_axis.write(addr_accel_gyro);
    n_axis.write(0x45);
    n_axis.start();
    n_axis.write(addr_accel_gyro|0x01);
    gyh = n_axis.read(0);
    n_axis.stop();

    n_axis.start();
    n_axis.write(addr_accel_gyro);
    n_axis.write(0x46);
    n_axis.start();
    n_axis.write(addr_accel_gyro|0x01);
    gyl = n_axis.read(0);
    n_axis.stop();

    double  gyro_ay = short((gyh<<8) | (gyl));
    *gy = (gyro_ay)*0.00763;

//z軸GYRO
    n_axis.start();
    n_axis.write(addr_accel_gyro);
    n_axis.write(0x47);
    n_axis.start();
    n_axis.write(addr_accel_gyro|0x01);
    gzh = n_axis.read(0);
    n_axis.stop();

    n_axis.start();
    n_axis.write(addr_accel_gyro);
    n_axis.write(0x48);
    n_axis.start();
    n_axis.write(addr_accel_gyro|0x01);
    gzl = n_axis.read(0);
    n_axis.stop();

    double  gyro_az = short((gzh<<8) | (gzl));
    *gz = (gyro_az)*0.00763;

}

void Hepta9axis::sen_mag(float *mx,float *my,float *mz)
{

    n_axis.start();
    n_axis.write(addr_compus);
    n_axis.write(0x0A);
    n_axis.write(0x12);
    n_axis.stop();

    n_axis.start();
    n_axis.write(addr_compus);
    n_axis.write(0x03);
    n_axis.start();
    n_axis.write(addr_compus|0x01);
    mxl = n_axis.read(0);
    n_axis.stop();
    n_axis.start();
    n_axis.write(addr_compus);
    n_axis.write(0x04);
    n_axis.start();
    n_axis.write(addr_compus|0x01);
    mxh = n_axis.read(0);
    n_axis.stop();
    
    double  mg_x = short((mxh<<8) | (mxl));
    *mx = ( mg_x)*0.15;

    n_axis.start();

    n_axis.write(addr_compus);
    n_axis.write(0x05);
    n_axis.start();
    n_axis.write(addr_compus|0x01);
    myl = n_axis.read(0);
    n_axis.stop();
    n_axis.start();
    n_axis.write(addr_compus);
    n_axis.write(0x06);
    n_axis.start();
    n_axis.write(addr_compus|0x01);
    myh = n_axis.read(0);
    n_axis.stop();

    double  mg_y = short((myh<<8) | (myl));
    *my = ( mg_y)*0.15;

    n_axis.start();

    n_axis.write(addr_compus);
    n_axis.write(0x07);
    n_axis.start();
    n_axis.write(addr_compus|0x01);
    mzl = n_axis.read(0);
    n_axis.stop();
    n_axis.start();
    n_axis.write(addr_compus);
    n_axis.write(0x08);
    n_axis.start();
    n_axis.write(addr_compus|0x01);
    mzh = n_axis.read(0);
    n_axis.stop();

    n_axis.start();
    n_axis.write(addr_compus);
    n_axis.write(0x09);
    n_axis.start();
    n_axis.write(addr_compus|0x01);
    st2 = n_axis.read(0);
    n_axis.stop();
    double  mg_z= short((mzh<<8) | (mzl));
    *mz = ( mg_z)*0.15;
   
}

//////////////16進数表記/////////////////////////////////////////
void Hepta9axis::sen_gyro_u16(char* gx_u16,char* gy_u16,char* gz_u16)
{
    //x軸GYRO
    n_axis.start();
    n_axis.write(addr_accel_gyro);
    n_axis.write(0x43);
    n_axis.start();
    n_axis.write(addr_accel_gyro|0x01);
    gxh = n_axis.read(0);
    n_axis.stop();

    n_axis.start();
    n_axis.write(addr_accel_gyro);
    n_axis.write(0x44);
    n_axis.start();
    n_axis.write(addr_accel_gyro|0x01);
    gxl = n_axis.read(0);
    n_axis.stop();

    sprintf( g1, "%02X", ((gxh)) & 0xFF);
    sprintf( g2, "%02X", ((gxl)) & 0xFF);
    gx_u16[0]=g1[0];
    gx_u16[1]=g1[1];
    gx_u16[2]=g2[0];
    gx_u16[3]=g2[1];

//y軸GYRO
    n_axis.start();
    n_axis.write(addr_accel_gyro);
    n_axis.write(0x45);
    n_axis.start();
    n_axis.write(addr_accel_gyro|0x01);
    gyh = n_axis.read(0);
    n_axis.stop();

    n_axis.start();
    n_axis.write(addr_accel_gyro);
    n_axis.write(0x46);
    n_axis.start();
    n_axis.write(addr_accel_gyro|0x01);
    gyl = n_axis.read(0);
    n_axis.stop();
    sprintf( g1, "%02X", (gyh) & 0xFF);
    sprintf( g2, "%02X", (gyl) & 0xFF);
    gy_u16[0]=g1[0];
    gy_u16[1]=g1[1];
    gy_u16[2]=g2[0];
    gy_u16[3]=g2[1];

//z軸GYRO
    n_axis.start();
    n_axis.write(addr_accel_gyro);
    n_axis.write(0x47);
    n_axis.start();
    n_axis.write(addr_accel_gyro|0x01);
    gzh = n_axis.read(0);
    n_axis.stop();

    n_axis.start();
    n_axis.write(addr_accel_gyro);
    n_axis.write(0x48);
    n_axis.start();
    n_axis.write(addr_accel_gyro|0x01);
    gzl = n_axis.read(0);
    n_axis.stop();

    sprintf( g1, "%02X", ((gzh)) & 0xFF);
    sprintf( g2, "%02X", ((gzl)) & 0xFF);
    gz_u16[0]=g1[0];
    gz_u16[1]=g1[1];
    gz_u16[2]=g2[0];
    gz_u16[3]=g2[1];
    //*dsize = 4;
}

void Hepta9axis::sen_acc_u16(char* ax_u16,char* ay_u16,char* az_u16)
{
    //x軸加速度
    n_axis.start();
    n_axis.write(addr_accel_gyro);
    n_axis.write(0x3B);
    n_axis.start();
    n_axis.write(addr_accel_gyro|0x01);
    xh = n_axis.read(0);
    n_axis.stop();

    n_axis.start();
    n_axis.write(addr_accel_gyro);
    n_axis.write(0x3C);
    n_axis.start();
    n_axis.write(addr_accel_gyro|0x01);
    xl = n_axis.read(0);
    n_axis.stop();

    sprintf( a1, "%02X", ((xh)) & 0xFF);
    sprintf( a2, "%02X", ((xl)) & 0xFF);
    ax_u16[0]=a1[0];
    ax_u16[1]=a1[1];
    ax_u16[2]=a2[0];
    ax_u16[3]=a2[1];

//Y軸加速度
    n_axis.start();
    n_axis.write(addr_accel_gyro);
    n_axis.write(0x3D);
    n_axis.start();
    n_axis.write(addr_accel_gyro|0x01);
    yh = n_axis.read(0);

    n_axis.stop();
    n_axis.start();
    n_axis.write(addr_accel_gyro);
    n_axis.write(0x3E);
    n_axis.start();
    n_axis.write(addr_accel_gyro|0x01);
    yl = n_axis.read(0);
    n_axis.stop();

    sprintf( a1, "%02X", ((yh)) & 0xFF);
    sprintf( a2, "%02X", ((yl)) & 0xFF);
    ay_u16[0]=a1[0];
    ay_u16[1]=a1[1];
    ay_u16[2]=a2[0];
    ay_u16[3]=a2[1];
//z軸加速度
    n_axis.start();
    n_axis.write(addr_accel_gyro);
    n_axis.write(0x3F);
    n_axis.start();
    n_axis.write(addr_accel_gyro|0x01);
    zh = n_axis.read(0);
    n_axis.stop();

    n_axis.start();
    n_axis.write(addr_accel_gyro);
    n_axis.write(0x40);
    n_axis.start();
    n_axis.write(addr_accel_gyro|0x01);
    zl = n_axis.read(0);
    n_axis.stop();

    sprintf( a1, "%02X", ((zh)) & 0xFF);
    sprintf( a2, "%02X", ((zl)) & 0xFF);
    az_u16[0]=a1[0];
    az_u16[1]=a1[1];
    az_u16[2]=a2[0];
    az_u16[3]=a2[1];
    //*dsize = 4;
}

void Hepta9axis::sen_mag_u16(char* mx_u16,char* my_u16,char* mz_u16)
{
    n_axis.start();
    n_axis.write(addr_compus);
    n_axis.write(0x0a);
    n_axis.write(0x12);
    n_axis.stop();

    n_axis.start();
    n_axis.write(addr_compus);
    n_axis.write(0x03);
    n_axis.start();
    n_axis.write(addr_compus|0x01);
    mxl = n_axis.read(0);
    n_axis.stop();
    n_axis.start();
    n_axis.write(addr_compus);
    n_axis.write(0x04);
    n_axis.start();
    n_axis.write(addr_compus|0x01);
    mxh = n_axis.read(0);
    n_axis.stop();

    sprintf( m1, "%02X", ((mxh)) & 0xFF);
    sprintf( m2, "%02X", ((mxl)) & 0xFF);
    mx_u16[0]=m1[0];
    mx_u16[1]=m1[1];
    mx_u16[2]=m2[0];
    mx_u16[3]=m2[1];

    n_axis.start();

    n_axis.write(addr_compus);
    n_axis.write(0x05);
    n_axis.start();
    n_axis.write(addr_compus|0x01);
    myl = n_axis.read(0);
    n_axis.stop();
    n_axis.start();
    n_axis.write(addr_compus);
    n_axis.write(0x06);
    n_axis.start();
    n_axis.write(addr_compus|0x01);
    myh = n_axis.read(0);
    n_axis.stop();

    sprintf( m1, "%02X", ((myh)) & 0xFF);
    sprintf( m2, "%02X", ((myl)) & 0xFF);
    my_u16[0]=m1[0];
    my_u16[1]=m1[1];
    my_u16[2]=m2[0];
    my_u16[3]=m2[1];

    n_axis.start();

    n_axis.write(addr_compus);
    n_axis.write(0x07);
    n_axis.start();
    n_axis.write(addr_compus|0x01);
    mzl = n_axis.read(0);
    n_axis.stop();
    n_axis.start();
    n_axis.write(addr_compus);
    n_axis.write(0x08);
    n_axis.start();
    n_axis.write(addr_compus|0x01);
    mzh = n_axis.read(0);
    n_axis.stop();
    
    n_axis.start();
    n_axis.write(addr_compus);
    n_axis.write(0x09);
    n_axis.start();
    n_axis.write(addr_compus|0x01);
    st2 = n_axis.read(0);
    n_axis.stop();

    sprintf( m1, "%02X", ((mzh)) & 0xFF);
    sprintf( m2, "%02X", ((mzl)) & 0xFF);
    mz_u16[0]=m1[0];
    mz_u16[1]=m1[1];
    mz_u16[2]=m2[0];
    mz_u16[3]=m2[1];
    //*dsize = 4;
}