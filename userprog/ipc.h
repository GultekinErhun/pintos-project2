#include <stdbool.h>
typedef int pid_t;

void ipc_initialiser(void);
int ipc_pipe_lecture(char *pipe_name, int ticket);
void ipc_pipe_ecriture(char *pipe_name, int ticket, int msg);
