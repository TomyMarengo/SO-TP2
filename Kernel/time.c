#include <stdint.h>
#include <time.h>

extern uint8_t rtc_getCurrentHours();
extern uint8_t rtc_getCurrentMinutes();
extern uint8_t rtc_getCurrentSeconds();
extern uint8_t rtc_getCurrentDay();
extern uint8_t rtc_getCurrentMonth();
extern uint8_t rtc_getCurrentYear();

static uint64_t ticks;

void timerIntHandler(void) {
    ticks++;
}

uint64_t rtc_getElapsedTicks() {
    return ticks;
}

uint64_t rtc_getElapsedSeconds() {
    return TICKS_TO_SECONDS(ticks);
}

uint64_t rtc_getElapsedMilliseconds() {
    return TICKS_TO_MILLISECONDS(ticks);
}

uint64_t rtc_getCurrentTime() {
    return rtc_getCurrentHours() | ((uint64_t)rtc_getCurrentMinutes() << 8) | ((uint64_t)rtc_getCurrentSeconds() << 16);
}

uint64_t rtc_getCurrentDate() {
    return rtc_getCurrentDay() | ((uint64_t)rtc_getCurrentMonth() << 8) | ((uint64_t)rtc_getCurrentYear() << 16);
}
