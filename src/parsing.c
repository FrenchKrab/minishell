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

    // Supprimer les doublons d'espaces entre les mots de la ligne
    // Ex: "str1    str2  str3" => "str1 str2 str3"

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