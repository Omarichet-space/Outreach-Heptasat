#include "HeptaCamera_GPS.h"
#define WAITIDLE    waitIdle
#define SENDFUNC    sendBytes
#define RECVFUNC    recvBytes
#define WAITFUNC    waitRecv
int num=0;
/**
 * Constructor.
 *
 * @param tx A pin for transmit.
 * @param rx A pin for receive.
 * @param baud Baud rate. (Default is Baud14400.)
 */
HeptaCamera_GPS::HeptaCamera_GPS(PinName tx, PinName rx, PinName CAM, PinName GPS) : serial(tx, rx), CAM_SW(CAM), GPS_SW(GPS)
{
    //serial.baud(baud);
}

/**
 * Destructor.
 */
HeptaCamera_GPS::~HeptaCamera_GPS()
{
}

/**
 * Make a sync. for baud rate.
 */
HeptaCamera_GPS::ErrorNumber HeptaCamera_GPS::sync()
{
    for (int i = 0; i < SYNCMAX; i++) {
        if (NoError == sendSync()) {
            if (NoError == recvAckOrNck()) {
                if (NoError == recvSync()) {
                    if (NoError == sendAck(0x0D, 0x00)) {
                        /*
                         * After synchronization, the camera needs a little time for AEC and AGC to be stable.
                         * Users should wait for 1-2 seconds before capturing the first picture.
                         */
                        wait(2.0);
                        return NoError;
                    }
                }
            }
        }
        wait_ms(50);
    }
    return UnexpectedReply;
}

/**
 * Initialize.
 *
 * @param baud Camera Interface Speed.
 * @param jr JPEG resolution.
 */
HeptaCamera_GPS::ErrorNumber HeptaCamera_GPS::init(Baud baud,JpegResolution jr)
{
    int i ;
    ErrorNumber en;
    WAITIDLE();
    setmbedBaud((Baud)(0x07)) ;

    for ( i = 1 ; i < 7 ; i++ ) {
        if ( NoError == sendSync() ) {
            if ( NoError == recvAckOrNck() ) {
                if ( NoError == recvSync() ) {
                    if ( NoError == sendAck(0x0D, 0x00) ) {
                        en = sendInitial(baud,jr);
                        if (NoError != en) {
                            return en;
                        }
                        en = recvAckOrNck();
                        if (NoError != en) {
                            return en;
                        }
                        wait_ms(50) ;
                        setmbedBaud(baud);
                        //wait_ms(50) ;
                        static bool alreadySetupPackageSize = false;
                        if (!alreadySetupPackageSize) {
                            en = sendSetPackageSize(packageSize);
                            if (NoError != en) {
                                return en;
                            }
                            WAITFUNC();
                            en = recvAckOrNck();
                            if (NoError != en) {
                                return en;
                            }
                            alreadySetupPackageSize = true;
                        }

                        wait(2.0);
                        return (ErrorNumber)NoError;
                        /*
                         * After synchronization, the camera needs a little time for AEC and AGC to be stable.
                         * Users should wait for 1-2 seconds before capturing the first picture.
                         */
                    }
                }
            } else {
                setmbedBaud((Baud)(i+1)) ;
            }
        }
        wait_ms(50);
    }
    return UnexpectedReply;
}


/**
 * Get JPEG snapshot picture.
 *
 * @param func A pointer to a callback function.
 *             You can block this function until saving the image datas.
 * @return Status of the error.
 */
