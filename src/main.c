/*Fichier main.c: Contient la fonction main permettant l'execution du programme
Auteur : Alexis Plaquet, Tom Rivero
Dépendances : parsing.h, process.h, builtin.h*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#include <readline/readline.h>
#include <readline/history.h>

#include "parsing.h"
#include "process.h"
#include "builtin.h"


char minishell_version[] = "1.2";

int main(int argc, char* argv[])
{
    char cmdline[MAXSTRSIZE];   //Recoit la ligne de commande entrée par l'utilisateur
    char *tokens[MAXCMD];       //Contient la suite de tokens extraite de cmdline
    process commands[MAXCMD];   //Contient les informations sur les commandes extraites des tokens

    char cwd[MAXPATHSIZE];      //Permet de stocker le cwd pour son affichage avant l'entrée de texte.

    char setpidenvcmd[16];

    sprintf(setpidenvcmd, "$=%d", getpid());
    putenv(setpidenvcmd);

    printf("\033[0;36m           _       _     _          _ _ \n _ __ ___ (_)_ __ (_)___| |__   ___| | |\n| '_ ` _ \\| | '_ \\| / __| '_ \\ / _ \\ | |\n| | | | | | | | | | \\__ \\ | | |  __/ | |\n|_| |_| |_|_|_| |_|_|___/_| |_|\\___|_|_|");
    printf("\nv%s - Alexis Plaquet, Tom Rivero - 2020\n###########################################\n",minishell_version);

    while(1)
    {
        //Initialiser toutes les données de commandes.
        for(size_t idx = 0; idx<MAXCMD; ++idx)
        {
            init_process(&commands[idx]);
        }
   
        //Afficher l'utilisateur actif et le chemin, et laisser l'utilisateur
        //rentrer sa ligne de commande
        getcwd(cwd, MAXPATHSIZE);
        char line_prefix[MAXPATHSIZE+128];
        sprintf(line_prefix, "\033[1;33m%s\033[0m:\033[0;35m%s\033[0m$", getenv("USER"), cwd);
        
        char *buf = readline(line_prefix);

        //Copier le buffer dans cmdline et ajouter la ligne à l'historique, puis libérer le buffer
        //(readline faisant un malloc à chaque appel)
        strncpy(cmdline, buf, MAXCMD);

        //Si la ligne est vide, redemander une entrée à l'utilisateur, sinon l'ajouter à l'historique.
        if (strlen(cmdline) > 0)
            add_history(buf);
        free(buf);
        if (strlen(cmdline) == 0)
            continue;   //On ne peut le faire que là car il faut free(buf avant)

        //Transformer la ligne en suite de commandes.
        clean_str(cmdline);
        split_str(cmdline, tokens);
        set_envs(tokens);
        split_cmds(tokens, commands);

        //Executer les commandes dans l'ordre
        exec_process_chain(commands[0]);
    }

    return 0;
}