#include "semaphore.h"
#include "bcp.h"
#include <string.h>
#include <stdio.h>

static Semaphore semaphores[MAX_SEMAPHORES];
static int semaphore_count = 0;

void semaphore_init() {
    semaphore_count = 0;
    for (int i = 0; i < MAX_SEMAPHORES; i++) {
        semaphores[i].id[0] = '\0';
        semaphores[i].value = 1;
        semaphores[i].waiting_count = 0;
    }
}

static Semaphore* get_semaphore(const char *id) {
    for (int i = 0; i < semaphore_count; i++) {
        if (strcmp(semaphores[i].id, id) == 0) return &semaphores[i];
    }
    if (semaphore_count < MAX_SEMAPHORES) {
        Semaphore *sem = &semaphores[semaphore_count++];
        strncpy(sem->id, id, sizeof(sem->id));
        sem->value = 1;
        sem->waiting_count = 0;
        return sem;
    }
    return NULL;
}

void semaphore_P(int pid, const char *sem_id) {
    Semaphore *sem = get_semaphore(sem_id);
    if (!sem) return;

    sem->value--;
    printf("[DEBUG] Entrou em P(%s) - valor após -- = %d\n", sem_id, sem->value);

    if (sem->value < 0) {
        if (sem->waiting_count < MAX_WAITING_PIDS) {
            sem->waiting_pids[sem->waiting_count++] = pid;
            bcp_set_state(pid, BLOCKED);
            printf("[SEM] PID %d bloqueado em P(%s)\n", pid, sem_id);
        }
    } else {
        printf("[SEM] P(%s) liberado para PID %d\n", sem_id, pid);
    }
}

void semaphore_V(const char *sem_id) {
    Semaphore *sem = get_semaphore(sem_id);
    if (!sem) return;

    if (sem->waiting_count > 0) {
        int pid = sem->waiting_pids[0];
        for (int i = 1; i < sem->waiting_count; i++) {
            sem->waiting_pids[i - 1] = sem->waiting_pids[i];
        }
        sem->waiting_count--;
        bcp_set_state(pid, READY);
        printf("[SEM] PID %d desbloqueado por V(%s)\n", pid, sem_id);
    } else {
        sem->value++;
        printf("[SEM] V(%s) incrementa valor para %d\n", sem_id, sem->value);
    }
}
