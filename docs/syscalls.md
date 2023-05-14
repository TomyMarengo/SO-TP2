# Syscall documentation

## [0x00] uint64_t sys_read(uint64_t fd, char* buf, uint64_t count);
Reads up to `count` bytes from a file descriptor. If no data is available to read at the moment, blocks until there is. Returns the amount of bytes read.

## [0x01] uint64_t sys_write(uint64_t fd, const char* buf, uint64_t count);
Writes up to `count` bytes to a file descriptor. Returns the amount of bytes written.

## [0x02] uint64_t sys_time();
Returns the current system time in seconds, minutes, and hours. Bits 0-7 represent hours, bits 8-15 minutes and bits 16-23 seconds.

## [0x03] uint64_t sys_millis();
Returns the amount of milliseconds elapsed since system startup.

## [0x04] void sys_clearscreen();
Clears the screen, putting it all to black and resetting the pen to the top left.

## [0x05] uint32_t sys_writeat(const char* buf, uint64_t count, uint16_t x, uint16_t y, Color color);
Writes up to `count` characters into the screen at the specified pixel position with the specified color. Returns the new pen position as a 32 bit number, where the 16 lowest bits are the x and the upper 16 bits are the y.

## [0x06] uint64_t sys_screensize();
Gets the width and height of the screen in pixels. Returns a 64 bit number, whose lower 32 bits are width and upper 32 bits are the height.

## [0x07] uint64_t sys_pollread(uint64_t fd, char* buf, uint64_t count, uint64_t timeout_ms);
Same as `sys_read(...)` but returns prematurely if no data becomes available after the timeout expires.

Passing a timeout of 0 will make `sys_pollread(...)` return immediately even if there is no available data to read. Otherwise, it only ensures at least `timeout_ms` milliseconds have passed.

## [0x08] void sys_drawpoint(uint16_t x, uint16_t y, Color color);
Draws a single pixel onto the screen at the specified position.

## [0x09] void sys_drawrect(uint16_t x, uint16_t y, uint16_t width, uint16_t height, Color color);
Draws a rectangle of pixels onto the screen.

## [0x0A] void sys_drawline(uint16_t fromX, uint16_t fromY, uint16_t toX, uint16_t toY, Color color);
Draws a line between two pixels on the screen.

## [0x0B] uint64_t sys_date();
Returns the current date as a 64-bit number whose bits 0-7 represent day, bits 8-15 month and bits 16-23 year.

# Default file descriptors
- [0] `STDIN`: Standard input
- [1] `STDOUT`: Standard output
- [2] `STDERR`: Standard error
- [3] `KBDIN` When `STDIN` brings data from the keyboard, this brings the same data but raw. `KBDIN` brings raw scancodes while `STDIN` filters them and turns them into ASCII characters.