HeptaCamera_GPS::ErrorNumber HeptaCamera_GPS::getJpegSnapshotPicture(FILE*fp)
{
    WAITIDLE();
    ErrorNumber en;


    en = sendSnapshot();
    if (NoError != en) {
        return en;
    }
    WAITFUNC();
    en = recvAckOrNck();
    if (NoError != en) {
        return en;
    }

    en = sendGetPicture();
    if (NoError != en) {
        return en;
    }
    WAITFUNC();
    en = recvAckOrNck();
    if (NoError != en) {
        return en;
    }

    /*
     * Data : snapshot picture
     */
    uint32_t length = 0;
    WAITFUNC();
    en = recvData(&length);
    if (NoError != en) {
        return en;
    }
    en = sendAck(0x00, 0);
    if (NoError != en) {
        return en;
    }

    char databuf[packageSize - 6];
    uint16_t pkg_total = length / (packageSize - 6);
    for (int i = 0; i <= (int)pkg_total; i++) {
        uint16_t checksum = 0;
        // ID.
        char idbuf[2];
        WAITFUNC();
        if (!RECVFUNC(idbuf, sizeof(idbuf))) {
            return (ErrorNumber)UnexpectedReply;
        }
        checksum += idbuf[0];
        checksum += idbuf[1];
        uint16_t id = (idbuf[1] << 8) | (idbuf[0] << 0);
        if (id != i) {
            return (ErrorNumber)UnexpectedReply;
        }

        // Size of the data.
        char dsbuf[2];
        WAITFUNC();
        if (!RECVFUNC(dsbuf, sizeof(dsbuf))) {
            return (ErrorNumber)UnexpectedReply;
        }

        // Received the data.
        checksum += dsbuf[0];
        checksum += dsbuf[1];
        uint16_t ds = (dsbuf[1] << 8) | (dsbuf[0] << 0);
        WAITFUNC();
        if (!RECVFUNC(&databuf[0], ds)) {
            return (ErrorNumber)UnexpectedReply;
        }
        for (int j = 0; j < ds; j++) {
            checksum += databuf[j];
        }

        // Verify code.
        char vcbuf[2];
        WAITFUNC();
        if (!RECVFUNC(vcbuf, sizeof(vcbuf))) {
            return (ErrorNumber)UnexpectedReply;
        }
        uint16_t vc = (vcbuf[1] << 8) | (vcbuf[0] << 0);
        if (vc != (checksum & 0xff)) {
            return (ErrorNumber)UnexpectedReply;
        }

        /*
         * Call a call back function.
         * You can block this function while working.
         */
        size_t siz = ds;
        for (int ii = 0; ii < (int)siz; ii++) {
            fprintf(fp, "%c", databuf[ii]);
        }
        /*
         * We should wait for camera working before reply a ACK.
         */
        wait_ms(100);
        en = sendAck(0x00, 1 + i);
        if (NoError != en) {
            return en;
        }
    }

    return (ErrorNumber)NoError;
}

