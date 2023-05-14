#include <syscalls.h>
#include <timeUtil.h>
#include <color.h>
extern uint32_t width, height;

#define POSX (3*width/4-5*CHAR_WIDTH)
#define POSY (height/6-CHAR_HEIGHT/2)
#define WIDTH (TIME_WITH_TENTHS_LENGTH*CHAR_WIDTH)
#define HEIGHT CHAR_HEIGHT
#define LENGTH (TIME_WITH_TENTHS_LENGTH+1)

static uint8_t isRunning = 0;
static uint64_t millisRunning;

void stw_update(uint64_t millis) {
    if(isRunning) {
        char stopwatchBuff[TIME_WITH_TENTHS_LENGTH+1];
        getTimeWithTenths(stopwatchBuff, millis-millisRunning);
        sys_drawrect(POSX, POSY, WIDTH, HEIGHT, black);
        sys_writeat(stopwatchBuff, LENGTH, POSX, POSY, green);
    }
}

void stw_changeStatus(uint64_t millis) {
    millisRunning = millis-millisRunning;
    if(isRunning) {
        isRunning = 0;   
    }
    else {
        isRunning = 1;
    }  
}

void stw_init() {
    sys_writeat("00:00:00:0", LENGTH, POSX, POSY, green);
    sys_writeat("Press space to stop or resume the timer.", 40, width/2+CHAR_WIDTH, height/3-CHAR_HEIGHT*2, gray);
    sys_writeat("Press tab to restart it.", 24, width/2+CHAR_WIDTH, height/3-CHAR_HEIGHT, gray);
}

void stw_stop() {
    isRunning = 0;
    millisRunning = 0;
    sys_drawrect(POSX, POSY, WIDTH, HEIGHT, black);
    stw_init();
}
