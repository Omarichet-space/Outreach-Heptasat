#ifndef _SERIAL_BUFFERED_H_
#define _SERIAL_BUFFERED_H_

/**
 * Buffered serial class.
 */
class SerialBuffered : public Serial {
public:
    /**
     * Create a buffered serial class.
     *
     * @param tx A pin for transmit.
     * @param rx A pin for receive.
     */
    SerialBuffered(PinName tx, PinName rx);

    /**
     * Destroy.
     */
    virtual ~SerialBuffered();

    /**
     * Get a character.
     *
     * @return A character. (-1:timeout)
     */
    int getc();

    /**
     * Returns 1 if there is a character available to read, 0 otherwise.
     */
    int readable();

    /**
     * Set timeout for getc().
     *
     * @param ms milliseconds. (-1:Disable timeout)
     */
    void setTimeout(int ms);

    /**
     * Read requested bytes.
     *
     * @param bytes A pointer to a buffer.
     * @param requested Length.
     *
     * @return Readed byte length.
     */
    size_t readBytes(uint8_t *bytes, size_t requested);

    void _baud(int b);
private:
    void handleInterrupt();
    static const int BUFFERSIZE = 4096;
    uint8_t buffer[BUFFERSIZE];            // points at a circular buffer, containing data from m_contentStart, for m_contentSize bytes, wrapping when you get to the end
    uint16_t indexContentStart;   // index of first bytes of content
    uint16_t indexContentEnd;     // index of bytes after last byte of content
    int timeout;
    Timer timer;
};

#endif
