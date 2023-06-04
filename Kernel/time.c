#include <defs.h>
#include <time.h>
#include <lib.h>

#define SECONDS 0x00
#define MINUTES 0x02
#define HOURS 0x04
#define DAY 0x07
#define MONTH 0x08
#define YEAR 0x09

extern uint8_t readValue(uint8_t mode);

static unsigned long ticks = 0;

void interruptHandlerRTC() {
    ticks++;
}

unsigned long getElapsedTicks() {
    return ticks;
}

unsigned long getElapsedSeconds() {
    return TICKS_TO_SECONDS(ticks);
}

uint8_t getCurrentSeconds() {
    return bcdToDec(readValue(SECONDS));
}

uint8_t getCurrentMinutes() {
    return bcdToDec(readValue(MINUTES));
}

uint8_t getCurrentHours() {
    return bcdToDec(readValue(HOURS));
}

uint8_t getCurrentDay() {
    return bcdToDec(readValue(DAY));
}

uint8_t getCurrentMonth() {
    return bcdToDec(readValue(MONTH));
}

uint8_t getCurrentYear() {
    return bcdToDec(readValue(YEAR));
}

void getCurrentDateString(char* buffer) {
    char date[] = {'0', '0', '/', '0', '0', '/', '0', '0', 0};

    int t = getCurrentDay();
    date[0] += t / 10;
    date[1] += t % 10;
    t = getCurrentMonth();
    date[3] += t / 10;
    date[4] += t % 10;
    t = getCurrentYear();
    date[6] += t / 10;
    date[7] += t % 10;

    int i;
    for (i = 0; date[i]; i++) {
        buffer[i] = date[i];
    }
    buffer[i] = 0;
}

void getCurrentTimeString(char* buffer) {
    char time[] = {'0', '0', ':', '0', '0', ':', '0', '0', 0};

    int t = getCurrentHours();
    time[0] += t / 10;
    time[1] += t % 10;
    t = getCurrentMinutes();
    time[3] += t / 10;
    time[4] += t % 10;
    t = getCurrentSeconds();
    time[6] += t / 10;
    time[7] += t % 10;

    int i;
    for (i = 0; time[i]; i++) {
        buffer[i] = time[i];
    }
    buffer[i] = 0;
}