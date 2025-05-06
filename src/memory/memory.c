#include "memory.h"
#include "clock.h"
#include <stdio.h>

static PageEntry page_table[NUM_PAGES];

void memory_init() {
    for (int i = 0; i < NUM_PAGES; i++) {
        page_table[i].pid = -1;
        page_table[i].page_number = -1;
        page_table[i].second_chance = 0;
        page_table[i].last_used_time = 0;
    }
}

int find_victim_page() {
    int oldest_time = __INT_MAX__;
    int victim_index = -1;
    for (int i = 0; i < NUM_PAGES; i++) {
        if (page_table[i].pid != -1 && page_table[i].second_chance == 0) {
            if (page_table[i].last_used_time < oldest_time) {
                oldest_time = page_table[i].last_used_time;
                victim_index = i;
            }
        }
    }
    if (victim_index == -1) {
        // Reiniciar os bits de segunda chance e tentar novamente
        for (int i = 0; i < NUM_PAGES; i++) {
            if (page_table[i].pid != -1) page_table[i].second_chance = 0;
        }
        return find_victim_page();
    }
    return victim_index;
}

int memory_allocate_page(int pid, int virtual_page) {
    for (int i = 0; i < NUM_PAGES; i++) {
        if (page_table[i].pid == -1) {
            page_table[i].pid = pid;
            page_table[i].page_number = virtual_page;
            page_table[i].second_chance = 1;
            page_table[i].last_used_time = clock_get_time();
            return i;
        }
    }
    int victim = find_victim_page();
    if (victim != -1) {
        page_table[victim].pid = pid;
        page_table[victim].page_number = virtual_page;
        page_table[victim].second_chance = 1;
        page_table[victim].last_used_time = clock_get_time();
        return victim;
    }
    return -1; // Falha
}

void memory_release_process(int pid) {
    for (int i = 0; i < NUM_PAGES; i++) {
        if (page_table[i].pid == pid) {
            page_table[i].pid = -1;
        }
    }
}

void memory_print() {
    printf("\n=== Tabela de Páginas ===\n");
    printf("%-5s %-5s %-5s %-10s\n", "PID", "VP", "SC", "Tempo");
    for (int i = 0; i < NUM_PAGES; i++) {
        if (page_table[i].pid != -1) {
            printf("%-5d %-5d %-5d %-10d\n",
                   page_table[i].pid,
                   page_table[i].page_number,
                   page_table[i].second_chance,
                   page_table[i].last_used_time);
        }
    }
}