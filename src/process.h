#ifndef PROCESS_H
#define PROCESS_H

#include <sys/types.h>
#include "parsing.h"

typedef struct {
  char* path;
  char* argv[MAXCMD];
  int stdin, stdout, stderr;
  int status;
  pid_t pid;
  int background;
  
} process_t;

int exec_process(process_t* proc);

#endif