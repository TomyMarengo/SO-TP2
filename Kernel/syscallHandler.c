#include <defs.h>
#include <interrupts.h>
#include <keyboard.h>
#include <memoryManager.h>
#include <stdint.h>
#include <time.h>
#include <video.h>
#include <scheduler.h>
#include <pipe.h>
#include <defs.h>

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
/*
static uint64_t
sys_write_handler(uint64_t fd, const char* buff, uint64_t count) {
    return prc_handleWriteFd(sch_getCurrentPID(), fd, buff, count);
}*/

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

/*ssize_t sys_read_handler(int fd, char* buffer, size_t count) {
    return prc_handleReadFd(sch_getCurrentPID(), fd, buffer, count);
}*/

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
    return mm_malloc(size);
}

int
sys_free_handler(void *ptr) {
    return mm_free(ptr);
}

void *
sys_realloc_handler(void *ptr, size_t size) {
    return mm_realloc(ptr, size);
}

int 
sys_close_handler(int fd) {
    return prc_deleteFd(sch_getCurrentPID(), fd);
}

int 
sys_pipe_handler(int pipefd[2]) {
    Pid pid = sch_getCurrentPID();

    Pipe pipe;
    int readFd = -1, writeFd = -1;

    if ((pipe = pipe_create()) == NULL || (readFd = pipe_addFd(pid, -1, pipe, 1, 0)) < 0
        || (writeFd = pipe_addFd(pid, -1, pipe, 0, 1)) < 0) {
        if (pipe != NULL)
            pipe_free(pipe);
        if (readFd >= 0)
            prc_deleteFd(pid, readFd);
        return 1;
    }

    pipefd[0] = readFd;
    pipefd[1] = writeFd;
    return 0;
}

int sys_kill_handler(Pid pid){
    int result = prc_kill(pid);
    if (pid == sch_getCurrentPID())
        sch_yield();
    return result;
}

int sys_block_handler(Pid pid){
    int result = sch_blockProcess(pid);
    if (pid == sch_getCurrentPID())
        sch_yield();
    return result;
}

int sys_unblock_handler(Pid pid){
    return sch_unblockProcess(pid);
}

Pid sys_createProcess_handler(ProcessStart start, int argc, const char* argv[]){
    Pid pid = prc_create(start, argc, argv);

    Color gray = {0x90, 0x90, 0x90};
    Color red = {0x00, 0x00, 0xFF};

    // COMPLETE
    if (pid >= 0) {
        kbd_addFd(pid, STDIN);      
        scr_addFd(pid, STDOUT, &gray); 
        scr_addFd(pid, STDERR, &red);
    }
    return pid;
}

void sys_yield_handler(){
    sch_yield();
}

Pid sys_getCurrentPid_handler(){
    return sch_getCurrentPID();
}

int sys_setPriority_handler(Pid pid, Priority priority){
    return sch_setPriority(pid, priority);
}

int sys_exit_handler() {
    prc_kill(sch_getCurrentPID());
    sch_yield();
    return 1;
}

// These function pointer casts are safe, provided all syscall handler functions take up to 5 parameters and they
// are all of integer type.
static uint64_t (*syscall_handlers[])(uint64_t rdi, uint64_t rsi, uint64_t rdx, uint64_t r10, uint64_t r8) = {
    sys_read_handler,           // 0
    sys_write_handler,          // 1
    sys_time_handler,           // 2
    sys_millis_handler,         // 3
    sys_clearscreen_handler,    // 4
    sys_writeat_handler,        // 5
    sys_screensize_handler,     // 6
    sys_pollread_handler,       // 7
    sys_drawpoint_handler,      // 8
    sys_drawrect_handler,       // 9
    sys_drawline_handler,       // 10
    sys_date_handler,           // 11
    sys_inforeg_handler,        // 12
    sys_malloc_handler,         // 13
    sys_free_handler,           // 14
    sys_realloc_handler,        // 15
    sys_close_handler,          // 16
    sys_pipe_handler,           // 17
    sys_kill_handler,           // 18
    sys_block_handler,          // 19
    sys_unblock_handler,        // 20
    sys_createProcess_handler,  // 21
    sys_yield_handler,          // 22
    sys_getCurrentPid_handler,  // 23
    sys_setPriority_handler,    // 24
    sys_exit_handler            // 25
};

uint64_t
syscallHandler(uint64_t rdi, uint64_t rsi, uint64_t rdx, uint64_t r10, uint64_t r8, uint64_t rax) {
    if (rax < (sizeof(syscall_handlers) / sizeof(syscall_handlers[0])) && syscall_handlers[rax] != 0)
        return syscall_handlers[rax](rdi, rsi, rdx, r10, r8);
    return 0;
}
