/*Fichier parsing.c: parsing simple et gestion des variables d'environnement
Auteur : Alexis Plaquet, Tom Rivero
Dépendances : parsing.h*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#include "parsing.h"

/*Fonction clean_str
Paramètre str : chaine de caractères à nettoyer
Retourne la chaine nettoyée
*/
char* clean_str(char* str) {
    assert(str!=NULL);
    assert(strlen(str)<MAXSTRSIZE);

    // Supprimer les blancs au début et à la fin de la ligne
    // Ex: "   str1    str2    " => "str1    str2"

    int strLen = strlen(str);
    int trueStartIndex = 0;
    for (int i = 0; i<strLen; ++i)
    {
        trueStartIndex = i;
        if(str[i] != ' ')
        {
            break;
        }
    }

    //+1 pour prendre le \0 de fin de chaine
    memmove(str, str + trueStartIndex, strLen - trueStartIndex + 1);
    strLen = strlen(str);

    for(int i = strLen - 1; i>=0; --i)
    {
        if(str[i]==' ' || str[i]=='\n')
        {
            str[i]='\0';
        }
        else
        {
            break;
        }
    }
    strLen = strlen(str);
    // Etat actuel : "str1    str2"

    // Supprimer les doublons d'espaces entre les mots de la ligne
    // Ex: "str1    str2  str3" => "str1 str2 str3"

    int start = 0;  //début d'une série d'espaces
    int end = 0;    //fin de la série d'espaces
    int previousWasSpace = 0;   //boolean : est-ce que le dernier caractère était un espace ?
    for(int i = 0; i < strLen; ++i)
    {
        if(str[i]==' ') //On est sur un espace
        {
            if(previousWasSpace)    //le dernier caractere était aussi un espace
            {
                end = i;
            }
            else    //c'est le premier espace rencontré
            {
                start = i;
                end = i;
                previousWasSpace = 1;
            }
        }
        else    //On n'est pas sur un espace
        {
            if(previousWasSpace)    //Et le dernier caractere était un espace
            {
                //Déplacer la chaine pour enlever les espaces redondants
                memmove(str + start,str + end, strLen-end+1);
                i = start + 1;
                previousWasSpace = 0;
            }
        }
    }

    return str;
}


/*Fonction split_str
Paramètre str : la chaine à split
Paramètre tokens[] : le tableau contenant les éléments séparés
Retourne un size_t, le nombre d'éléments dans le tableau tokens*/

// Découper la chaîne de caractères en ses différents éléments
size_t split_str(char* str, char* tokens[]) {
    assert(str!=NULL);
    assert(tokens!=NULL);

    tokens[0] = str;
    size_t compteur = 1;
    size_t strsize = strlen(str);
    for(int i=0; i<strsize; ++i)
    {
        if(str[i]==' ')
        {
            str[i] = '\0';
            tokens[compteur] = &str[i+1];
            compteur++;
        }
    }
    tokens[compteur]=NULL;

    // Renvoie le nombre d'éléments dans le tableau tokens
    return compteur;
}


/*Fonction set_envs
Paramètre tokens[] : le tableau d'élements où il faut substituer les variables d'environnement
Retourne un int, le nombre de variables substituées*/
int set_envs(char* tokens[]) {
    assert(tokens!=NULL);

    size_t compteur = 0;
    for(size_t i=0; tokens[i]!=NULL; ++i)
    {
        char* elem = tokens[i];
        //Cherche toutes les chaînes du tableau commençant par '$'
        if(elem[0]=='$')
        {
            //On récupère la variable d'environnement correspondante et la substitue à l'élément du tableau
            char *var = getenv(elem+1);
            printf("variable %s = %s",elem, var);
            if(var!=NULL)
                tokens[i]=var;
            else    //Si la variable n'est pas définie on utilise la chaîne vide "" (et pas NULL)
                tokens[i]="";
            compteur++;
        }
    }
    // Renvoie le nombre de variables substituées
    return compteur;
}