/*Fichier process.h: définiton de la structure process_t
Auteur : Alexis Plaquet, Tom Rivero
Dépendances : parsing.h*/

#ifndef PROCESS_H
#define PROCESS_H

#include <sys/types.h>
#include "parsing.h"

typedef struct{
  char* path;
  char** argv;
  int stdin, stdout, stderr;
  int status;
  pid_t pid;
  int background;
  struct process* next;
  struct process* next_failure;
  struct process* next_succes;
  
}process;

int exec_process(process* proc);

#endif