HeptaCamera_GPS::ErrorNumber HeptaCamera_GPS::getJpegSnapshotPicture_data(FILE*fp)
{
    WAITIDLE();
    ErrorNumber en;


    en = sendSnapshot();
    if (NoError != en) {
        return en;
    }
    WAITFUNC();
    en = recvAckOrNck();
    if (NoError != en) {
        return en;
    }

    en = sendGetPicture();
    if (NoError != en) {
        return en;
    }
    WAITFUNC();
    en = recvAckOrNck();
    if (NoError != en) {
        return en;
    }

    /*
     * Data : snapshot picture
     */
    uint32_t length = 0;
    WAITFUNC();
    en = recvData(&length);
    if (NoError != en) {
        return en;
    }
    en = sendAck(0x00, 0);
    if (NoError != en) {
        return en;
    }

    char databuf[packageSize - 6];
    uint16_t pkg_total = length / (packageSize - 6);
    for (int i = 0; i <= (int)pkg_total; i++) {
        uint16_t checksum = 0;
        // ID.
        char idbuf[2];
        WAITFUNC();
        if (!RECVFUNC(idbuf, sizeof(idbuf))) {
            return (ErrorNumber)UnexpectedReply;
        }
        checksum += idbuf[0];
        checksum += idbuf[1];
        uint16_t id = (idbuf[1] << 8) | (idbuf[0] << 0);
        if (id != i) {
            return (ErrorNumber)UnexpectedReply;
        }

        // Size of the data.
        char dsbuf[2];
        WAITFUNC();
        if (!RECVFUNC(dsbuf, sizeof(dsbuf))) {
            return (ErrorNumber)UnexpectedReply;
        }

        // Received the data.
        checksum += dsbuf[0];
        checksum += dsbuf[1];
        uint16_t ds = (dsbuf[1] << 8) | (dsbuf[0] << 0);
        WAITFUNC();
        if (!RECVFUNC(&databuf[0], ds)) {
            return (ErrorNumber)UnexpectedReply;
        }
        for (int j = 0; j < ds; j++) {
            checksum += databuf[j];
        }

        // Verify code.
        char vcbuf[2];
        WAITFUNC();
        if (!RECVFUNC(vcbuf, sizeof(vcbuf))) {
            return (ErrorNumber)UnexpectedReply;
        }
        uint16_t vc = (vcbuf[1] << 8) | (vcbuf[0] << 0);
        if (vc != (checksum & 0xff)) {
            return (ErrorNumber)UnexpectedReply;
        }

        /*
         * Call a call back function.
         * You can block this function while working.
         */
        size_t siz = ds;
        for (int ii = 0; ii < (int)siz; ii++) {
            fprintf(fp, "%02X ", databuf[ii]);
            if(++num%16==0) fprintf(fp,"\r\n");
        }
        /*
         * We should wait for camera working before reply a ACK.
         */
        wait_ms(100);
        en = sendAck(0x00, 1 + i);
        if (NoError != en) {
            return en;
        }
    }

    return (ErrorNumber)NoError;
}

HeptaCamera_GPS::ErrorNumber HeptaCamera_GPS::sendInitial(Baud baud, JpegResolution jr)
{
    char send[COMMAND_LENGTH];

    send[0] = 0xAA;
    send[1] = 0x01;
    send[2] = (char)baud;
    send[3] = 0x07;
    send[4] = 0x00;
    send[5] = (char)jr;

    if (!SENDFUNC(send, sizeof(send))) {
        return (ErrorNumber)SendRegisterTimeout;
    }

    return (ErrorNumber)NoError;
}

HeptaCamera_GPS::ErrorNumber HeptaCamera_GPS::sendGetPicture(void)
{
    char send[COMMAND_LENGTH];

    send[0] = 0xAA;
    send[1] = 0x04;
    send[2] = 0x01;
    send[3] = 0x00;
    send[4] = 0x00;
    send[5] = 0x00;

    if (!SENDFUNC(send, sizeof(send))) {
        return (ErrorNumber)SendRegisterTimeout;
    }
    return (ErrorNumber)NoError;
}

HeptaCamera_GPS::ErrorNumber HeptaCamera_GPS::sendSnapshot(void)
{
    char send[COMMAND_LENGTH];
    send[0] = 0xAA;
    send[1] = 0x05;
    send[2] = 0x00;
    send[3] = 0x00;
    send[4] = 0x00;
    send[5] = 0x00;

    if (!SENDFUNC(send, sizeof(send))) {
        return (ErrorNumber)SendRegisterTimeout;
    }
    return (ErrorNumber)NoError;
}

HeptaCamera_GPS::ErrorNumber HeptaCamera_GPS::sendSetPackageSize(uint16_t packageSize)
{
    char send[COMMAND_LENGTH];
    send[0] = 0xAA;
    send[1] = 0x06;
    send[2] = 0x08;
    send[3] = (packageSize >> 0) & 0xff;
    send[4] = (packageSize >> 8) & 0xff;
    send[5] = 0x00;

    if (!SENDFUNC(send, sizeof(send))) {
        return (ErrorNumber)SendRegisterTimeout;
    }
    return (ErrorNumber)NoError;
}

