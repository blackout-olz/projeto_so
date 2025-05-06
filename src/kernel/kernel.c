#include "kernel.h"
#include "bcp.h"
#include "scheduler.h"
#include "clock.h"
#include "semaphore.h"
#include "memory.h"
#include <stdio.h>

void execute_next_command(BCP *proc) {
    if (!proc || proc->program_counter >= proc->command_count) {
        bcp_finish(proc->pid);
        memory_release_process(proc->pid);
        scheduler_select_next();
        return;
    }

    ProgramCommand cmd = proc->commands[proc->program_counter];
    printf("[EXEC] PID %d - Comando %d\n", proc->pid, cmd.type);

    switch (cmd.type) {
        case CMD_EXEC: {
            int pages_needed = (cmd.value + 999) / 1000;
            for (int i = 0; i < pages_needed && proc->num_pages < MAX_PAGES; i++) {
                int frame = memory_allocate_page(proc->pid, proc->num_pages);
                if (frame != -1) {
                    proc->memory_pages[proc->num_pages++] = frame;
                }
            }
            clock_advance(cmd.value);
            proc->exec_time_remaining -= cmd.value;
            break;
        }
        case CMD_P:
            semaphore_P(proc->pid, cmd.sem_id);
            if (proc->state == BLOCKED) {
                proc->program_counter++;
                scheduler_select_next();
                return;
            }
            break;
        case CMD_V:
            semaphore_V(cmd.sem_id);
            break;
            
        default:
            break;
    }

    proc->program_counter++;

    if (proc->program_counter >= proc->command_count) {
        bcp_finish(proc->pid);
        memory_release_process(proc->pid);
        scheduler_select_next();
        return;
    }

    int syscall_event = sysCall();
    if (syscall_event >= 0) {
        printf("[KERNEL] sysCall gerou evento %d\n", syscall_event);
        kernel_handle_event(syscall_event);
        return; // syscall pode ter mudado estado
    }

    // Simula preempção: devolve processo para fila
    bcp_set_state(proc->pid, READY);
    scheduler_set_current(-1);
    scheduler_select_next();
}

void kernel_init() {
    bcp_init();
    scheduler_init();
    clock_init();
    semaphore_init();
    memory_init();
}

int interruptControl() {
    return -1;
}

int sysCall() {
    int curr = scheduler_get_current();
    if (curr == -1) return -1;

    BCP *proc = bcp_get(curr);
    if (!proc || proc->program_counter >= proc->command_count) return -1;

    ProgramCommand cmd = proc->commands[proc->program_counter];

    switch (cmd.type) {
        case CMD_P:
            return EVENT_SEMAPHORE_P;
        case CMD_V:
            return EVENT_SEMAPHORE_V;
        default:
            return -1;
    }
}

void kernel_handle_event(int event_code) {
    switch (event_code) {
        case EVENT_PROCESS_INTERRUPT: {
            int curr = scheduler_get_current();
            if (curr != -1) {
                BCP *proc = bcp_get(curr);
                if (proc && proc->state == RUNNING) {
                    execute_next_command(proc);
                }
            } else {
                scheduler_select_next();
            }
            break;
        }
        case EVENT_PROCESS_FINISH: {
            int curr = scheduler_get_current();
            if (curr != -1) {
                bcp_finish(curr);
                memory_release_process(curr);
                printf("[KERNEL] Processo finalizado PID %d\n", curr);
            }
            scheduler_select_next();
            break;
        }
        case EVENT_SEMAPHORE_P:
        case EVENT_SEMAPHORE_V:
            printf("[KERNEL] Syscall de semáforo %d\n", event_code);
            break;
        default:
            printf("[KERNEL] Evento desconhecido (ignorado): %d\n", event_code);
            break;
    }
}