#include <unistd.h>
#include <sys/wait.h>

#include "process.h"

// Lance un nouveau processus qui exécute le programme passé en paramètre
// dans la structure proc. La fonction attend la fin de l'exécution si
// proc.background != 0. Les entrées/sorties sont redirigées si nécessaire.
// Le PID du nouveau processus est stocké dans proc.pid. À la fin de l'exécution
// proc.status récupère le status de la commande tel que retourné par waitpid()
int exec_process(process_t* proc) {
    int child_pid = fork();

    if(child_pid == 0)  //Le fils
    {
        if(proc->stdin != STDIN_FILENO)
            dup2(proc->stdin, STDIN_FILENO);

        if(proc->stdout != STDOUT_FILENO)
            dup2(proc->stdout, STDOUT_FILENO);
            
        if(proc->stderr != STDERR_FILENO)
            dup2(proc->stderr, STDERR_FILENO);

        execv(proc->path, proc->argv);
    }
    else    //Le père
    {
        proc->pid = child_pid;
        if(proc->background!=0) //Si on ne lance pas en background
        {
            proc->status = waitpid(child_pid, NULL, 0);
        }
    }
    return 0;
}
