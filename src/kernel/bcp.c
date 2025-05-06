#include "bcp.h"
#include <stdio.h>
#include <string.h>

static BCP process_table[MAX_PROCESSES];
static int next_pid = 1;

void bcp_init() {
    for (int i = 0; i < MAX_PROCESSES; i++) {
        process_table[i].pid = 0;
    }
}

int bcp_create(const char *name, int segment_id, int priority, int exec_time) {
    for (int i = 0; i < MAX_PROCESSES; i++) {
        if (process_table[i].state == FINISHED || process_table[i].pid == 0 ) {
            process_table[i].pid = next_pid++;
            strncpy(process_table[i].name, name, sizeof(process_table[i].name) - 1);
            process_table[i].name[sizeof(process_table[i].name) - 1] = '\0';
            process_table[i].segment_id = segment_id;
            process_table[i].priority = priority;
            process_table[i].exec_time_total = exec_time;
            process_table[i].exec_time_remaining = exec_time;
            process_table[i].io_count = 0;
            process_table[i].num_pages = 0;
            process_table[i].state = READY;
            process_table[i].program_counter = 0;
            process_table[i].waiting_io = false;
            process_table[i].command_count = 0;
            return process_table[i].pid;
        }
    }
    printf("[BCP] Erro: tabela de processos cheia.\n");
    return -1;
}

BCP* bcp_get(int pid) {
    for (int i = 0; i < MAX_PROCESSES; i++) {
        if (process_table[i].pid == pid) return &process_table[i];
    }
    return NULL;
}

void bcp_finish(int pid) {
    BCP* proc = bcp_get(pid);
    if (proc) {
        proc->state = FINISHED;
        proc->end_time = proc->exec_time_total;
    }
}

void bcp_set_state(int pid, ProcessState state) {
    BCP* proc = bcp_get(pid);
    if (proc) {
        proc->state = state;
    }
}

void bcp_increment_io(int pid) {
    BCP* proc = bcp_get(pid);
    if (proc) {
        proc->io_count++;
    }
}

int bcp_get_highest_priority_pid() {
    int chosen_pid = -1;
    int max_io = -1;

    for (int i = 0; i < MAX_PROCESSES; i++) {
        if (process_table[i].pid != 0 && process_table[i].state == READY) {
            if (process_table[i].io_count > max_io ||
               (process_table[i].io_count == max_io &&
                (chosen_pid == -1 || process_table[i].pid < chosen_pid))) {
                max_io = process_table[i].io_count;
                chosen_pid = process_table[i].pid;
            }
        }
    }
    return chosen_pid;
}

void bcp_print_all() {
    printf("\n=== Lista de Processos ===\n");
    printf("%-5s %-10s %-5s %-10s %-10s %-10s\n", "PID", "Name", "Prio", "IO_Count", "State", "Time_Left");
    for (int i = 0; i < MAX_PROCESSES; i++) {
        if (process_table[i].pid > 0) {
            printf("%-5d %-10s %-5d %-10d %-10d %-10d\n",
                   process_table[i].pid,
                   process_table[i].name,
                   process_table[i].priority,
                   process_table[i].io_count,
                   process_table[i].state,
                   process_table[i].exec_time_remaining);
        }
    }
}