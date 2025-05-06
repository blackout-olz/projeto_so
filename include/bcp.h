#ifndef BCP_H
#define BCP_H

#include <stdbool.h>
#include "interpreter.h"

#define MAX_PAGES 16
#define MAX_COMMANDS 128
#define MAX_PROCESSES 128

// Estados possíveis do processo
typedef enum {
    READY,
    RUNNING,
    BLOCKED,
    FINISHED
} ProcessState;

// Estrutura do Bloco de Controle de Processo
typedef struct {
    int pid;
    char name[64];
    int segment_id;
    int priority;
    int io_count;
    int exec_time_total;
    int exec_time_remaining;
    int memory_pages[MAX_PAGES];
    int num_pages;
    ProcessState state;
    int program_counter;
    int start_time;
    int end_time;
    bool waiting_io;

    ProgramCommand commands[MAX_COMMANDS];
    int command_count;
} BCP;

void bcp_init();
int bcp_create(const char *name, int segment_id, int priority, int exec_time);
BCP* bcp_get(int pid);
void bcp_finish(int pid);
void bcp_set_state(int pid, ProcessState state);
void bcp_increment_io(int pid);
int bcp_get_highest_priority_pid();
void bcp_print_all();

#endif // BCP_H