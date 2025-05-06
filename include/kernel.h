#ifndef KERNEL_H
#define KERNEL_H

// Códigos de evento válidos para Parte 1
#define EVENT_PROCESS_INTERRUPT 1
#define EVENT_PROCESS_CREATE    2
#define EVENT_PROCESS_FINISH    3
#define EVENT_MEM_LOAD_REQ      6
#define EVENT_MEM_LOAD_FINISH   7
#define EVENT_SEMAPHORE_P       10
#define EVENT_SEMAPHORE_V       11

void kernel_init();
int interruptControl();
int sysCall();
void kernel_handle_event(int event_code);

#endif // KERNEL_H