# Simulador de Núcleo de Sistema Operacional

Este projeto implementa um simulador do núcleo de um sistema operacional, com foco na execução de programas sintéticos e no gerenciamento de processos, semáforos, memória e escalonamento.

## Objetivo

Simular o funcionamento de um núcleo de SO, controlando:

* Processos concorrentes
* Escalonamento por uso de I/O
* Gerenciamento de memória com paginação
* Semáforos com suporte a P(s) e V(s)
* Interface interativa para controle manual

## Estrutura de Pastas

```
include/        # Arquivos .h (headers)
src/            # Código-fonte modular
  - kernel/     # Lógica principal do SO (BCP, escalonador, kernel)
  - memory/     # Gerenciamento de memória com segunda chance
  - printer/    # (Reservado para Parte 2)
  - disk/       # (Reservado para Parte 2)
  - semaphore/  # Semáforos
  - synthetic_program/ # Interpretador, relógio virtual
programs/       # Programas sintéticos (.txt)
```

## Como compilar e executar

```bash
make
./simulador
```

## Menu de Controle

1. Carregar programa sintético
2. Listar processos
3. Executar ciclo de evento (simula 1 quantum)
4. Visualizar estado da memória
5. Exibir tempo do relógio virtual
6. Forçar liberação de um semáforo (V)
7. Sair

## Programas Sintéticos

Arquivo `.txt` dividido em:

* Cabeçalho: nome, id, prioridade, tamanho, semáforos
* Corpo: comandos (`exec`, `P(s)`, `V(s)`, `read`, `write`, `print`)

Exemplo:

```txt
progA
1
3
32
s
exec 1000
P(s)
exec 1000
V(s)
print 500
```

## Escalonador

Escolhe o processo com maior `io_count` (read/write).
Em caso de empate, o processo com menor PID é escolhido.

## Memória

* Memória total: 1 MB
* SO reserva: 64 KB
* Página: 1 KB
* Cada processo tem até 16 páginas
* `exec` consome 1 página por 1000 unidades de tempo
* Política de substituição: Segunda Chance

## Semáforos

* Suporte completo a `P(s)` e `V(s)`
* Fila FIFO para bloqueio
* Interface permite forçar `V(s)` manualmente

## Clock

* Avança com cada `exec`
* Pode ser consultado pelo menu

## Teste sugerido

* Execute 3 programas com `P(s)` simultaneamente
* Carregue um 4º com dois `V(s)` para desbloquear
* Observe alternância entre processos e desbloqueio correto

## Requisitos atendidos da Parte 1

* [x] Criação e finalização de processos
* [x] Gerenciamento de memória com paginação e segunda chance
* [x] Suporte a semáforos (P e V)
* [x] Escalonamento por I/O
* [x] Interface de controle
* [x] Relógio virtual

---

Desenvolvido para o projeto de Sistemas Operacionais.

## Autores
- André Luís Mendes Júnior
- Bruna Fontes de Castro
- Felipe do Amparo Lauria
- Vitor Hugo Lellis Lopes

