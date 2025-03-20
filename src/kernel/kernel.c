#include "kernel.h"
#include "bcp.h"
#include <stdio.h>

// Inicializa o kernel e seus componentes
void initKernel() {
    printf("[Kernel] Initializing kernel...\n");
    bcp_init();
}

// Recebe chamadas de sistema feitas pelos programas (syscalls)
void sysCall(int eventCode, int pid) {
    printf("[Kernel] sysCall - Event %d from process %d\n", eventCode, pid);

    switch (eventCode) {
        case 2: // PROCESS_CREATE
            // Exemplo: prioridade fixa em 1 no início
            bcp_createProcess(pid, 1);
            break;

        case 3: // PROCESS_FINISH
            bcp_finalizeProcess(pid);
            break;

        case 4: // DISK_REQUEST (exemplo de bloqueio)
            bcp_blockProcess(pid);
            printf("[Kernel] Process %d requested disk access and was blocked.\n", pid);
            break;

        default:
            printf("[Kernel] Unknown syscall event: %d\n", eventCode);
            break;
    }
}

// Recebe interrupções externas
void interruptControl(int eventCode, int pid) {
    printf("[Kernel] interruptControl - Event %d from process %d\n", eventCode, pid);

    switch (eventCode) {
        case 5: // DISK_FINISH
            bcp_unblockProcess(pid);
            printf("[Kernel] Process %d disk operation finished and was unblocked.\n", pid);
            break;

        default:
            printf("[Kernel] Unknown interrupt event: %d\n", eventCode);
            break;
    }
}
