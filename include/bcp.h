#ifndef BCP_H
#define BCP_H

typedef enum {
    READY,
    RUNNING,
    BLOCKED,
    FINISHED
} ProcessState;

typedef struct {
    int pid;
    int priority;
    ProcessState state;
    int remainingTime; // A depender do scheduler
} BCP;

// interface
void bcp_init();
void bcp_createProcess(int pid, int prioridade);
void bcp_blockProcess(int pid);
void bcp_unblockProcess(int pid);
void bcp_finalizeProcess(int pid);
int bcp_getState(int pid);
void bcp_showProcesses();  // Para debug

#endif /* BPC_H */