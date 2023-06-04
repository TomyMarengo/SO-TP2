#ifndef __PHYLO_H__
#define __PHYLO_H__

/* Local headers */
#include <defs.h>

/* Constants */
#define PHYLO_PRIORITY PRIORITY_DEFAULT

#define MAX_PHYLOSOPHERS 10
#define MIN_PHYLOSOPHERS 5
#define MAX_FORKS MAX_PHYLOSOPHERS
#define MIN_FORKS MIN_PHYLOSOPHERS

#define FORKS_SEM_NAME "semFORKS"

#define ADD 'a'
#define REMOVE 'r'
#define QUIT 'q'

#define EATING_TIME_MIN 2000
#define EATING_TIME_MAX 5000
#define THINKING_TIME_MIN 2000
#define THINKING_TIME_MAX 5000
#define SLEEPING_TIME_MIN 2000
#define SLEEPING_TIME_MAX 5000

typedef enum {EATING = 0, WAITINGTOEAT, SLEEPING, THINKING, DEAD} PhyloState;

typedef struct phylo {
    char* phyloName;
    PhyloState phyloState;
    Pid phyloPid;
} Phylo;

/**
 * @brief Initialize phylos program 
 */
void startPhylo(int argc, char * argv[]);

#endif