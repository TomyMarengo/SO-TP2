#include <defs.h>
#include <phylo.h>
#include <string.h>
#include <syscalls.h>
#include <testUtil.h>
#include <userlib.h>

static char *phyloName[MAX_PHYLOSOPHERS] = {"Aristoteles", "Platon", "Pitagoras", "Heraclito", "TalesDeMileto", "Diogenes"};

static Phylo phylos[MAX_PHYLOSOPHERS];
static int phyloCount = 0;

static int maxPhilosophers = 0;

static Sem forks[MAX_FORKS];
static Sem semPickForks;

static void philosopher(int argc, char *argv[]);
static void waitForKey();

static void think(int phyloIdx);
static void eat(int phyloIdx);
static void phyloSleep(int phyloIdx);

static void addPhylo();
static void removePhylo();
static void addFork();
static void removeFork();

static void terminatePhylos();
static void terminateForks();

static void printState();

static unsigned long int nextRand = 1;

static int
rand(void) {
    nextRand = nextRand * 1103515245 + 12345;
    return (unsigned int) (nextRand / 65536) % 32768;
}

static inline int
getThinkingTime() {
    return THINKING_TIME_MIN + rand() % (THINKING_TIME_MAX - THINKING_TIME_MIN + 1);
}

static inline int
getEatingTime() {
    return EATING_TIME_MIN + rand() % (EATING_TIME_MAX - EATING_TIME_MIN + 1);
}

static inline int
getSleepingTime() {
    return SLEEPING_TIME_MIN + rand() % (SLEEPING_TIME_MAX - SLEEPING_TIME_MIN + 1);
}

void
startPhylo(int argc, char *argv[]) {

    maxPhilosophers = getMaxAvailableProcesses();
    if (maxPhilosophers < MIN_PHYLOSOPHERS) {
        fprintf(STDERR, "phylo: (%d) - Error: Phylo requires a minimum of 6 available processes.\n", sys_getpid());
        return;
    }
    nextRand = (unsigned int) sys_millis();
    printf("5 philosophers are starting to thinking...\n");
    printf("Press %c to quit, %c to add a new philosopher, and %c to remove a philosopher.\n", QUIT, ADD, REMOVE);
    phyloSleep(500);

    for (int i = 0; i < maxPhilosophers; ++i) {
        phylos[i].phyloPid = -1;
        forks[i] = -1;
    }

    for (int i = 0; i < MIN_PHYLOSOPHERS; ++i)
        addPhylo();

    if ((semPickForks = sys_openSem("semPickForks", 1)) == -1) {
        fprint(STDERR, "sys_openSem failed\n");
        terminatePhylos();
        sys_exit();
    }

    waitForKey();
}

static void
waitForKey() {
    while (1) {
        char c = getChar();

        if (c < 0 || c == QUIT) {
            terminatePhylos();
            terminateForks();
            sys_exit();
        } else if (c == ADD) {
            if (phyloCount >= maxPhilosophers)
                printf("\nCan't have more than %d philosophers\n", maxPhilosophers);
            else
                addPhylo();
        } else if (c == REMOVE) {
            if (phyloCount <= MIN_PHYLOSOPHERS)
                printf("\nCan't have less than %d philosophers\n", MIN_PHYLOSOPHERS);
            else
                removePhylo();
        }
    }
}

static void
philosopher(int argc, char *argv[]) {
    int phyloIdx = atoi(argv[0]);

    while (1) {
        think(phyloIdx);
        if (phyloCount <= phyloIdx)
            break;
        eat(phyloIdx);
        if (phyloCount <= phyloIdx)
            break;
        phyloSleep(phyloIdx);
        if (phyloCount <= phyloIdx)
            break;
    }

    phylos[phyloIdx].phyloState = DEAD;
    phylos[phyloIdx].phyloPid = -1;
}

static void
think(int phyloIdx) {
    phylos[phyloIdx].phyloState = THINKING;
    printState();
    phyloSleep(getThinkingTime());
}

static void
eat(int phyloIdx) {
    phylos[phyloIdx].phyloState = WAITING;
    printState();

    int firstForkIdx = phyloIdx;
    int secondForkIdx = (phyloIdx + 1) % phyloCount;

    // Ensure philosophers pick up forks in a specific order
    if (phyloIdx % 2 == 0) {
        sys_wait(forks[firstForkIdx]);   // Left fork
        sys_wait(forks[secondForkIdx]);  // Right fork
    } else {
        sys_wait(forks[secondForkIdx]);  // Right fork
        sys_wait(forks[firstForkIdx]);   // Left fork
    }

    printState();
    phylos[phyloIdx].phyloState = EATING;
    phyloSleep(getEatingTime());

    sys_post(forks[secondForkIdx]);  // Right fork
    sys_post(forks[firstForkIdx]);   // Left fork
}

static void
phyloSleep(int phyloIdx) {
    phylos[phyloIdx].phyloState = SLEEPING;
    printState();
    sleep(getSleepingTime());
}

static void
addPhylo() {
    if (phylos[phyloCount].phyloPid != -1) {
        fprintf(STDERR, "Please wait a bit before trying to add again.\n");
        return;
    }

    addFork();
    phylos[phyloCount].phyloName = phyloName[phyloCount];
    phylos[phyloCount].phyloState = THINKING;

    char idxToString[] = {phyloCount + '0', 0};
    char *auxi[] = {idxToString};
    ProcessCreateInfo phyloContexInfo = {
        .name = phyloName[phyloCount],
        .start = (ProcessStart) philosopher,
        .isForeground = 1,
        .priority = PHYLO_PRIORITY,
        .argc = 1,
        .argv = (const char *const *) auxi,
    };

    phylos[phyloCount].phyloPid = sys_createProcess(-1, -1, -1, &phyloContexInfo);
    phyloCount++;
}

static void
removePhylo() {
    removeFork();
}

static void
addFork() {
    if (forks[phyloCount] >= 0)
        return;

    char aux[] = {phyloCount + '0', 0};
    char semName[strlen(FORKS) + 2];
    strcpy(semName, FORKS);
    strcat(semName, aux);
    forks[phyloCount] = sys_openSem(semName, 1);
    if (forks[phyloCount] < 0) {
        fprint(STDERR, "sys_openSem failed\n");
        terminatePhylos();
        terminateForks();
        sys_exit();
    }
}

static void
removeFork() {
    phyloCount--;
}

static void
terminatePhylos() {
    for (int i = 0; i < phyloCount; ++i)
        sys_kill(phylos[i].phyloPid);
}

static void
terminateForks() {
    for (int i = 0; i < MAX_FORKS && forks[i] >= -1; ++i)
        sys_closeSem(forks[i]);

    sys_closeSem(semPickForks);
}

static void
printState() {
    print("\n");
    for (int i = 0; i < phyloCount; ++i)
        printf("%c ", phylos[i].phyloState == EATING ? 'E' : '.');
}
