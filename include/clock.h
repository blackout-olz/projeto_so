#ifndef CLOCK_H
#define CLOCK_H

// Inicializa o relógio virtual
void clock_init();

// Avança o relógio virtual em t unidades de tempo
void clock_advance(int t);

// Retorna o tempo atual do relógio virtual
int clock_get_time();

#endif // CLOCK_H
