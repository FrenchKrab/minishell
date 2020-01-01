/*Fichier builtin.h: gestion des commandes internes
Auteur : Alexis Plaquet, Tom Rivero
Dépendances : process.h*/
#ifndef BUILTIN_H
#define BUILTIN_H

#include "process.h"

int try_exec_builtin(process);
int try_exec_special_builtin(process);

//Commandes internes
void builtin_pwd(process);
void builtin_env(process);

//Commandes internes spéciales (ne créent pas de processus fils)
void builtin_special_export(process);
void builtin_special_unsetenv(process);
void builtin_special_cd(process);
void builtin_special_exit(process);

#endif


