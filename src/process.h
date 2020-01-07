/*Fichier process.h: définiton de la structure process_t
Auteur : Alexis Plaquet, Tom Rivero
Dépendances : parsing.h*/

#ifndef PROCESS_H
#define PROCESS_H

#include <sys/types.h>
#include "parsing.h"

/** struct process
 * Contient les données necessaires à l'execution d'une
 * commande dans un processus séparé
 */
typedef struct process{
  char* path;   //Chemin du programme à executer
  char** argv;  //Arguments à passer au programme
  int stdin, stdout, stderr;  //File descriptors d'entrée/sortie/erreur 
  int status;   //Status du processus une fois terminé
  pid_t pid;    //pid du processus
  int background;   //si =/= 0, lancer en background (en parallèle)

  //next_failure/succes ne sont utilisés que si next est NULL
  struct process* next;   //Processus suivant à executer
  struct process* next_failure;   //Processus suivant à executer en cas d'echec
  struct process* next_succes;    //Processus suivant à executer en cas de réussite
  
  //Pipes
  //à initialiser à -1 si non utilisées
  int pipe_in[2];   //Pipe connectant à l'entrée du processus
  int pipe_out[2];  //Pipe connectant à la sortie du processus
}process;


void init_process(process* proc);

int exec_process(process* proc);

void exec_process_chain(process proc);

int split_cmds(char *cmds[], process *commands);

#endif