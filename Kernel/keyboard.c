#include <interrupts.h>
#include <keyboard.h>
#include <stdint.h>
#include <string.h>
#include <process.h>
#include <scheduler.h>
#include <video.h>

#define SCANCODE_BUFFER_MAXLENGTH 32

static char scancodeToAsciiTable[] = {0,   27,  '1', '2', '3', '4', '5', '6', '7',  '8', '9', '0',  '-',  '=', '\b', '\t',
                                      'q', 'w', 'e', 'r', 't', 'y', 'u', 'i', 'o',  'p', '[', ']',  '\n', 0,   'a',  's',
                                      'd', 'f', 'g', 'h', 'j', 'k', 'l', ';', '\'', '`', 0,   '\\', 'z',  'x', 'c',  'v',
                                      'b', 'n', 'm', ',', '.', '/', 0,   ' ', 0,    ' ', 0,   0,    0,    0,   0,    0,
                                      0,   0,   0,   0,   0,   0,   0,   0,   0,    0,   '-', 0,    0,    0,   '+'};

#define SCANCODE_ARR_LENGTH (sizeof(scancodeToAsciiTable) / sizeof(scancodeToAsciiTable[0]))

char
scancodeToAscii(uint8_t scancode) {
    return scancode < SCANCODE_ARR_LENGTH ? scancodeToAsciiTable[scancode] : '\0';
}

static TWaitQueue processReadWaitQueue;

static uint8_t scancodeBuffer[SCANCODE_BUFFER_MAXLENGTH];
static unsigned int scancodeBufferLength = 0;

static ssize_t fdReadHandler(Pid pid, int fd, void* resource, char* buf, size_t count);
static int fdCloseHandler(Pid pid, int fd, void* resource);
static int fdDupHandler(TPid pidFrom, TPid pidTo, int fdFrom, int fdTo, void* resource);

void kbd_init() {
    processReadWaitQueue = wq_new();
}

void
keyboardIntHandler(uint8_t scancode) {
    if (scancodeBufferLength < SCANCODE_BUFFER_MAXLENGTH){
        scancodeBuffer[scancodeBufferLength++] = scancode;
        wq_unblockAll(processReadWaitQueue);
        // scr_print("KEYBOARD!");
    }
}

unsigned int
kbd_getBufferLength() {
    return scancodeBufferLength;
}

void
kbd_clearBuffer() {
    scancodeBufferLength = 0;
}

unsigned int
kbd_readScancodes(uint8_t *buf, unsigned int n) {
    _cli();
    if (scancodeBufferLength < n)
        n = scancodeBufferLength;
    memcpy(buf, scancodeBuffer, n);
    scancodeBufferLength -= n;
    memcpy(scancodeBuffer, scancodeBuffer + n, scancodeBufferLength);
    _sti();
    return n;
}
// BRB

unsigned int
kbd_readCharacters(char *buf, unsigned int n) {
    _cli();
    unsigned int charsRead = 0;
    unsigned int scancodeIndex;
    scr_print(" readCharacter ");
    for (scancodeIndex = 0; scancodeIndex < scancodeBufferLength && charsRead < n; scancodeIndex++) {
        uint8_t sc = scancodeBuffer[scancodeIndex];
        char c;
        if ((sc & 0b10000000) == BIT_SCANCODE_DOWN && (c = scancodeToAscii(sc)) != 0){
            buf[charsRead++] = c;
            scr_print(" readChar ");}
    }

    scancodeBufferLength -= scancodeIndex;
    memcpy(scancodeBuffer, scancodeBuffer + scancodeIndex, scancodeBufferLength);
    _sti();
    return charsRead;
}

int kbd_mapToProcessFd(TPid pid, int fd) {
    scr_print(" MAPPING KBD -> PRC! ");
    return prc_mapFd(pid, fd, (void*)1, &fdReadHandler, NULL, &fdCloseHandler, &fdDupHandler);
}

static ssize_t fdReadHandler(TPid pid, int fd, void* resource, char* buf, size_t count) {
    scr_print(" startReadHandle ");
    // Only foreground processes are allowed to read from the keyboard.
    if (!prc_isForeground(pid))
        return -1;

    if (count == 0)
        return 0;

    if (count > SCANCODE_BUFFER_MAXLENGTH)
        count = SCANCODE_BUFFER_MAXLENGTH;

    int read;
    scr_print(" readHandler! ");
    while ((read = kbd_readCharacters(buf, count)) == 0) {
        wq_add(processReadWaitQueue, pid);
        sch_blockProcess(pid);
        sch_yieldProcess();
    }

    return read;
}

static int fdCloseHandler(TPid pid, int fd, void* resource) {
    wq_remove(processReadWaitQueue, pid);
    return 0;
}

static int fdDupHandler(TPid pidFrom, TPid pidTo, int fdFrom, int fdTo, void* resource) {
    return kbd_mapToProcessFd(pidTo, fdTo);
}