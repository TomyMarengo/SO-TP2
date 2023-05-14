#include <stdint.h>
#include <interrupts.h>
#include <video.h>
#include <time.h>
#include <keyboard.h>

// From loader.asm, resets states and re-calls kernel main.
extern void resetmain(void);

static const char* exceptionMessages[] = {
/* 0x00 */ "Divide by Zero",
/* 0x01 */ 0,
/* 0x02 */ 0,
/* 0x03 */ 0,
/* 0x04 */ 0,
/* 0x05 */ 0,
/* 0x06 */ "Invalid Opcode",
/* 0x07 */ 0,
/* 0x08 */ 0,
/* 0x09 */ 0,
/* 0x0A */ 0,
/* 0x0B */ 0,
/* 0x0C */ 0,
/* 0x0D */ "General Protection",
/* 0x0E */ "Page Fault",
};

#define BORDER_SIZE 20
#define FRAME_HEIGHT 300

static const char* registerNames[18] = {
    "RAX", "RBX", "RCX", "RDX", "RSI", "RDI", "RBP", "RSP", "R8 ", "R9 ", "R10", "R11", "R12", "R13", "R14", "R15", "RIP", "RFLAGS"
};

static const char* errorMessage = "The program had to be aborted due to an unhandled exception: ";

/* Converts a uint64_t into a 16-char hexadecimal number. */
static void uint64ToHex(uint64_t n, char buf[16]) {
    int i=15;
    do {
        int digit = n % 16;
        buf[i] = (digit < 10 ? '0' : ('A' - 10)) + digit;
        n /= 16;
    } while(i-- != 0);
}

void exceptionHandler(uint64_t code, const uint64_t regdata[17]) {
    // temporary buffer used to print the register values.
    char hexbuf[19];
    hexbuf[0] = '0';
    hexbuf[1] = 'x';
    hexbuf[18] = '\0';

    uint16_t scrHeight = scr_getHeight();
    uint16_t scrWidth = scr_getWidth();

    // We find a place close to the middle of the screen to write the error messages.
    uint16_t drawY = scrHeight / 4;
    if (scrHeight - drawY < (FRAME_HEIGHT+BORDER_SIZE))
        drawY = scrHeight < (FRAME_HEIGHT+BORDER_SIZE) ? 0 : (scrHeight - (FRAME_HEIGHT+BORDER_SIZE));

    Color black = {0x00, 0x00, 0x00};
    Color gray = {0x90, 0x90, 0x90};
    Color red = {0x00, 0x00, 0xFF};

    // Draw the frame in which the error message will be shown.
    scr_drawRect(0, drawY, scrWidth, BORDER_SIZE, red);
    drawY += BORDER_SIZE;
    scr_drawRect(0, drawY, scrWidth, FRAME_HEIGHT, black);
    scr_drawRect(0, drawY+FRAME_HEIGHT, scrWidth, BORDER_SIZE, red);

    // Print the error message onto the frame.
    scr_setPenPosition(0, drawY);
    scr_setPenColor(gray);
    scr_print(errorMessage);
    uint64ToHex(code, hexbuf+2);

    // We print the exception's message, or "Unknown" if there's no predefined message for the exception code.
    if (code >= (sizeof(exceptionMessages)/sizeof(exceptionMessages[0])) || exceptionMessages[code] == 0) {
        scr_print("Unknown");
    } else {
        scr_print(exceptionMessages[code]);
    }

    // We print the exception's code.
    scr_print(" (code 0x");
    scr_print(hexbuf+13);
    scr_print(")\n\n");

    // We print the RIP and RFLAGS values.
    scr_print(registerNames[16]);
    scr_print(": ");
    uint64ToHex(regdata[16], hexbuf+2);
    scr_print(hexbuf);
    scr_print("   ");
    scr_print(registerNames[17]);
    scr_print(": ");
    uint64ToHex(regdata[17], hexbuf+2);
    scr_println(hexbuf);

    // We print the value of the other 16 registers.
    for (int i = 0; i < 16; i++) {
        scr_print(registerNames[i]);
        scr_print(": ");
        uint64ToHex(regdata[i], hexbuf+2);
        scr_print(hexbuf);
        if (i % 4 == 3)
            scr_printNewline();
        else
            scr_print("   ");
    }

    // We wait for 3 seconds to prevent the user from accidentally exiting before seeing the error.
    uint64_t start_ms = rtc_getElapsedMilliseconds();
    do { _hlt(); } while (rtc_getElapsedMilliseconds() - start_ms < 3000);

    kbd_clearBuffer();
    scr_print("\nPress any key to restart.");

    // We wait until any key is pressed.
    do { _hlt(); } while (kbd_getBufferLength() == 0);

    // We reset everything and load the kernel back up.
    kbd_clearBuffer();
    scr_clear();
    _cli();
    resetmain();
}
