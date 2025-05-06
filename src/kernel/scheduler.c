#include "scheduler.h"
#include "bcp.h"

static int current_pid = -1;

void scheduler_init() {
    current_pid = -1;
}

int scheduler_select_next() {
    int next = bcp_get_highest_priority_pid();
    scheduler_set_current(next);
    if (next != -1) {
        bcp_set_state(next, RUNNING);
    }
    return next;
}

int scheduler_finish_current() {
    if (current_pid != -1) {
        bcp_finish(current_pid);
    }
    return scheduler_select_next();
}

void scheduler_set_current(int pid) {
    current_pid = pid;
}

int scheduler_get_current() {
    return current_pid;
}
    