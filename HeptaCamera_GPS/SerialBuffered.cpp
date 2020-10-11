#include "mbed.h"
#include "SerialBuffered.h"
 
/**
 * Create a buffered serial class.
 *
 * @param tx A pin for transmit.
 * @param rx A pin for receive.
 */
SerialBuffered::SerialBuffered(PinName tx, PinName rx) : Serial(tx, rx) {
    indexContentStart = 0;
    indexContentEnd = 0;
    timeout = 1;
    attach(this, &SerialBuffered::handleInterrupt);
}

/**
 * Destroy.
 */
SerialBuffered::~SerialBuffered() {
}

/**
 * Set timeout for getc().
 *
 * @param ms milliseconds. (-1:Disable timeout)
 */
void SerialBuffered::setTimeout(int ms) {
    timeout = ms;
}

/**
 * Read requested bytes.
 *
 * @param bytes A pointer to a buffer.
 * @param requested Length.
 *
 * @return Readed byte length.
 */
size_t SerialBuffered::readBytes(uint8_t *bytes, size_t requested) {
    int i = 0;
    while (i < requested) {
        int c = getc();
        if (c < 0) {
            break;
        }
        bytes[i] = c;
        i++;
    }
    return i;
}

/**
 * Get a character.
 *
 * @return A character. (-1:timeout)
 */
int SerialBuffered::getc() {
    timer.reset();
    timer.start();
    while (indexContentStart == indexContentEnd) {
        wait_ms(1);
        if ((timeout > 0) && (timer.read_ms() > timeout)) {
            /*
             * Timeout occured.
             */
            // printf("Timeout occured.\n");
            return EOF;
        }
    }
    timer.stop();

    uint8_t result = buffer[indexContentStart++];
    indexContentStart =  indexContentStart % BUFFERSIZE;

    return result;
}

/**
 * Returns 1 if there is a character available to read, 0 otherwise.
 */
int SerialBuffered::readable() {
    return indexContentStart != indexContentEnd;
}

void SerialBuffered::handleInterrupt() {
    while (Serial::readable()) {
        if (indexContentStart == ((indexContentEnd + 1) % BUFFERSIZE)) {
            /*
             * Buffer overrun occured.
             */
            // printf("Buffer overrun occured.\n");
            Serial::getc();
        } else {
            buffer[indexContentEnd++] = Serial::getc();
            indexContentEnd = indexContentEnd % BUFFERSIZE;
        }
    }
}

void SerialBuffered::_baud(int b) {
    Serial::baud(b);
}    
