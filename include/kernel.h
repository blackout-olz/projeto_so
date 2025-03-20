#ifndef KERNEL_H
#define KERNEL_H

// Função para inicializar o Kernel e seus módulos (BCP, Scheduler, etc.)
void initKernel();

// Interface de chamada de sistema (chamadas vindas de processos sintéticos)
void sysCall(int eventCode, int pid);

// Interface de controle de interrupções externas (vindas de E/S simuladas)
void interruptControl(int eventCode, int pid);

#endif /* KERNEL_H */