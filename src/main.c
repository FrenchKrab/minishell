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
    char cmdline[MAXSTRSIZE];   //Recoit la ligne de commande entrée par l'utilisateur
    char *tokens[MAXCMD];       //Contient la suite de tokens extraite de cmdline
    process commands[MAXCMD];   //Contient les informations sur les commandes extraites des tokens

    char cwd[MAXPATHSIZE];      //Permet de stocker le cwd pour son affichage avant l'entrée de texte.

    char setpidenvcmd[16];
    sprintf(setpidenvcmd, "$=%d", getpid());
    putenv(setpidenvcmd);

    while(1)
    {
        //Initialiser toutes les données de commandes.
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
   
        //Afficher l'utilisateur actif et le chemin, et laisser l'utilisateur
        //rentrer sa ligne de commande
        getcwd(cwd, MAXPATHSIZE);
        printf("\n%s:%s$", getenv("USER"), cwd);
        fgets(cmdline, MAXSTRSIZE, stdin);

        //Transformer la ligne en suite de commandes.
        clean_str(cmdline);
        split_str(cmdline, tokens);
        set_envs(tokens);
        split_cmds(tokens, commands);

        //Executer les commandes dans l'ordre
        process *nextCommand = &commands[0];
        while (nextCommand != NULL)
        {   
            //Si la commande n'est pas une commande spéciale interne au
            //minishell, l'executer comme un processus normal. 
            if(!try_exec_special_builtin(*nextCommand))
            {
                exec_process(nextCommand);
            }

            //Executer la commande suivante en respectant les eventuelles
            //conditions d'execution
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

    return 0;
}