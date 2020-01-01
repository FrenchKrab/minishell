/*Fichier main.c: Contient la fonction main permettant l'execution du programme
Auteur : Alexis Plaquet, Tom Rivero
Dépendances : parsing.h*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "parsing.h"
#include "process.h"
#include "builtin.h"


#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

int main(int argc, char* argv[])
{
    char cmdline[MAXSTRSIZE];
    char *tokens[MAXCMD];
    process commands[MAXCMD];

    char cwd[MAXPATHSIZE];

    while(1)
    {
        for(size_t idx = 0; idx<MAXCMD; ++idx)
        {
            commands[idx].stdin = 0;
            commands[idx].stdout = 1;
            commands[idx].stderr = 2;
            commands[idx].background = 0;
            commands[idx].argv = NULL;
            commands[idx].path = NULL;
            commands[idx].next_failure = NULL;
            commands[idx].next_succes = NULL;
            commands[idx].next = NULL;

            commands[idx].pipe_in[0] = -1;
            commands[idx].pipe_in[1] = -1;
            commands[idx].pipe_out[0] = -1;
            commands[idx].pipe_out[1] = -1;
        }
   
        getcwd(cwd, MAXPATHSIZE);

        printf("%s:%s$", getenv("USER"), cwd);
        fgets(cmdline, MAXSTRSIZE, stdin);


        clean_str(cmdline);
        split_str(cmdline, tokens);
        set_envs(tokens);


/*
        printf("--args:%p\n", tokens);

        for (char **tok=tokens; *tok!=NULL;++tok)
        {
            printf(">%s<\n",*tok);
        }
*/

        /*
        process process_info;

        process_info.argv = tokens;
        

        process_info.path = process_info.argv[0];
        //process_info.path = "/bin/ls";

        process_info.background = 1;
        process_info.stdin = STDIN_FILENO;
        //process_info.stdin = open("prout.txt", O_RDWR | O_CREAT, S_IRUSR | S_IWUSR);
        process_info.stdout = STDOUT_FILENO;
        //process_info.stdout = open("prout2.txt", O_WRONLY | O_TRUNC | O_CREAT, S_IWUSR | S_IRUSR);
        process_info.stderr = STDERR_FILENO;


        exec_process(&process_info);*/

        split_cmds(tokens, commands);

        process *nextCommand = &commands[0];

        while (nextCommand != NULL)
        {

            if(!try_exec_special_builtin(*nextCommand))
            {
                exec_process(nextCommand);
            }

            if(nextCommand->next != NULL)
            {
                nextCommand = nextCommand->next;
            }
            else
            {
                if(nextCommand->status == 0)
                {
                    nextCommand = nextCommand->next_succes;
                }
                else
                {
                    nextCommand = nextCommand->next_failure;
                }
            }
        }
        


    }
    
    /*
    process process_info;
    process_info.path = "/bin/cat";

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
    //process_info.stdout = open("prout2.txt", O_WRONLY | O_TRUNC | O_CREAT, S_IWUSR | S_IRUSR);
    process_info.stderr = STDERR_FILENO;



    printf("Fin de phase buggée, début du lancement\n");

    exec_process(&process_info);

*/

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