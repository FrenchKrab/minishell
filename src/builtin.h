#ifndef BUILTIN_H
#define BUILTIN_H

#include "process.h"


int try_exec_builtin(process);

int try_exec_special_builtin(process);


void builtin_pwd(process);
void builtin_env(process);

void builtin_special_export(process);
void builtin_special_unsetenv(process);
void builtin_special_cd(process);
void builtin_special_exit(process);

#endif


