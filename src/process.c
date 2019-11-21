/*Fichier process.c: lancement de processus
Auteur : Alexis Plaquet, Tom Rivero
Dépendances : process.h*/
#include <unistd.h>
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
        //Si le stdin/out/err est différent de son descripteur de fichier standard
        //alors fermer le standard et dupliquer le nouveau à sa place.
        if(proc->stdin != STDIN_FILENO)
            dup2(proc->stdin, STDIN_FILENO);

        if(proc->stdout != STDOUT_FILENO)
            dup2(proc->stdout, STDOUT_FILENO);
            
        if(proc->stderr != STDERR_FILENO)
            dup2(proc->stderr, STDERR_FILENO);

        //Executer le processus
        execvp(proc->path, proc->argv);
        exit(1);
    }
    else    //Si on est le père
    {
        proc->pid = child_pid;
        if(proc->background!=0) //Si on ne lance pas en background
        {
            //Attendre que le fils ait terminé son execution
            waitpid(child_pid, &proc->status, 0);
        }
        else
        {
            proc->status = 0;
        }
        
    }
    return proc->status;
}


int split_cmds(char *cmds[], process *commands)
{
    size_t idx = 0;

    for(size_t i = 0; cmds[i]!=NULL; ++i)
    {
        commands[idx].stdin = 0;
        commands[idx].stdout = 1;
        commands[idx].stderr = 2;
        commands[idx].background = 0;
        commands[idx].argv = cmds + i;
        while(cmds[i] != NULL &&
                strcmp(cmds[i], ";")!=0 &&
                strcmp(cmds[i], ">")!=0 &&
                strcmp(cmds[i], ">>")!=0 &&
                strcmp(cmds[i], "<")!=0 &&
                strcmp(cmds[i], "&")!=0 &&
                strcmp(cmds[i], "|")!=0)
        {
            ++i;
        }

        if(cmds[i]==NULL)
            break;

        if(strcmp(cmds[i], ";")==0)
        {
            ++idx;
            cmds[i]=NULL;
            commands[idx].next = commands + idx + 1;
        }

        if(strcmp(cmds[i], ">")==0)
        {
            commands[idx].stdout = open(cmds[i+1], O_CREAT | O_TRUNC | O_WRONLY, S_IRUSR | S_IWUSR);
            cmds[i] = NULL;
            idx += 2;
        }

        if(strcmp(cmds[i], ">>")==0)
        {
            commands[idx].stdout = open(cmds[i+1], O_CREAT | O_APPEND | O_WRONLY, S_IRUSR | S_IWUSR);
            cmds[i] = NULL;
            idx += 2;
        }

        if(strcmp(cmds[i], "<")==0)
        {
            commands[idx].stdout = open(cmds[i+1], O_RDWR | O_CREAT, S_IRUSR | S_IWUSR);
            cmds[i] = NULL;
            idx += 2;
        }

        if(strcmp(cmds[i], "&")==0)
        {
            commands[idx].background = 0;
            cmds[i] = NULL;
            idx += 2;
        }

        
        if(strcmp(cmds[i], "&&")==0)
        {
            commands[idx].background = 1;
            commands[idx].next_succes = &commands[idx+1];
            cmds[i] = NULL;
            idx += 2;
        }

    }

}