void HeptaCamera_GPS::setmbedBaud(Baud baud)
{
    switch((int)baud) {
        case 0x02:
            serial._baud(460800);
            break;
        case 0x03:
            serial._baud(230400);
            break;
        case 0x04:
            serial._baud(115200);
            break;
        case 0x05:
            serial._baud(57600);
            break;
        case 0x06:
            serial._baud((int)28800);
            break;
        case 0x07:
            serial._baud(14400);
            break;
        default:
            serial._baud(14400);
    }
}


HeptaCamera_GPS::ErrorNumber HeptaCamera_GPS::sendReset(ResetType specialReset)
{
    char send[COMMAND_LENGTH];
    send[0] = 0xAA;
    send[1] = 0x08;
    send[2] = 0x00;
    send[3] = 0x00;
    send[4] = 0x00;
    send[5] = specialReset;
    /*
     * Special reset : If the parameter is 0xFF, the command is a special Reset command and the firmware responds to it immediately.
     */

    if (!SENDFUNC(send, sizeof(send))) {
        return (ErrorNumber)SendRegisterTimeout;
    }

    return (ErrorNumber)NoError;
}


HeptaCamera_GPS::ErrorNumber HeptaCamera_GPS::recvData(uint32_t *length)
{
    char recv[COMMAND_LENGTH];
    if (!RECVFUNC(recv, sizeof(recv))) {
        return (ErrorNumber)UnexpectedReply;
    }
    if ((0xAA != recv[0]) || (0x0A != recv[1])) {
        return (ErrorNumber)UnexpectedReply;
    }
    recv[2] = (char)0x01;
    *length = (recv[5] << 16) | (recv[4] << 8) | (recv[3] << 0);
    return (ErrorNumber)NoError;
}

HeptaCamera_GPS::ErrorNumber HeptaCamera_GPS::sendSync()
{
    char send[COMMAND_LENGTH];
    send[0] = 0xAA;
    send[1] = 0x0D;
    send[2] = 0x00;
    send[3] = 0x00;
    send[4] = 0x00;
    send[5] = 0x00;


    if (!SENDFUNC(send, sizeof(send))) {
        return (ErrorNumber)SendRegisterTimeout;
    }
    return (ErrorNumber)NoError;
}

HeptaCamera_GPS::ErrorNumber HeptaCamera_GPS::recvSync()
{
    char recv[COMMAND_LENGTH];

    if (!RECVFUNC(recv, sizeof(recv))) {
        return (ErrorNumber)UnexpectedReply;
    }
    if ((0xAA != recv[0]) || (0x0D != recv[1])) {
        return (ErrorNumber)UnexpectedReply;
    }
    return (ErrorNumber)NoError;
}

/**
 * Send ACK.
 *
 * @param commandId The command with that ID is acknowledged by this command.
 * @param packageId For acknowledging Data command, these two bytes represent the requested package ID. While for acknowledging other commands, these two bytes are set to 00h.
 */
HeptaCamera_GPS::ErrorNumber HeptaCamera_GPS::sendAck(uint8_t commandId, uint16_t packageId)
{
    char send[COMMAND_LENGTH];

    send[0] = 0xAA;
    send[1] = 0x0E;
    send[2] = commandId;
    send[3] = 0x00;    // ACK counter is not used.
    send[4] = (packageId >> 0) & 0xff;
    send[5] = (packageId >> 8) & 0xff;
    if (!SENDFUNC(send, sizeof(send))) {
        return (ErrorNumber)SendRegisterTimeout;
    }
    return (ErrorNumber)NoError;
}

/**
 * Receive ACK or NCK.
 *
 * @return Error number.
 */
HeptaCamera_GPS::ErrorNumber HeptaCamera_GPS::recvAckOrNck()
{
    char recv[COMMAND_LENGTH];

    if (!RECVFUNC(recv, sizeof(recv))) {
        return (ErrorNumber)UnexpectedReply;
    }
    if ((0xAA == recv[0]) && (0x0E == recv[1])) {
        return (ErrorNumber)NoError;
    }
    if ((0xAA == recv[0]) && (0x0F == recv[1]) && (0x00 == recv[2])) {
        return (ErrorNumber)NoError;
    }
    if ((0xAA == recv[0]) && (0x0F == recv[1])) {
        return (ErrorNumber)recv[4];
    }
    return (ErrorNumber)UnexpectedReply;
}

