#ifndef MEMORY_H
#define MEMORY_H

#define PAGE_SIZE 1024
#define TOTAL_MEMORY (1024 * 1024)
#define RESERVED_MEMORY (64 * 1024)
#define NUM_PAGES ((TOTAL_MEMORY - RESERVED_MEMORY) / PAGE_SIZE)
#define MAX_RESIDENT_SET 16
#define MAX_PROCESSES 128

typedef struct {
    int pid;
    int page_number;
    int second_chance;
    int last_used_time;
} PageEntry;

void memory_init();
int memory_allocate_page(int pid, int virtual_page);
void memory_release_process(int pid);
void memory_print();

#endif // MEMORY_H