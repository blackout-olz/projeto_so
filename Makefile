# Compilador
CC = gcc

# Diretórios
SRC_DIR = src
BIN_DIR = bin
INCLUDE_DIR = include

# Flags de compilação
CFLAGS = -Wall -I$(INCLUDE_DIR)
LDFLAGS = -lpthread    # caso use pthread futuramente

# Arquivos fonte (módulos)
KERNEL_SRC = $(SRC_DIR)/kernel/kernel.c
BCP_SRC = $(SRC_DIR)/kernel/bcp.c
# Adicione o scheduler quando for implementado:
# SCHEDULER_SRC = $(SRC_DIR)/kernel/scheduler.c

# Arquivo principal
MAIN_SRC = $(SRC_DIR)/main.c

# Objetos
OBJS = $(MAIN_SRC:.c=.o) $(KERNEL_SRC:.c=.o) $(BCP_SRC:.c=.o)
# Inclua o scheduler depois:
# OBJS += $(SCHEDULER_SRC:.c=.o)

# Nome do binário
TARGET = $(BIN_DIR)/sim

# Regra padrão
all: $(TARGET)

# Linkagem final
$(TARGET): $(OBJS)
	@mkdir -p $(BIN_DIR)
	$(CC) $(OBJS) -o $@ $(LDFLAGS)

# Compilação dos .c em .o
%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

# Limpeza dos arquivos objeto e do executável
clean:
	rm -f $(SRC_DIR)/**/*.o
	rm -f $(SRC_DIR)/*.o
	rm -f $(TARGET)

# Facilita pra rodar direto pelo make
run: all
	./$(TARGET)

# Ajuda básica
help:
	@echo "Comandos disponíveis:"
	@echo "  make          - Compila o projeto"
	@echo "  make run      - Compila e executa o simulador"
	@echo "  make clean    - Limpa arquivos compilados"
