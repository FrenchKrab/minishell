/*Fichier main.c: Contient la fonction main permettant l'execution du programme
Auteur : Alexis Plaquet, Tom Rivero
Dépendances : parsing.h*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "parsing.h"
#include "process.h"


#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

int main(int argc, char* argv[])
{
    char cmdline[MAXSTRSIZE];
    char* cmds[MAXCMD];
    cmdline[MAXSTRSIZE-1] = '\0';

    char *test = strdup("cat");
    char *tokens[MAXCMD];
    split_str(test, tokens);

    set_envs(tokens);
    printf("args:\n");

    for (char **tok=tokens; *tok!=NULL;++tok)
        printf(">%s<\n",*tok);

    process_t process_info;
    process_info.path = "/bin/cat";

    printf("Debut de phase buggée\n");

    //Copier tous les tokens dans argv (y compris le dernier, NULL)
    int i = -1;
    do
    {
        i+=1;
        process_info.argv[i] = tokens[i];
    } while (tokens[i]!=NULL);
    //process_info.argv[i] = NULL;
    
    process_info.background = 1;
    process_info.stdin = STDIN_FILENO;
    //process_info.stdin = open("prout.txt", O_RDWR | O_CREAT, S_IRUSR | S_IWUSR);
    process_info.stdout = STDOUT_FILENO;
    process_info.stderr = STDERR_FILENO;

    printf("Fin de phase buggée, début du lancement\n");

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