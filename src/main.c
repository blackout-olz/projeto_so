#include "../include/kernel.h"
#include "../include/bcp.h"
#include <stdio.h>
#include <stdlib.h>

// Função para limpar a tela (opcional, para clareza)
void clearScreen() {
    system("clear"); // Linux/Mac
}

int main() {
    int option;
    int pid;

    initKernel();

    while (1) {
        clearScreen(); // opcional, comentar se não quiser limpar a tela

        printf("===== SIMULADOR DE NÚCLEO - MENU PRINCIPAL =====\n");
        printf("1. Create process\n");
        printf("2. Finish process\n");
        printf("3. Request disk (blocks process)\n");
        printf("4. Disk operation finished (unblocks process)\n");
        printf("5. Show process table\n");
        printf("0. Exit\n");
        printf("Option: ");
        scanf("%d", &option);

        switch (option) {
            case 1:
                printf("Enter PID to create (0-9): ");
                scanf("%d", &pid);
                sysCall(2, pid);  // PROCESS_CREATE
                break;

            case 2:
                printf("Enter PID to finish: ");
                scanf("%d", &pid);
                sysCall(3, pid);  // PROCESS_FINISH
                break;

            case 3:
                printf("Enter PID to request disk access: ");
                scanf("%d", &pid);
                sysCall(4, pid);  // DISK_REQUEST (bloqueia)
                break;

            case 4:
                printf("Enter PID that finished disk operation: ");
                scanf("%d", &pid);
                interruptControl(5, pid);  // DISK_FINISH (desbloqueia)
                break;

            case 5:
                bcp_showProcesses();  // exibe todos os processos
                printf("\nPress ENTER to continue...");
                getchar(); getchar(); // aguarda ENTER duas vezes
                break;

            case 0:
                printf("Exiting simulator...\n");
                exit(0);
                break;

            default:
                printf("Invalid option!\n");
                break;
        }
    }

    return 0;
}
