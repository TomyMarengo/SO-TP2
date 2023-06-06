#include <defs.h>
#include <graphics.h>
#include <keyboard.h>
#include <lib.h>
#include <process.h>
#include <scheduler.h>
#include <waitingQueue.h>

#define LEFT_SHIFT      0x2A
#define RIGHT_SHIFT     0x36
#define BUFFER_MAX_SIZE 256

extern unsigned int readKey();

static uint8_t keyMapRow = 0;
static uint8_t buffer[BUFFER_MAX_SIZE];

static uint16_t bufferStart = 0;
static uint16_t bufferSize = 0;

static uint8_t scancodeLToAscii[] = {
    0,    27,  '1', '2',  '3', '4', '5', '6', '7',  '8', '9', '0', '-', '=', '\b', '\t', 'q', 'w', 'e', 'r',
    't',  'y', 'u', 'i',  'o', 'p', '[', ']', '\n', 0,   'a', 's', 'd', 'f', 'g',  'h',  'j', 'k', 'l', ';',
    '\'', '`', 0,   '\\', 'z', 'x', 'c', 'v', 'b',  'n', 'm', ',', '.', '/', 0,    '*',  0,   ' ', 0,   0,
    0,    0,   0,   0,    0,   0,   0,   0,   0,    0,   0,   0,   38,  0,   '-',  37,   0,   39,  '+', 0,
    40,   0,   0,   0,    0,   0,   0,   0,   0,    0,   0,   0,   0,   0,   0,    0,    0,   0,
};

static uint8_t scancodeUToAscii[] = {0,   27,  '!', '@', '#', '$', '%', '^', '&', '*', '(',  ')', '_', '+', '\b', '\t', 'Q', 'W',
                                     'E', 'R', 'T', 'Y', 'U', 'I', 'O', 'P', '{', '}', '\n', 0,   'A', 'S', 'D',  'F',  'G', 'H',
                                     'J', 'K', 'L', ':', '"', '~', 0,   '|', 'Z', 'X', 'C',  'V', 'B', 'N', 'M',  '<',  '>', '?',
                                     0,   '*', 0,   ' ', 0,   0,   0,   0,   0,   0,   0,    0,   0,   0,   0,    0,    0,   0,
                                     0,   0,   '-', 0,   0,   0,   '+', 0,   0,   0,   0,    0,   0,   0,   0,    0,    0,   0};

static uint8_t *keyMap[] = {scancodeLToAscii, scancodeUToAscii};

static ssize_t readHandler(Pid pid, int fd, void *resource, char *buf, size_t count);
static int closeHandler(Pid pid, int fd, void *resource);
static int dupHandler(Pid pidFrom, Pid pidTo, int fdFrom, int fdTo, void *resource);

static WaitingQueue processReadWQ;
static int ctrl = 0;

int
getCtrlState() {
    return ctrl;
}

void
initializeKeyboard() {
    processReadWQ = newQueue();
}

void
interruptHandlerKeyboard() {
    unsigned char code = readKey();
    if (code < 0x80) {
        if (code == LEFT_SHIFT || code == RIGHT_SHIFT) {
            keyMapRow |= 0x01;
        } else if (code == 0x1D) {
            ctrl = 1;
        } else {
            uint8_t keyChar = keyMap[keyMapRow][code];
            if (keyChar == 'c' && ctrl == 1) {
                if (getpid() != 0)
                    killCurrentProcess();
                return;
            }
            if (keyChar != 0 && bufferSize < BUFFER_MAX_SIZE) {
                uint16_t bufferEnd = (bufferStart + bufferSize) % BUFFER_MAX_SIZE;
                buffer[bufferEnd] = keyChar;
                bufferSize++;
                unblockAllInQueue(processReadWQ);
            }
        }
    } else {
        if (code == 0x9D) {
            ctrl = 0;
        }
        code -= 0x80;
        if (code == LEFT_SHIFT || code == RIGHT_SHIFT) {
            keyMapRow &= 0xFE;
        }
    }
}

unsigned int
readChars(char *bufferDestination, unsigned int count) {
    unsigned int charsToRead = bufferSize;
    if (charsToRead > count)
        charsToRead = count;

    if (charsToRead == 0)
        return 0;

    unsigned int firstReadSize = BUFFER_MAX_SIZE - bufferStart;
    if (firstReadSize > charsToRead)
        firstReadSize = charsToRead;
    memcpy(bufferDestination, buffer + bufferStart, firstReadSize);

    if (firstReadSize < charsToRead)
        memcpy(bufferDestination + firstReadSize, buffer, charsToRead - firstReadSize);

    bufferSize -= charsToRead;
    bufferStart = (bufferStart + charsToRead) % BUFFER_MAX_SIZE;
    return charsToRead;
}

int
getChar() {
    if (bufferSize == 0)
        return -1;

    int c = buffer[bufferStart];
    bufferSize--;
    bufferStart = (bufferStart + 1) % BUFFER_MAX_SIZE;
    return c;
}

void
clearKeyboard() {
    bufferStart = 0;
    bufferSize = 0;
}

int
addFdKeyboard(Pid pid, int fd) {
    return addFd(pid, fd, (void *) 1, &readHandler, NULL, &closeHandler, &dupHandler);
}

static ssize_t
readHandler(Pid pid, int fd, void *resource, char *buf, size_t count) {
    if (!isForeground(pid))
        return -1;

    if (count == 0)
        return 0;

    if (count > BUFFER_MAX_SIZE)
        count = BUFFER_MAX_SIZE;

    int read;
    while ((read = readChars(buf, count)) == 0) {
        addInQueue(processReadWQ, pid);
        block(pid);
        yield();
    }

    return read;
}

static int
closeHandler(Pid pid, int fd, void *resource) {
    removeInQueue(processReadWQ, pid);
    return 0;
}

static int
dupHandler(Pid pidFrom, Pid pidTo, int fdFrom, int fdTo, void *resource) {
    return addFdKeyboard(pidTo, fdTo);
}
