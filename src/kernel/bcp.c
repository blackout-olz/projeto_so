#include "../../include/bcp.h"
#include <stdio.h>

#define MAX_PROCESSES 10

// Vetor que armazena os processos simulados
BCP processes[MAX_PROCESSES];

void bcp_init() {
    for (int i = 0; i < MAX_PROCESSES; i++) {
        processes[i].pid = -1;
        processes[i].state = FINISHED;
    }
}

// Cria um novo processo e o insere na lista de processos
void bcp_createProcess(int pid, int priority) {
    if (pid < 0 || pid >= MAX_PROCESSES) {
        printf("[BCP] Invalid PID!\n");
        return;
    }

    processes[pid].pid = pid;
    processes[pid].priority = priority;
    processes[pid].state = READY;
    processes[pid].remainingTime = 0; // Pode ser ajustado depois, dependendo do scheduler

    printf("[BCP] Process %d created with priority %d.\n", pid, priority);
}

// Bloqueia o processo
void bcp_blockProcess(int pid) {
    if (pid < 0 || pid >= MAX_PROCESSES || processes[pid].pid == -1) {
        printf("[BCP] Cannot block process %d (invalid PID or non-existent).\n", pid);
        return;
    }

    processes[pid].state = BLOCKED;
    printf("[BCP] Process %d blocked.\n", pid);
}

// Desbloqueia o processo
void bcp_unblockProcess(int pid) {
    if (pid < 0 || pid >= MAX_PROCESSES || processes[pid].pid == -1) {
        printf("[BCP] Cannot unblock process %d (invalid PID or non-existent).\n", pid);
        return;
    }

    processes[pid].state = READY;
    printf("[BCP] Process %d unblocked.\n", pid);
}

// Finaliza o processo
void bcp_finalizeProcess(int pid) {
    if (pid < 0 || pid >= MAX_PROCESSES || processes[pid].pid == -1) {
        printf("[BCP] Cannot finalize process %d (invalid PID or non-existent).\n", pid);
        return;
    }

    processes[pid].state = FINISHED;
    printf("[BCP] Process %d finished.\n", pid);
}

// Retorna o estado atual do processo
int bcp_getState(int pid) {
    if (pid < 0 || pid >= MAX_PROCESSES || processes[pid].pid == -1) {
        printf("[BCP] Invalid PID %d for state retrieval.\n", pid);
        return -1; // Código de erro para estado inválido
    }

    return processes[pid].state;
}

// Exibe todos os processos existentes (para debug)
void bcp_showProcesses() {
    printf("PID\tSTATE\t\tPRIORITY\tREMAINING TIME\n");
    for (int i = 0; i < MAX_PROCESSES; i++) {
        if (processes[i].pid != -1) {
            char* stateStr;

            switch (processes[i].state) {
                case READY: stateStr = "READY"; break;
                case RUNNING: stateStr = "RUNNING"; break;
                case BLOCKED: stateStr = "BLOCKED"; break;
                case FINISHED: stateStr = "FINISHED"; break;
                default: stateStr = "UNKNOWN"; break;
            }

            printf("%d\t%s\t%d\t\t%d\n",
                   processes[i].pid,
                   stateStr,
                   processes[i].priority,
                   processes[i].remainingTime);
        }
    }
}
