#include "mbed.h"
#include "SerialBuffered.h"

#ifndef HEPTA_SERIAL_H
#define HEPTA_SERIAL_H

class HeptaCamera_GPS
{
public:

    enum JpegResolution {
        JpegResolution80x64 = 0x01,   // unofficial
        JpegResolution160x128 = 0x03, // unofficial
        JpegResolution320x240 = 0x05, // QVGA
        JpegResolution640x480 = 0x07  // VGA
    };

    enum ErrorNumber {
        NoError = 0x00,
        UnexpectedReply = 0x04,
        ParameterError = 0x0b,
        SendRegisterTimeout = 0x0c,
        CommandIdError = 0x0d,
        CommandHeaderError = 0xf0,
        SetTransferPackageSizeWrong = 0x11
    };

    enum Baud {
        Baud460800 = 0x02,
        Baud230400 = 0x03,
        Baud115200 = 0x04,
        Baud57600  = 0x05,
        Baud28800  = 0x06,
        Baud14400  = 0x07  // Default.
    };

    enum ResetType {
        Nomal = 0x00,
        High  = 0xff
    };

    HeptaCamera_GPS(PinName tx, PinName rx, PinName CAM, PinName GPS);

    ~HeptaCamera_GPS();

    ErrorNumber sync();
    ErrorNumber init(Baud baud, JpegResolution jr);
    ErrorNumber getJpegSnapshotPicture(FILE*fp);
    ErrorNumber getJpegSnapshotPicture_data(FILE*fp);
    void setmbedBaud(Baud baud);
    void camera_setting(void);
    void Sync(void);
    void initialize(Baud baud,JpegResolution jr);
    void test_jpeg_snapshot_picture(const char *filename);
    void test_jpeg_snapshot_data(const char *filename);
    void jpeg_callback(char *buf, size_t siz);
    void gps_setting(void);
    char getc();
    int  readable();
    void flushSerialBuffer(void);
    void gga_sensing(float *time, float *latitude, char *ns, float *longitude, char *ew, int *quality, int *stnum, float *hacu, float *altitude, char *aunit, int *gps_check);
    void lat_log_sensing_u16(char *lat, char *log, char * height);

private:
    SerialBuffered serial;
    DigitalOut CAM_SW;
    DigitalOut GPS_SW;
    static const int COMMAND_LENGTH = 6;
    static const int SYNCMAX = 60;
    static const int packageSize = 256;
    //static const int CAPTURE_FRAMES = 3;

    char msg[256],msgd[256];
    int i,ite,rlock,stn;
    int _quality,_stnum;
    float _hacu;
    float _altitude; 
    char _aunit; 
    int _gps_check;
    
    char c;
    char gps_data[7][1000];
    char ns,ew,statas;
    float time,hokui,tokei,vel;
    float g_hokui,g_tokei;
    float d_hokui,m_hokui,d_tokei,m_tokei;
    int h_time,m_time,s_time;
    float height_1;
    int m_height,cm_height;

    ErrorNumber sendInitial(Baud band, JpegResolution jr);
    ErrorNumber sendGetPicture(void);
    ErrorNumber sendSnapshot(void);
    ErrorNumber sendSetPackageSize(uint16_t packageSize);
    ErrorNumber sendReset(ResetType specialReset);
    ErrorNumber recvData(uint32_t *length);
    ErrorNumber sendSync();
    ErrorNumber recvSync();
    ErrorNumber sendAck(uint8_t commandId, uint16_t packageId);
    ErrorNumber recvAckOrNck();

    bool sendBytes(char *buf, size_t len, int timeout_us = 20000);
    bool recvBytes(char *buf, size_t len, int timeout_us = 20000);
    bool waitRecv();
    bool waitIdle();
};
#endif
