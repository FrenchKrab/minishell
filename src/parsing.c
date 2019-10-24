#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#include "parsing.h"

// "Nettoyer" la chaîne de caractères
char* clean_str(char* str) {
    assert(str!=NULL);
    assert(strlen(str)<MAXSTRSIZE);

    // Supprimer les blancs au début et à la fin de la ligne
    // Ex: "   str1    str2    " => "str1    str2"


    int strLen = strlen(str);
    int trueStartIndex = 0;
    for (int i = 0; i<strLen; ++i)
    {
        if(str[i] == ' ')
        {
            trueStartIndex = i;
        }
        else
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

    // Etat actuel : "str1    str2"

    // Supprimer les doublons d'espaces entre les mots de la ligne
    // Ex: "str1    str2  str3" => "str1 str2 str3"

    /* Methode pas opti
    strLen = strlen(str);
    for(int i = 0; i < strLen - 1; ++i)
    {
        if(str[i]==' ' && str[i+1]==' ')
        {
            //+2 pour prendre le \0 de fin de chaine
            memmove(str[i],str[i+1], strLen-i+2);
            strLen = strlen(str);
        }
    }
    */

   //methode opti mais qui marche peut etre pas
    int start = 0;
    int end = 0;
    int previousWasSpace = 0;
    for(int i = 0; i < strLen; ++i)
    {
        if(str[i]==' ')
        {
            if(previousWasSpace)
            {
                end = i;
            }
            else
            {
                start = i;
            }
            previousWasSpace = 1;
        }
        else
        {
            if(previousWasSpace)
            {
                memmove(str[start],str[end], strLen-end+1);
                i = start + 1;
            }
            previousWasSpace = 0;
        }
    }
    

    return str;
}


// Découper la chaîne de caractères en ses différents éléments
size_t split_str(char* str, char* tokens[]) {
    assert(str!=NULL);
    assert(tokens!=NULL);


    // Renvoie le nombre d'éléments dans le tableau tokens
}


int set_envs(char* tokens[]) {
    assert(tokens!=NULL);

    // Cherche toutes les chaînes du tableau commençant par '$'
    // Récupère la variable d'environnement correspondante et
    // la substitue à l'élément du tableau
    // Attention: si la variable n'est pas définie on utilisera
    // la chaîne vide "" (et pas NULL)


    // Renvoie le nombre de variables substituées

}