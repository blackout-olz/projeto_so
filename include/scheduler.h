#ifndef SCHEDULER_H
#define SCHEDULER_H

// Inicializa o escalonador
void scheduler_init();

// Escolhe o próximo processo para execução com base na política de IO count
int scheduler_select_next();

// Marca o processo atual como terminado e escalona o próximo
int scheduler_finish_current();

// Define o processo atual
void scheduler_set_current(int pid);

// Retorna o PID do processo atualmente em execução
int scheduler_get_current();

#endif // SCHEDULER_H
