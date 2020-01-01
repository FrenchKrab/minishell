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
#include "builtin.h"


/*Fonction exec_process
Paramètre proc : informations liées au processus à executer
Retourne 0 si tout s'est bien déroulé, et autre chose en cas d'erreur*/
int exec_process(process* proc) {
    int child_pid = fork();     //on fork

    if(child_pid == 0)  //Si on est le fils
    {
        //Fermer les "bouts" inutiles des pipes de ce processus 
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

        //Tenter d'executer le processus comme commande interne au shell.
        //Si ce n'est pas une commmande interne au shell, l'executer comme
        //commande normale
        if(try_exec_builtin(*proc))
        {
            exit(0);
        }
        else
        {
            execvp(proc->path, proc->argv);
            fprintf(stderr, "ERROR LAUNCHING THE PROCESS %s\n", proc->path);
            exit(1);    
        }
    }
    else    //Si on est le père
    {
        //Fermer le pipe reliant à l'entrée du processus fils
        //(ce dernier en possède déjà une copie ouverte, il est inutile de le
        // garder plus longtemps)
        close(proc->pipe_in[0]);
        close(proc->pipe_in[1]);

        proc->pid = child_pid;
        if(proc->background==0) //Si on ne lance pas en background
        {
            //Attendre que le fils ait terminé son execution
            waitpid(child_pid, &proc->status, 0);
        }
        else    
        {
            //Si on lance en background, on ne se préoccupe pas du status
            proc->status = 0;
        }
    }
    return proc->status;
}

/*Fonction split_cmds
Paramètres:
    -tokens : tokens extraits de la ligne de commande entrée par l'utilisateur
    -commands: tableau permettant de stocker les informations sur les commandes
Retourne 0 si tout s'est bien déroulé, et autre chose en cas d'erreur*/
int split_cmds(char *tokens[], process *commands)
{
    size_t idx = 0;     //Index du processus
    size_t i = 0;       //Index du token actuel
    int newCommand = 1; //Le prochain token est-il le début d'une nouvelle commande

    while(tokens[i] != NULL) {
        //Si on est sur une nouvelle commande, créer les informations associées
        if(newCommand == 1)
        {
            newCommand = 0;
            commands[idx].argv = tokens + i;
            commands[idx].path = tokens[i];
        }


        //Detection des tokens spéciaux
        if(strcmp(tokens[i], ";")==0)
        {
            tokens[i]=NULL;
            commands[idx].next = commands + idx + 1;
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