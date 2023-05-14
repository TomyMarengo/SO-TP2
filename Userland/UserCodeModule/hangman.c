#include <hangman.h>
#include <color.h>
#include <string.h>
#include <syscalls.h>
#include <steve.h>

#define CANT_LETTERS ('z'-'a'+1)
extern uint32_t width, height;

static char* words[] = {"aeropuerto","murcielago","zapatilla","velocirraptor","marengo","mizrahi",};
static uint8_t usedLetters[CANT_LETTERS];
static uint8_t wordLength;
static char* word;
static uint8_t lives;
static uint8_t rightLetters;
static uint8_t isRunning;
static uint8_t cantUsedLetters;

#define POSYWORD (5*height/6)
#define POSXWORD ((width/2-(wordLength*2-1)*CHAR_WIDTH)/2)

static void lose() {
    drawSteveClearEyes();
    sys_writeat("You Lose",8,(width/2-8*CHAR_WIDTH)/2, POSYWORD+4*CHAR_HEIGHT, red);
    isRunning = 0;
    for(int i=0; i<wordLength; i++) {
        if(!usedLetters[word[i]-'a'])
            sys_writeat(&word[i], 1, POSXWORD+2*i*CHAR_WIDTH, POSYWORD, red);
    }
}

static void win() {
    sys_writeat("You Win",7,(width/2-7*CHAR_WIDTH)/2, POSYWORD+4*CHAR_HEIGHT, green);
    isRunning = 0;
}

static void (*lives_actions[])(void) = {lose, drawSteveRightLeg, drawSteveLeftLeg, drawSteveRightArm, drawSteveLeftArm, drawSteveTorso, drawSteveHead};

static void printLives() {
    sys_drawrect((width/2-8*CHAR_WIDTH)/2+7*CHAR_WIDTH,POSYWORD+2*CHAR_HEIGHT,CHAR_WIDTH,CHAR_HEIGHT, black);
    sys_writeat("Lives: ",7,(width/2-8*CHAR_WIDTH)/2, POSYWORD+2*CHAR_HEIGHT, green);
    char liv = lives + '0';
    sys_writeat(&liv,1,(width/2-8*CHAR_WIDTH)/2+7*CHAR_WIDTH,POSYWORD+2*CHAR_HEIGHT, green);
}

void hang_init() {
    word = words[sys_millis() % 6];
    wordLength = strlen(word);
    lives = 7;
    rightLetters = 0;
    isRunning = 1;
    cantUsedLetters = 0;
    sys_drawrect(0,height/3+1,width/2-1,2*height/3-1,black);
    drawSteveRope();
    printLives();

    sys_writeat("Press any letter to try to guess.", 33, CHAR_WIDTH, height-CHAR_HEIGHT*2, gray);
    sys_writeat("Once finished, press any letter to start over.", 46, CHAR_WIDTH, height-CHAR_HEIGHT, gray);


    for(int i=0; i<wordLength; i++) {
        sys_writeat("_", 1, POSXWORD+2*i*CHAR_WIDTH, POSYWORD, gray);
    }

    for(int i=0; i<CANT_LETTERS; i++) {
        usedLetters[i] = 0;
    }
}

void hang_update(char letter) {
    if(isRunning) {
        if(!usedLetters[letter-'a']) {
            usedLetters[letter-'a'] = 1;
            sys_writeat(&letter, 1, CHAR_WIDTH, height/3 + CHAR_HEIGHT*(cantUsedLetters+1)+3*CHAR_HEIGHT, magenta);

            cantUsedLetters++;
            uint8_t letterAppears = 0;
            for(uint8_t i=0; i<wordLength; i++) {
                if(word[i]==letter) {
                    sys_writeat(&word[i], 1, POSXWORD+2*i*CHAR_WIDTH, POSYWORD, green);
                    letterAppears++;
                }
            }
            rightLetters += letterAppears;
            if(rightLetters == wordLength)
                win();

            if(!letterAppears) {
                lives--;
                lives_actions[lives]();
                printLives();
            }
        }
    }
    else
        hang_init();
}