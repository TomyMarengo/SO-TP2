#include <defs.h>
#include <fonts.h>
#include <graphics.h>
#include <lib.h>
#include <process.h>

// Retrieved from https://wiki.osdev.org/VESA_Video_Modes
struct vbe_mode_info_structure {
    uint16_t attributes;
    uint8_t window_a;
    uint8_t window_b;
    uint16_t granularity;
    uint16_t window_size;
    uint16_t segment_a;
    uint16_t segment_b;
    uint32_t win_func_ptr;
    uint16_t pitch;
    uint16_t width;
    uint16_t height;
    uint8_t w_char;
    uint8_t y_char;
    uint8_t planes;
    uint8_t bpp;
    uint8_t banks;
    uint8_t memory_model;
    uint8_t bank_size;
    uint8_t image_pages;
    uint8_t reserved0;

    uint8_t red_mask;
    uint8_t red_position;
    uint8_t green_mask;
    uint8_t green_position;
    uint8_t blue_mask;
    uint8_t blue_position;
    uint8_t reserved_mask;
    uint8_t reserved_position;
    uint8_t direct_color_attributes;

    uint32_t framebuffer;
    uint32_t off_screen_mem_off;
    uint16_t off_screen_mem_size;
    uint8_t reserved1[206];
} __attribute__((packed));

const Color RED = {0xFF, 0x00, 0x00};
const Color WHITE = {0xFF, 0xFF, 0xFF};
const Color BLACK = {0x00, 0x00, 0x00};

static uint8_t current_i, current_j;
static uint8_t width, height;

static const struct vbe_mode_info_structure *graphicModeInfo = (struct vbe_mode_info_structure *) 0x5C00;

static void getNextPosition();
static void scrollIfNecessary();

static ssize_t writeHandler(Pid pid, int fd, void *resource, const char *buf, size_t count);
static int dupHandler(Pid pidFrom, Pid pidTo, int fdFrom, int fdTo, void *resource);

static void *
getPixelAddress(int i, int j) {
    return (void *) ((size_t) graphicModeInfo->framebuffer + 3 * (graphicModeInfo->width * i + j));
}

static void
drawPixel(int i, int j, const Color *color) {
    uint8_t *pixel = getPixelAddress(i, j);
    pixel[0] = color->B;
    pixel[1] = color->G;
    pixel[2] = color->R;
}

void
initializeScreen() {
    current_i = 0;
    current_j = 0;
    width = graphicModeInfo->width / CHAR_WIDTH;
    height = graphicModeInfo->height / CHAR_HEIGHT;
    clearScreen();
}

void
printCharFormat(char character, const Color *charColor, const Color *bgColor) {
    if (character == '\b') {
        if (current_j == 0) {
            current_i -= 1;
            current_j = width - 1;
            printCharFormat(' ', charColor, bgColor);
            current_i -= 1;
            current_j = width - 1;
        } else {
            current_j = (current_j - 1) % width;
            printCharFormat(' ', charColor, bgColor);
            current_j = (current_j - 1) % width;
        }
        return;
    }

    scrollIfNecessary();

    if (character == '\n') {
        printLine();
        return;
    }

    uint8_t *characterPtr = getCharMapping(character);

    uint16_t write_i = (current_i) *CHAR_HEIGHT;
    uint16_t write_j = (current_j) *CHAR_WIDTH;

    uint8_t mask;

    for (int i = 0; i < CHAR_HEIGHT; ++i) {
        for (int j = 0; j < CHAR_WIDTH; ++j) {
            mask = 1 << (CHAR_WIDTH - j - 1);
            if (characterPtr[i] & mask) {
                drawPixel(write_i + i, write_j + j, charColor);
            } else {
                drawPixel(write_i + i, write_j + j, bgColor);
            }
        }
    }
    getNextPosition();
}

void
printChar(char character) {
    printCharFormat(character, &WHITE, &BLACK);
}

void
print(const char *string) {
    for (int i = 0; string[i] != 0; ++i) {
        printChar(string[i]);
    }
}

void
printLine() {
    current_j = 0;
    current_i += 1;
}

void
restartCursor() {
    current_i = 0;
    current_j = 0;
}

void
clearScreen() {
    void *p = (void *) (size_t) graphicModeInfo->framebuffer;
    memset(p, 0, (size_t) graphicModeInfo->width * graphicModeInfo->height * 3);
    current_i = 0;
    current_j = 0;
}

void
printDec(uint64_t number) {
    printBase(number, 10);
}

void
printHex(uint64_t number) {
    printBase(number, 16);
}

void
printBin(uint64_t number) {
    printBase(number, 2);
}

void
printBase(uint64_t number, uint32_t base) {
    static char buffer[65] = {'\0'};
    uintToBase(number, buffer, base);
    print(buffer);
}

void
printRegisterFormat(uint64_t regis) {

    uint64_t aux = regis;
    uint64_t count = 16;

    while (aux) {
        aux = aux >> 4;
        --count;
    }

    for (int i = 0; i < count; i++) {
        printChar('0');
    }

    if (regis) {
        printHex(regis);
    }
}

static void
getNextPosition() {
    current_i += ((current_j + 1) == width) ? 1 : 0;
    current_j = (current_j + 1) % width;
}

static void
scrollIfNecessary() {
    if (current_i < height)
        return;

    void *dst = (void *) ((size_t) graphicModeInfo->framebuffer);
    void *src = (void *) (dst + 3 * (CHAR_HEIGHT * (size_t) graphicModeInfo->width));
    size_t len = 3 * ((size_t) graphicModeInfo->width * (graphicModeInfo->height - CHAR_HEIGHT));
    memcpy(dst, src, len);
    memset(dst + len, 0, 3 * (size_t) graphicModeInfo->width * CHAR_HEIGHT);

    current_i--;
}

int
addFdScreen(Pid pid, int fd, const Color *color) {
    uint64_t col = color->R | (color->G << 8) | (color->B << 16) | (1 << sizeof(Color));
    return addFd(pid, fd, (void *) col, NULL, &writeHandler, NULL, &dupHandler);
}

static ssize_t
writeHandler(Pid pid, int fd, void *resource, const char *buf, size_t count) {
    if (!isForeground(pid))
        return -1;

    for (size_t i = 0; i < count; i++)
        printCharFormat(buf[i], (const Color *) &resource, &BLACK);

    return count;
}

static int
dupHandler(Pid pidFrom, Pid pidTo, int fdFrom, int fdTo, void *resource) {
    return addFdScreen(pidTo, fdTo, (const Color *) &resource);
}