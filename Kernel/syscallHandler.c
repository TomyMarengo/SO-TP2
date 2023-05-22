#include <defs.h>
#include <interrupts.h>
#include <keyboard.h>
#include <memoryManager.h>
#include <stdint.h>
#include <time.h>
#include <video.h>

extern uint8_t hasRegdump;
extern const uint64_t regdump[17];

static uint64_t
sys_write_handler(uint64_t fd, const char *buf, uint64_t count) {
    if (fd != STDOUT)  // Ignore any file handle that isn't STDOUT
        return 0;

    for (int i = 0; i < count; i++)
        scr_printChar(buf[i]);
    return count;
}

static uint64_t
sys_time_handler() {
    return rtc_getCurrentTime();
}

static uint64_t
sys_date_handler() {
    return rtc_getCurrentDate();
}

static uint64_t
sys_millis_handler() {
    return rtc_getElapsedMilliseconds();
}

static void
sys_clearscreen_handler() {
    scr_clear();
}

static uint32_t
sys_writeat_handler(const char *buf, uint64_t count, uint16_t x, uint16_t y, Color color) {
    scr_setPenPosition(x, y);
    scr_setPenColor(color);
    for (int i = 0; i < count; i++)
        scr_printChar(buf[i]);
    return scr_getPenX() | ((uint32_t) scr_getPenY() << 16);
}

static uint64_t
sys_screensize_handler() {
    return scr_getWidth() | ((uint64_t) scr_getHeight() << 32);
}

static uint64_t
sys_pollread_handler(uint64_t fd, char *buf, uint64_t count, uint64_t timeout_ms) {
    // Any file descriptor that isnt STDIN or KBDIN gets ignored
    if (fd != STDIN && fd != KBDIN)
        return 0;

    // We do an initial read of the available characters
    unsigned int totalRead = (fd == STDIN ? kbd_readCharacters(buf, count) : kbd_readScancodes((uint8_t *) buf, count));

    if (timeout_ms != 0) {
        // We block until data was read or the timeout expires
        uint64_t start_ms = rtc_getElapsedMilliseconds();
        do {
            _hlt();
            totalRead += (fd == STDIN ? kbd_readCharacters(buf + totalRead, count - totalRead)
                                      : kbd_readScancodes((uint8_t *) buf + totalRead, count - totalRead));
        } while (totalRead == 0 && (rtc_getElapsedMilliseconds() - start_ms) < timeout_ms);
    }

    return totalRead;
}

static uint64_t
sys_read_handler(uint64_t fd, char *buf, uint64_t count) {
    return sys_pollread_handler(fd, buf, count, 0xFFFFFFFFFFFFFFFF);
}

static void
sys_drawpoint_handler(uint16_t x, uint16_t y, Color color) {
    scr_setPixel(x, y, color);
}

static void
sys_drawrect_handler(uint16_t x, uint16_t y, uint16_t width, uint16_t height, Color color) {
    scr_drawRect(x, y, width, height, color);
}

static void
sys_drawline_handler(uint16_t fromX, uint16_t fromY, uint16_t toX, uint16_t toY, Color color) {
    scr_drawLine(fromX, fromY, toX, toY, color);
}

static uint8_t
sys_inforeg_handler(uint64_t reg[17]) {
    if (hasRegdump) {
        for (uint8_t i = 0; i < 17; i++) {
            reg[i] = regdump[i];
        }
    }
    return hasRegdump;
}

void *
sys_malloc_handler(size_t size) {
    return my_malloc(size);
}

int
sys_free_handler(void *ptr) {
    return my_free(ptr);
}

// These function pointer casts are safe, provided all syscall handler functions take up to 5 parameters and they
// are all of integer type.
static uint64_t (*syscall_handlers[])(uint64_t rdi, uint64_t rsi, uint64_t rdx, uint64_t r10, uint64_t r8) = {
    sys_read_handler,         // 0
    sys_write_handler,        // 1
    sys_time_handler,         // 2
    sys_millis_handler,       // 3
    sys_clearscreen_handler,  // 4
    sys_writeat_handler,      // 5
    sys_screensize_handler,   // 6
    sys_pollread_handler,     // 7
    sys_drawpoint_handler,    // 8
    sys_drawrect_handler,     // 9
    sys_drawline_handler,     // 10
    sys_date_handler,         // 11
    sys_inforeg_handler,      // 12
    sys_malloc_handler,       // 13
    sys_free_handler          // 14
};

uint64_t
syscallHandler(uint64_t rdi, uint64_t rsi, uint64_t rdx, uint64_t r10, uint64_t r8, uint64_t rax) {
    if (rax < (sizeof(syscall_handlers) / sizeof(syscall_handlers[0])) && syscall_handlers[rax] != 0)
        return syscall_handlers[rax](rdi, rsi, rdx, r10, r8);
    return 0;
}
