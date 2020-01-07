/*Fichier builtin.c: gestion des commandes internes
Auteur : Alexis Plaquet, Tom Rivero
Dépendances : builtin.h*/
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>

#include "builtin.h"


extern char** environ;

/*----------Fonctions tentant l'execution à partir de données 'process'----------*/

/*Fonction try_exec_builtin
Paramètre proc : Informations sur le processus à executer comme commande interne
Retourne 0 si aucune commande interne ne correspond, et 1 (ou rien) sinon*/
int try_exec_builtin(process proc)
{
    void (*fun_to_exec)(process) = NULL;

    if(strcmp(proc.path, "pwd") == 0)
    {
        fun_to_exec = builtin_pwd;
    }
    else if(strcmp(proc.path, "env") == 0)
    {
        fun_to_exec = builtin_env;
    }
    else if(strcmp(proc.path, "credits") == 0)
    {
        fun_to_exec = builtin_credits;
    }


    if(fun_to_exec != NULL)
    {
        fun_to_exec(proc);
        return 1;
    }
    else
    {
        return 0;
    }
}

/*Fonction try_exec_special_builtin
Paramètre proc : Informations sur le processus à executer comme commande interne spéciale
Retourne 0 si aucune commande interne ne correspond, et 1 si une commande a été executée*/
int try_exec_special_builtin(process proc)
{
    void (*fun_to_exec)(process) = NULL;

    if(strcmp(proc.path, "export") == 0)
    {
        fun_to_exec = builtin_special_export;
    }
    else if(strcmp(proc.path, "unsetenv") == 0)
    {
        fun_to_exec = builtin_special_unsetenv;
    }
    else if(strcmp(proc.path, "cd") == 0)
    {
        fun_to_exec = builtin_special_cd;
    }
    else if(strcmp(proc.path, "exit") == 0)
    {
        fun_to_exec = builtin_special_exit;
    }

    if(fun_to_exec != NULL)
    {
        fun_to_exec(proc);
        return 1;
    }
    else
    {
        return 0;
    }
}

/*----------Commandes intégrées aux shell normales---------*/
/* (se comportent comme des commandes normales) */

/*Fonction builtin_pwd
Execute la commande pwd*/
void builtin_pwd(process proc)
{
    size_t length = 2048;
    char path[length];
    getcwd(path, length);
    fprintf(stdout, "%s\n", path);
}

/*Fonction builtin_env
Execute la commande env*/
void builtin_env(process proc)
{
    for (char **env = environ; *env; ++env)
        fprintf(stdout, "%s\n", *env);
}

/*Fonction builtin_minishell_credits
Execute la commande non demandée par le professeur*/
void builtin_credits(process proc)
{
    fprintf(stdout,"----------\nLogiciel développé par Alexis PLAQUET et Tom RIVERO\nau cours de leur L3 informatique en 2019-2020\nFélicitations pour avoir trouvé cette commande !\nTapez credits MOTDEPASSE pour afficher le message secret\n----------");
    //Easter egg :D (désolé pour cette ligne)
    if (proc.argv[0] != NULL && proc.argv[1] != NULL)
    {
        char its_easter_egg_time[] = " 0-+2:93?*1=De3k518q6G4a#o5*5:684C9r.&b<68E,('}q7=$+o5l3A0+Aq57=%7o5*r?00oA77.*'DIeF0.2Bq5A@3b36(:4'(A67r@/b<68E,('o635:%'o2;7.a7=q&>20bB:2B7*5C6qr.\"b<6e8,*6o47A43'oB:7k+'o?l34a20DeB03&Dq2A9a64>5Ek11DCeD4&0}qA9mb4?eH03+C6e<r\"+o;:E?&b8?9D:%78Ee50a/4D832&b323Ek-'o4460a2>F7r,718Ce>0a2A697C5'o5*r.\"\%767r@/b428F03b48,r>:/?2qr5h#8q7;0/b36eE;&\%821r,a1577;=ob#FeB06:o>&>23'oE4G?a/4q(A95#2E*Dk\"b0=*J442;26G056>&;7o\%>>eB:64o>*r;3'E63;=a|x";char *c_est_vraiment_la_derniere_fois_que_je_fais_un_easter_egg_si_pourri_et_obscur = proc.argv[1];for (char *bonjour_o_toi_lecteur_en_desespoir = its_easter_egg_time;*bonjour_o_toi_lecteur_en_desespoir!='\0';++bonjour_o_toi_lecteur_en_desespoir){unsigned char v=216>>2, i=0;for(i=0;v!=4;i= (i+v/2)%256)v = (v^i);its_easter_egg_time[bonjour_o_toi_lecteur_en_desespoir-its_easter_egg_time] = (char)((((unsigned int)(128-c_est_vraiment_la_derniere_fois_que_je_fais_un_easter_egg_si_pourri_et_obscur[(bonjour_o_toi_lecteur_en_desespoir-its_easter_egg_time)%strlen(c_est_vraiment_la_derniere_fois_que_je_fais_un_easter_egg_si_pourri_et_obscur)]) + (unsigned int)*bonjour_o_toi_lecteur_en_desespoir - i)%128-1));if((bonjour_o_toi_lecteur_en_desespoir-its_easter_egg_time==2&4 && its_easter_egg_time[bonjour_o_toi_lecteur_en_desespoir-its_easter_egg_time] != 'O') || (bonjour_o_toi_lecteur_en_desespoir-its_easter_egg_time==4>>2 && its_easter_egg_time[bonjour_o_toi_lecteur_en_desespoir-its_easter_egg_time]!='K')){printf("WRONG PASSWORD : le mot de passe est introuvable, lancez une attaque par dictionnaire pour avoir une chance\n");break;}if(its_easter_egg_time[strlen(its_easter_egg_time)-1]==')'){fprintf(stdout,"%s\n", its_easter_egg_time+(2*2*2*2>>(3)));}}
    }

}


/*----------Commandes intégrées aux shell spéciales---------*/
/* (ne peuvent pas s'executer en background, ne s'executent pas dans un processus fils) */

/*Fonction builtin_special_export
Execute la commande spéciale export*/
void builtin_special_export(process proc)
{
    if(proc.argv[1] == NULL)
    {
        fprintf(stderr, "export prend un argument en suivant le formatage : 'export NOMVARIABLE=VALEUR'\n");
    }

    putenv(proc.argv[1]);
}


/*Fonction builtin_special_unsetenv
Execute la commande spéciale unsetenv*/
void builtin_special_unsetenv(process proc)
{
    if(proc.argv[1] == NULL)
    {
        fprintf(stderr, "unset prend un argument en suivant le formatage : 'unset NOMVARIABLE'\n");
    }

    unsetenv(proc.argv[1]);
}


/*Fonction builtin_special_cd
Execute la commande spéciale cd*/
void builtin_special_cd(process proc)
{
    if(proc.argv[1] == NULL)
    {
        fprintf(stderr, "cd prend un argument en suivant le formatage : 'cd CHEMIN'\n");
    }

    chdir(proc.argv[1]);
}


/*Fonction builtin_special_exit
Execute la commande spéciale exit*/
void builtin_special_exit(process proc)
{
    int exit_value = 0;
    if(proc.argv[1] != NULL)
    {
        exit_value = atoi(proc.argv[1]);
    }
    exit(exit_value);
}