/**
 * Send bytes to camera module.
 *
 * @param buf Pointer to the data buffer.
 * @param len Length of the data buffer.
 *
 * @return True if the data sended.
 */
bool HeptaCamera_GPS::sendBytes(char *buf, size_t len, int timeout_us)
{
    for (uint32_t i = 0; i < (uint32_t)len; i++) {
        int cnt = 0;
        while (!serial.writeable()) {
            wait_us(1);
            cnt++;
            if (timeout_us < cnt) {
                return false;
            }
        }
        serial.putc(buf[i]);
    }
    return true;
}


bool HeptaCamera_GPS::recvBytes(char *buf, size_t len, int timeout_us)
{
    for (uint32_t i = 0; i < (uint32_t)len; i++) {
        int cnt = 0;
        while (!serial.readable()) {
            wait_us(1);
            cnt++;
            if (timeout_us < cnt) {
                return false;
            }
        }
        buf[i] = serial.getc();
    }
    return true;
}


bool HeptaCamera_GPS::waitRecv()
{
    while (!serial.readable()) {
    }
    return true;
}

bool HeptaCamera_GPS::waitIdle()
{
    while (serial.readable()) {
        serial.getc();
    }
    return true;
}

void HeptaCamera_GPS::camera_setting(void)
{
    GPS_SW = 0;
    CAM_SW = 1;
    serial.setTimeout(1);
    flushSerialBuffer();
}

void HeptaCamera_GPS::Sync(void)
{
    camera_setting();
    HeptaCamera_GPS::ErrorNumber err = HeptaCamera_GPS::NoError;
    printf("synchro setting now\r\n");
    err = sync();
    int count = 0;
    int fflag = 0;
    while(err) {
        switch(count) {
            case 0:
                printf("Connection of camera and mbed at baudrate 14400\r\n");
                setmbedBaud(HeptaCamera_GPS::Baud14400);
                count++;
                break;
            case 1:
                printf("Connection of camera and mbed at baudrate 115200\r\n");
                setmbedBaud(HeptaCamera_GPS::Baud115200);
                count++;
                break;
            default:
                count=0;
                fflag = 1;
        }
        //count++;
        err = sync();
        printf("synchro setting now\r\n");
        if(!err) {
            printf("synchro setting finish\r\n");
        }
        if(fflag==1) {
            printf("Synchronization failed\r\n");
            fflag = 0;
            break;
        }
    }//while
}

void HeptaCamera_GPS::test_jpeg_snapshot_picture(const char *filename)
{
    FILE*fp_jpeg;
    HeptaCamera_GPS::ErrorNumber err = HeptaCamera_GPS::NoError;
    fp_jpeg = fopen(filename, "w");
    if(fp_jpeg == NULL) {
        printf("Could not open file for write\r\n");
    } else {
        err = getJpegSnapshotPicture(fp_jpeg);

        if (HeptaCamera_GPS::NoError == err) {
            printf("[ OK ]:Picture taken\r\n");
        } else {
            printf("[FAIL]:Picture taken(Error=%02X)\r\n", (int)err);
        }
        fclose(fp_jpeg);
    }
}

void HeptaCamera_GPS::test_jpeg_snapshot_data(const char *filename)
{
    FILE*fp_jpeg;
    HeptaCamera_GPS::ErrorNumber err = HeptaCamera_GPS::NoError;
    fp_jpeg = fopen(filename, "w");
    if(fp_jpeg == NULL) {
        printf("Could not open file for write\r\n");
    } else {
        err = getJpegSnapshotPicture_data(fp_jpeg);

        if (HeptaCamera_GPS::NoError == err) {
            printf("[ OK ]:Picture taken\r\n");
        } else {
            printf("[FAIL]:Picture taken(Error=%02X)\r\n", (int)err);
        }
        fclose(fp_jpeg);
    }
}

