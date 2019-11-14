/*Fichier process.c: lancement de processus
Auteur : Alexis Plaquet, Tom Rivero
Dépendances : process.h*/
#include <unistd.h>
#include <sys/wait.h>

#include "process.h"

/*Fonction exec_process
Paramètre proc : informations liées au processus à executer
Retourne 0 si tout s'est bien déroulé, et autre chose en cas d'erreur*/
int exec_process(process_t* proc) {
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
        execv(proc->path, proc->argv);
        exit(1);
    }
    else    //Si on est le père
    {
        proc->pid = child_pid;
        if(proc->background!=0) //Si on ne lance pas en background
        {
            //Attendre que le fils ait terminé son execution
            proc->status = waitpid(child_pid, NULL, 0);
        }
        else
        {
            proc->status = 0;
        }
        
    }
    return proc->status;
}
