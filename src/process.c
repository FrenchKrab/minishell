/*Fichier process.c: lancement de processus
Auteur : Alexis Plaquet, Tom Rivero
Dépendances : process.h*/
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <sys/wait.h>

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#include "process.h"


/*Fonction exec_process
Paramètre proc : informations liées au processus à executer
Retourne 0 si tout s'est bien déroulé, et autre chose en cas d'erreur*/
int exec_process(process* proc) {

    
    int child_pid = fork();     //on fork


    if(child_pid == 0)  //Si on est le fils
    {
        close(proc->pipe_in[1]);
        close(proc->pipe_out[0]);

        //Si le stdin/out/err est différent de son descripteur de fichier standard
        //alors fermer le standard et dupliquer le nouveau à sa place.
        if(proc->stdin != STDIN_FILENO)
        {
            dup2(proc->stdin, STDIN_FILENO);
        }

        if(proc->stdout != STDOUT_FILENO)
        {
            dup2(proc->stdout, STDOUT_FILENO);
        }

        if(proc->stderr != STDERR_FILENO)
        {
            
            dup2(proc->stderr, STDERR_FILENO);
        }



        //close(proc->pipe_in[0]);
        //close(proc->pipe_out[1]);

        //Executer le processus
        execvp(proc->path, proc->argv);
        fprintf(stderr, "ERROR LAUNCHING THE PROCESS");
        exit(1);
    }
    else    //Si on est le père
    {

        close(proc->pipe_in[0]);
        close(proc->pipe_in[1]);

        proc->pid = child_pid;
        if(proc->background==0) //Si on ne lance pas en background
        {
            //Attendre que le fils ait terminé son execution
            //printf("%s] (parent) Waiting for process to end ...\n", proc->path);
            waitpid(child_pid, &proc->status, 0);
            //printf("%s] (parent) Process ended with error code %d \n", proc->path, proc->status);
        }
        else
        {
            proc->status = 0;
        }
    }
    return proc->status;
}


int split_cmds(char *tokens[], process *commands)
{
    size_t idx = 0;     //Index du processus
    size_t i = 0;       //Index du token actuel
    int newCommand = 1;

    while(tokens[i] != NULL) {
        if(newCommand == 1)
        {
            newCommand = 0;
            commands[idx].argv = tokens + i;
            commands[idx].path = tokens[i];
            //printf("test %s ", commands[idx].path);
        }

        if(strcmp(tokens[i], ";")==0)
        {
            tokens[i]=NULL;
            commands[idx].next = commands + idx + 1;
            //commands[idx].next_failure = commands + idx + 1;
            //commands[idx].next_succes = commands + idx + 1;
            ++idx;
            ++i;
            newCommand = 1;
            continue;
        }

        if(strcmp(tokens[i], ">")==0)
        {
            commands[idx].stdout = open(tokens[i+1], O_CREAT | O_TRUNC | O_WRONLY, S_IRUSR | S_IWUSR);
            tokens[i] = NULL;
            ++idx;
            ++i;
            continue;
        }
        
        if(strcmp(tokens[i], "2>")==0)
        {
            commands[idx].stderr = open(tokens[i+1], O_CREAT | O_TRUNC | O_WRONLY, S_IRUSR | S_IWUSR);
            tokens[i] = NULL;
            ++idx;
            ++i;
            continue;
        }

        if(strcmp(tokens[i], ">>")==0)
        {
            commands[idx].stdout = open(tokens[i+1], O_CREAT | O_APPEND | O_WRONLY, S_IRUSR | S_IWUSR);
            tokens[i] = NULL;
            ++idx;
            ++i;
            continue;
        }

        if(strcmp(tokens[i], "2>>")==0)
        {
            commands[idx].stderr = open(tokens[i+1], O_CREAT | O_APPEND | O_WRONLY, S_IRUSR | S_IWUSR);
            tokens[i] = NULL;
            ++idx;
            ++i;
            continue;
        }

        if(strcmp(tokens[i], "<")==0)
        {
            commands[idx].stdin = open(tokens[i+1], O_RDWR | O_CREAT, S_IRUSR | S_IWUSR);
            tokens[i] = NULL;
            ++idx;
            ++i;
            continue;
        }

        if(strcmp(tokens[i], "&")==0)
        {
            commands[idx].background = 1;
            tokens[i] = NULL;
            ++idx;
            ++i;
            newCommand = 1;
            continue;
        }

        if(strcmp(tokens[i], "|")==0)
        {
            commands[idx].next = &commands[idx+1];
            int new_pipe[2];
            pipe(new_pipe);
            
            commands[idx].stdout = new_pipe[1];
            commands[idx+1].stdin = new_pipe[0];

            commands[idx].pipe_out[0] = new_pipe[0];
            commands[idx].pipe_out[1] = new_pipe[1];
            commands[idx+1].pipe_in[0] = new_pipe[0];
            commands[idx+1].pipe_in[1] = new_pipe[1];


            tokens[i] = NULL;
            ++idx;
            ++i;
            newCommand = 1;

            continue;
        }


        if(strcmp(tokens[i], "||")==0)
        {
            commands[idx].next_failure = &commands[idx+1];
            tokens[i] = NULL;
            ++idx;
            ++i;
            newCommand = 1;
            printf("YES ||  \n");
            continue;
        }



        
        if(strcmp(tokens[i], "&&")==0)
        {
            commands[idx].next_succes = &commands[idx+1];
            tokens[i] = NULL;
            ++idx;
            ++i;
            newCommand = 1;
            continue;
        }

        i++;
    }


}