#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include"parsing.h"

int main(int argc, char* argv[])
{
    char cmdline[MAXSTRSIZE];
    char* cmds[MAXCMD];
    cmdline[MAXSTRSIZE-1] = NULL;

    while (1)
    {
        printf("$");
        fgets(cmdline, MAXSTRSIZE-1, stdin);
        clean_str(cmdline);         //"  chaine   chaine   $USER  " --> "chaine chaine $USER"
        split_str(cmdline, cmds);   //split mot à mot : cmds = {"chaine", "chaine", "$USER", NULL}
        set_envs(cmds);             //cmds = {"chaine", "chaine", "login", NULL}
        if (strcmp(cmds[0], "exit")==0)
            break;
    }

    return 0;
}