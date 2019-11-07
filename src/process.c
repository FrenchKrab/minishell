#include <unistd.h>

#include "process.h"

// Lance un nouveau processus qui exécute le programme passé en paramètre
// dans la structure proc. La fonction attend la fin de l'exécution si
// proc.background != 0. Les entrées/sorties sont redirigées si nécessaire.
// Le PID du nouveau processus est stocké dans proc.pid. À la fin de l'exécution
// proc.status récupère le status de la commande tel que retourné par waitpid()
int exec_process(process_t* proc) {

}
