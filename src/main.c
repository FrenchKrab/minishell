/*Fichier main.c: Contient la fonction main permettant l'execution du programme
Auteur : Alexis Plaquet, Tom Rivero
Dépendances : parsing.h*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "parsing.h"

int main(int argc, char* argv[])
{
    char cmdline[MAXSTRSIZE];
    char* cmds[MAXCMD];
    cmdline[MAXSTRSIZE-1] = '\0';

    char *test = strdup("    1aaaa 2     3 4  df  ");
    char *tokens[MAXCMD];
    clean_str(test);
    split_str(test, tokens);
    for (char **tok=tokens; *tok!=NULL;++tok)
        printf("-%s-\n",*tok);


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