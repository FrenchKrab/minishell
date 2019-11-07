/*Fichier main.c: Contient la fonction main permettant l'execution du programme
Auteur : Alexis Plaquet, Tom Rivero
Dépendances : parsing.h*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "parsing.h"
#include "process.h"

int main(int argc, char* argv[])
{
    char cmdline[MAXSTRSIZE];
    char* cmds[MAXCMD];
    cmdline[MAXSTRSIZE-1] = '\0';

    char *test = strdup("1aaaa 2 $USER 4 df");
    char *tokens[MAXCMD];
    split_str(test, tokens);
    for (char **tok=tokens; *tok!=NULL;++tok)
        printf("-%s-\n",*tok);
    set_envs(tokens);
    printf("\n\n");
    for (char **tok=tokens; *tok!=NULL;++tok)
        printf("-%s-\n",*tok);

    process_t process_info;
    process_info.path = "/bin/ls";

    int i = 0;
    do
    {
        process_info.argv[i] = tokens[i];
    } while (tokens[i]!=NULL);
    
    process_info.background = 1;
    process_info.stdin = STDIN_FILENO;
    process_info.stdout = STDOUT_FILENO;
    process_info.stderr = STDERR_FILENO;

    exec_process(&process_info);

    /*while (1)
    {
        printf("$");
        fgets(cmdline, MAXSTRSIZE-1, stdin);
        //clean_str(cmdline);         //"  chaine   chaine   $USER  " --> "chaine chaine $USER"
        split_str(cmdline, cmds);   //split mot à mot : cmds = {"chaine", "chaine", "$USER", NULL}
        //set_envs(cmds);             //cmds = {"chaine", "chaine", "login", NULL}
        if (strcmp(cmds[0], "exit")==0)
            break;
    }*/

    return 0;
}