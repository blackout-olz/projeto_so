#ifndef INTERPRETER_H
#define INTERPRETER_H

#define MAX_COMMANDS 128

// Tipos de comandos sintéticos
typedef enum {
    CMD_EXEC,
    CMD_READ,
    CMD_WRITE,
    CMD_P,
    CMD_V,
    CMD_PRINT
} CommandType;

typedef struct {
    CommandType type;
    int value;
    char sem_id[16];
} ProgramCommand;

// Adiciona os comandos no processo criado pelo interpretador
int interpreter_load_program(const char *filepath);

#endif // INTERPRETER_H