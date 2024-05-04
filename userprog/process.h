#ifndef USERPROG_PROCESS_H
#define USERPROG_PROCESS_H

#include "threads/thread.h"

// should be shared between syscall and process.h
#define CMD_ARGS_DELIMITER " "
#define CMD_ARGS_MAX 30
#define CMD_LENGTH_MAX 100
typedef int pid_t;


pid_t process_execute (const char *file_name);
int process_wait (pid_t);
void process_exit (int status);
void process_activate (void);
void processus_init(void);
int processus_ouvrir (const char *file_name);
int processus_ecrire(int fd, const void *buffer, unsigned size);
int processus_lire (int fd, void *buffer, unsigned length);
void processus_fermer (int fd);
int processus_lire (int fd, void *buffer, unsigned length);
int processus_ecrire (int fd, const void *buffer, unsigned length);
void processus_deplacer (int fd, unsigned position);
int processus_taille_fichier (int fd);
int processus_position (int fd);
#endif /* userprog/process.h */