void HeptaCamera_GPS::initialize(Baud baud,JpegResolution jr)
{
    HeptaCamera_GPS::ErrorNumber err = HeptaCamera_GPS::NoError;//ErrorNumber define
    err = init(baud, jr);//
    if (HeptaCamera_GPS::NoError == err) {
        printf("[ OK ]:Camera setting\r\n");
        setmbedBaud(baud);
    } else {
        printf("[FAIL]:Camera setting(Error=%02X)\r\n", (int)err);
    }

}

//*********************serial*********************//
void HeptaCamera_GPS::gps_setting(void)
{
    CAM_SW = 0;
    GPS_SW = 1;
    flushSerialBuffer();
    serial._baud(4800);
    serial.setTimeout(9999);
}

char HeptaCamera_GPS::getc()
{
    c = serial.getc();
    return c;
}
int HeptaCamera_GPS::readable()
{
    i = serial.readable();
    return i;
}
void HeptaCamera_GPS::flushSerialBuffer(void)
{
    ite = 0;
    while (serial.readable()) {
        serial.getc();
        ite++;
        if(ite==100) {
            break;
        };
    }
    return;
}
void HeptaCamera_GPS::gga_sensing(float *time, float *latitude, char *ns, float *longitude, char *ew, int *quality, int *stnum, float *hacu, float *altitude, char *aunit, int *serial_check)
{
    int ite = 0;
    while(serial.getc()!='$') {
        ite++;
        if(ite==10000) break;
    }
    for(int i=0; i<5; i++) {
        msg[i] = serial.getc();
    }
    if((msg[2]=='G')&(msg[3]=='G')&(msg[4]=='A')) {
        for(int j=0; j<6; j++) {
            if(j==0) {
                for(int i=5; i<256; i++) {
                    msg[i] = serial.getc();
                    if(msg[i] == '\r') {
                        msg[i] = 0;
                        break;
                    }
                }
            } else {
                for(int i=0; i<256; i++) {
                    msgd[i] = serial.getc();
                    if(msgd[i] == '\r') {
                        break;
                    }
                }
                if((msgd[4]=='V')&(msgd[5]=='T')&(msgd[6]=='G')) {
                    break;
                }
            }
        }
        if(sscanf(msg, "GPGGA,%f,%f,%c,%f,%c,%d,%d,%f,%f,%c", time, latitude, ns, longitude, ew, quality, stnum, hacu, altitude, aunit) >= 1) {
            if(!(quality)) {
                //latitude(unit transformation)
                *latitude=int(*latitude/100)+(*latitude-int(*latitude/100)*100)/60;
                //longitude(unit transformation)
                *longitude = int(*longitude/100)+(*longitude-int(*longitude/100)*100)/60;
                *serial_check = 0;
            } else {
                //latitude(unit transformation)
                *latitude=int(*latitude/100)+(*latitude-int(*latitude/100)*100)/60;
                //longitude(unit transformation)
                *longitude = int(*longitude/100)+(*longitude-int(*longitude/100)*100)/60;
                *serial_check = 1;
            }
            if(!strcmp(ns,"S")){
                *latitude = -*latitude;
            }
            if(!strcmp(ns,"W")){
                *longitude = -*longitude;
            }
        } else {
            printf("No Data");
            *serial_check = 2;
        }
    } else {
        *serial_check = 3;
    }
}

