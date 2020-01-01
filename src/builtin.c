#include "builtin.h"
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <ctype.h>

extern char** environ;



int try_exec_builtin(process proc)
{
    void (*fun_to_exec)(process) = NULL;

    if(strcmp(proc.path, "pwd") == 0)
    {
        fun_to_exec = builtin_pwd;
    }
    else if(strcmp(proc.path, "env") == 0)
    {
        fun_to_exec = builtin_env;
    }


    if(fun_to_exec != NULL)
    {
        fun_to_exec(proc);
    }
    else
    {
        return 0;
    }
}


int try_exec_special_builtin(process proc)
{
    void (*fun_to_exec)(process) = NULL;

    if(strcmp(proc.path, "export") == 0)
    {
        fun_to_exec = builtin_special_export;
    }
    else if(strcmp(proc.path, "unsetenv") == 0)
    {
        fun_to_exec = builtin_special_unsetenv;
    }
    else if(strcmp(proc.path, "cd") == 0)
    {
        fun_to_exec = builtin_special_cd;
    }
    else if(strcmp(proc.path, "exit") == 0)
    {
        fun_to_exec = builtin_special_exit;
    }

    if(fun_to_exec != NULL)
    {
        fun_to_exec(proc);
        return 1;
    }
    else
    {
        return 0;
    }
}



void builtin_pwd(process proc)
{
    size_t length = 2048;
    char path[length];
    getcwd(path, length);
    fprintf(stdout, "%s\n", path);
}

void builtin_env(process proc)
{
    for (char **env = environ; *env; ++env)
        fprintf(stdout, "%s\n", *env);
}

void builtin_special_export(process proc)
{
    if(proc.argv[1] == NULL)
    {
        fprintf(stderr, "export prend un argument en suivant le formatage : 'export NOMVARIABLE=VALEUR'\n");
    }

    putenv(proc.argv[1]);
}

void builtin_special_unsetenv(process proc)
{
    if(proc.argv[1] == NULL)
    {
        fprintf(stderr, "unset prend un argument en suivant le formatage : 'unset NOMVARIABLE'\n");
    }

    unsetenv(proc.argv[1]);
}

void builtin_special_cd(process proc)
{
    if(proc.argv[1] == NULL)
    {
        fprintf(stderr, "cd prend un argument en suivant le formatage : 'cd CHEMIN'\n");
    }

    chdir(proc.argv[1]);
}

void builtin_special_exit(process proc)
{
    int exit_value = 0;
    if(proc.argv[1] != NULL)
    {
        exit_value = atoi(proc.argv[1]);
    }
    exit(exit_value);
}