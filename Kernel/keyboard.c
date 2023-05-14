#include <stdint.h>
#include <keyboard.h>
#include <string.h>
#include <interrupts.h>

#define SCANCODE_BUFFER_MAXLENGTH 32

static char scancodeToAsciiTable[] = {
    0, 27, '1', '2', '3', '4', '5', '6', '7', '8',
    '9', '0', '-', '=', '\b', '\t', 'q', 'w', 'e', 'r',
    't', 'y', 'u', 'i', 'o', 'p', '[', ']', '\n', 0,
    'a', 's', 'd', 'f', 'g', 'h', 'j', 'k', 'l', ';',
    '\'', '`', 0, '\\', 'z', 'x', 'c', 'v', 'b', 'n',
    'm', ',', '.', '/', 0, ' ', 0, ' ', 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, '-', 0, 0, 0, '+'
};

#define SCANCODE_ARR_LENGTH (sizeof(scancodeToAsciiTable)/sizeof(scancodeToAsciiTable[0]))

char scancodeToAscii(uint8_t scancode) {
    return scancode < SCANCODE_ARR_LENGTH ? scancodeToAsciiTable[scancode] : '\0';
}

static uint8_t scancodeBuffer[SCANCODE_BUFFER_MAXLENGTH];
static unsigned int scancodeBufferLength = 0;

void keyboardIntHandler(uint8_t scancode) {
    if (scancodeBufferLength < SCANCODE_BUFFER_MAXLENGTH)
        scancodeBuffer[scancodeBufferLength++] = scancode;
}

unsigned int kbd_getBufferLength() {
    return scancodeBufferLength;
}

void kbd_clearBuffer() {
    scancodeBufferLength = 0;
}

unsigned int kbd_readScancodes(uint8_t* buf, unsigned int n) {
    _cli();
    if (scancodeBufferLength < n)
        n = scancodeBufferLength;
    memcpy(buf, scancodeBuffer, n);
    scancodeBufferLength -= n;
    memcpy(scancodeBuffer, scancodeBuffer + n, scancodeBufferLength);
    _sti();
    return n;
}

unsigned int kbd_readCharacters(char* buf, unsigned int n) {
    _cli();
    unsigned int charsRead = 0;
    unsigned int scancodeIndex;
    for (scancodeIndex = 0; scancodeIndex < scancodeBufferLength && charsRead < n; scancodeIndex++) {
        uint8_t sc = scancodeBuffer[scancodeIndex];
        char c;
        if ((sc & 0b10000000) == BIT_SCANCODE_DOWN && (c = scancodeToAscii(sc)) != 0)
            buf[charsRead++] = c;
    }

    scancodeBufferLength -= scancodeIndex;
    memcpy(scancodeBuffer, scancodeBuffer + scancodeIndex, scancodeBufferLength);
    _sti();
    return charsRead;
}
