# Makefile

CC = gcc
CFLAGS = -Wall -Iinclude

SRC = \
  src/main.c \
  src/kernel/bcp.c \
  src/kernel/kernel.c \
  src/kernel/scheduler.c \
  src/memory/memory.c \
  src/semaphore/semaphore.c \
  src/synthetic_program/clock.c \
  src/synthetic_program/interpreter.c

OBJ = $(SRC:.c=.o)

EXEC = simulador

all: $(EXEC)

$(EXEC): $(SRC)
	$(CC) $(CFLAGS) -o $@ $^

clean:
	rm -f $(EXEC) *.o src/*/*.o
