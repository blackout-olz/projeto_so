#ifndef SEMAPHORE_H
#define SEMAPHORE_H

#define MAX_SEMAPHORES 32
#define MAX_WAITING_PIDS 64

typedef struct {
    char id[16];
    int value;
    int waiting_pids[MAX_WAITING_PIDS];
    int waiting_count;
} Semaphore;

void semaphore_init();
void semaphore_P(int pid, const char *sem_id);
void semaphore_V(const char *sem_id);

#endif // SEMAPHORE_H