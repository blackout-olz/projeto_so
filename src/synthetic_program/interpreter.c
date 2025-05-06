#include "interpreter.h"
#include "bcp.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_LINE 256

extern BCP* bcp_get(int pid);

static CommandType parse_command_type(const char *cmd) {
    if (strncmp(cmd, "exec", 4) == 0) return CMD_EXEC;
    if (strncmp(cmd, "read", 4) == 0) return CMD_READ;
    if (strncmp(cmd, "write", 5) == 0) return CMD_WRITE;
    if (cmd[0] == 'P') return CMD_P;
    if (cmd[0] == 'V') return CMD_V;
    if (strncmp(cmd, "print", 5) == 0) return CMD_PRINT;
    return -1;
}

int interpreter_load_program(const char *filepath) {
    FILE *fp = fopen(filepath, "r");
    if (!fp) {
        perror("Erro ao abrir o programa sintético");
        return -1;
    }

    char name[64];
    if (!fgets(name, sizeof(name), fp)) return -1;
    name[strcspn(name, "\r\n")] = '\0';
    int start = 0;
    while (name[start] != '\0' && (unsigned char)name[start] <= 32) start++;
    if (start > 0) memmove(name, name + start, strlen(name + start) + 1);

    int segment_id, priority, size;
    if (fscanf(fp, "%d", &segment_id) != 1) return -1;
    if (fscanf(fp, "%d", &priority) != 1) return -1;
    if (fscanf(fp, "%d", &size) != 1) return -1;
    fgetc(fp);

    char semaphores_line[MAX_LINE];
    if (!fgets(semaphores_line, sizeof(semaphores_line), fp)) return -1;

    char line[MAX_LINE];
    ProgramCommand commands[MAX_COMMANDS];
    int cmd_count = 0;
    int exec_time_total = 0;

    while (fgets(line, sizeof(line), fp)) {
        char op[64];
        if (sscanf(line, "%s", op) < 1) continue;

        CommandType type = parse_command_type(op);
        if (type == -1) continue;

        ProgramCommand *cmd = &commands[cmd_count];
        cmd->type = type;

        if (type == CMD_EXEC || type == CMD_READ || type == CMD_WRITE || type == CMD_PRINT) {
            int val;
            if (sscanf(line + strlen(op), "%d", &val) == 1) {
                cmd->value = val;
            } else {
                continue;
            }
            cmd->sem_id[0] = '\0';
        } else if (type == CMD_P || type == CMD_V) {
            char *par_open = strchr(op, '(');
            char *par_close = strchr(op, ')');
            if (par_open && par_close && par_close > par_open + 1) {
                size_t len = par_close - par_open - 1;
                strncpy(cmd->sem_id, par_open + 1, len);
                cmd->sem_id[len] = '\0';
            } else {
                continue;
            }
            cmd->value = -1;
        }

        if (type == CMD_EXEC) exec_time_total += cmd->value;

        printf("[DEBUG] Comando %d: tipo=%d sem=%s val=%d\n", cmd_count, cmd->type, cmd->sem_id, cmd->value);
        cmd_count++;
    }

    fclose(fp);

    int pid = bcp_create(name, segment_id, priority, exec_time_total);
    BCP *proc = bcp_get(pid);
    if (proc) {
        for (int i = 0; i < cmd_count && i < MAX_COMMANDS; i++) {
            proc->commands[i] = commands[i];
        }
        proc->command_count = cmd_count;
    }

    return pid;
}