void HeptaCamera_GPS::lat_log_sensing_u16(char *lat, char *log, char *height)
{
    char gph1[8]= {0x00},gph2[8]= {0x00},gph3[8]= {0x00},gph4[8]= {0x00},gpt1[8]= {0x00},gpt2[8]= {0x00},gpt3[8]= {0x00},gpt4[8]= {0x00};
    char hei1[8]= {0x00},hei2[8]= {0x00};
    int i=0,j=0;
    while (serial.readable()) {
        serial.getc();
    }
loop:
    while(serial.getc()!='$') {}
    for(j=0; j<5; j++) {
        gps_data[1][j]=serial.getc();
    }
    if((gps_data[1][2]==0x47)&(gps_data[1][3]==0x47)&(gps_data[1][4]==0x41)) {
        for(j=0; j<1; j++) {
            if(j==0) {
                i=0;
                while((gps_data[j+1][i+5] = serial.getc()) != '\r') {
                    //pc.putc(gps_data[j+1][i+5]);
                    i++;
                }
                gps_data[j+1][i+5]='\0';
                i=0;
                //pc.printf("\n\r");
            } else {
                while(serial.getc()!='$') {}
                i=0;
                while((gps_data[j+1][i] = serial.getc()) != '\r') {
                    //pc.putc(gps_data[j+1][i]);
                    i++;
                }
                gps_data[j+1][i]='\0';
                i=0;
                //pc.printf("\n\r");
            }
        }
    } else {
        goto loop;
    }
    if( sscanf(gps_data[1],"GPGGA,%f,%f,%c,%f,%c,%d,%d,%f,%f,%c", &time, &hokui, &ns, &tokei, &ew, &_quality, &_stnum, &_hacu, &_altitude, &_aunit) >= 1) {
        //hokui
        d_hokui=int(hokui/100);
        m_hokui=(hokui-d_hokui*100);
        //m_hokui=(hokui-d_hokui*100)/60;
        g_hokui=d_hokui+(hokui-d_hokui*100)/60;
        //printf("%f\r\n",hokui);
        sprintf( gph1, "%02X", (char(d_hokui)) & 0xFF);
        sprintf( gph2, "%02X", (char(m_hokui)) & 0xFF);
        sprintf( gph3, "%02X", (char((m_hokui-char(m_hokui))*100)) & 0xFF);
        sprintf( gph4, "%02X", (char(((m_hokui-char(m_hokui))*100-char((m_hokui-char(m_hokui))*100))*100)) & 0xFF);

        //tokei
        d_tokei=int(tokei/100);
        m_tokei=(tokei-d_tokei*100);
        //m_tokei=(tokei-d_tokei*100)/60;
        g_tokei=d_tokei+(tokei-d_tokei*100)/60;
        //printf("%f\r\n",tokei);
        sprintf( gpt1, "%02X", (char(d_tokei)) & 0xFF);
        sprintf( gpt2, "%02X", (char(m_tokei)) & 0xFF);
        sprintf( gpt3, "%02X", (char((m_tokei-char(m_tokei))*100)) & 0xFF);
        sprintf( gpt4, "%02X", (char(((m_tokei-char(m_tokei))*100-char((m_tokei-char(m_tokei))*100))*100)) & 0xFF);

        m_height = int(_altitude);
        cm_height = int((_altitude-m_height)*100 );
        // printf("%f\r\n",_altitude);
        sprintf( hei1, "%02X", (char(m_height)) & 0xFF);
        sprintf( hei2, "%02X", (char(cm_height)) & 0xFF);

        lat[0] = gph1[0];
        lat[1] = gph1[1];
        lat[2] = gph2[0];
        lat[3] = gph2[1];
        lat[4] = gph3[0];
        lat[5] = gph3[1];
        lat[6] = gph4[0];
        lat[7] = gph4[1];
        log[0] = gpt1[0];
        log[1] = gpt1[1];
        log[2] = gpt2[0];
        log[3] = gpt2[1];
        log[4] = gpt3[0];
        log[5] = gpt3[1];
        log[6] = gpt4[0];
        log[7] = gpt4[1];
        height[0] = hei1[0];
        height[1] = hei1[1];
        height[2] = hei2[0];
        height[3] = hei2[1];
    }
    //*dsize1 = 8;
    //*dsize2 = 4;
}
