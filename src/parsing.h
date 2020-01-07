/*Fichier parsing.h: déclaration des différentes constantes liées au parsing
Auteur : Alexis Plaquet, Tom Rivero*/

#ifndef PARSING_H
#define PARSING_H

#define MAXSTRSIZE 2048     //Taille maximale de la ligne entrée par l'utilisateur
#define MAXCMD     128      //Nombre maximal de commandes gérées en une ligne de commande
#define MAXPATHSIZE 4096    //Taille max d'une chaine de caractère désignant un chemin

char* clean_str(char* str);
size_t split_str(char* str, char* tokens[]);
int set_envs(char* tokens[]);

#endif
