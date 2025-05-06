#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "kernel.h"
#include "bcp.h"
#include "interpreter.h"
#include "memory.h"
#include "scheduler.h"
#include "clock.h"
#include "semaphore.h"

void show_menu() {
    printf("\n=== SIMULADOR DE NÚCLEO DE SO ===\n");
    printf("1. Carregar programa sintético\n");
    printf("2. Listar processos\n");
    printf("3. Executar ciclo de evento\n");
    printf("4. Visualizar estado da memória\n");
    printf("5. Exibir tempo do relógio virtual\n");
    printf("6. Forçar liberação de um semáforo (V)\n");
    printf("0. Sair\n");
    printf("Escolha: ");
}

int main() {
    kernel_init();
    int opcao;
    char filepath[128];

    while (1) {
        show_menu();
        if (scanf("%d", &opcao) != 1) break;
        getchar();

        switch (opcao) {
            case 1:
                printf("Digite o caminho do programa sintético (.txt): ");
                fgets(filepath, sizeof(filepath), stdin);
                filepath[strcspn(filepath, "\n")] = 0;
                if (interpreter_load_program(filepath) >= 0) {
                    printf("[OK] Programa carregado com sucesso.\n");
                } else {
                    printf("[ERRO] Falha ao carregar programa.\n");
                }
                break;

            case 2:
                bcp_print_all();
                break;

            case 3: {
                int curr = scheduler_get_current();
                if (curr == -1) {
                    scheduler_select_next();
                    curr = scheduler_get_current();
                }

                if (curr != -1) {
                    kernel_handle_event(EVENT_PROCESS_INTERRUPT);
                } else {
                    int event = interruptControl();
                    if (event == -1) {
                        printf("[INFO] Nenhum evento a processar.\n");
                    } else {
                        printf("[INFO] Evento %d recebido.\n", event);
                        kernel_handle_event(event);
                    }
                }
                break;
            }

            case 4:
                memory_print();
                break;

            case 5:
                printf("[CLOCK] Tempo atual do relógio virtual: %d unidades\n", clock_get_time());
                break;

            case 6: {
                char sem_id[16];
                printf("Digite o nome do semáforo a liberar: ");
                fgets(sem_id, sizeof(sem_id), stdin);
                sem_id[strcspn(sem_id, "\n")] = 0;
                semaphore_V(sem_id);
                break;
            }

            case 0:
                printf("Encerrando simulador...\n");
                exit(0);

            default:
                printf("[ERRO] Opção inválida.\n");
        }
    }

    return 0;
}
