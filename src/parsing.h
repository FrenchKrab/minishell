#ifndef PARSING_H
#define PARSING_H

#define MAXSTRSIZE 2048
#define MAXCMD     128
#define MAXPATHSIZE 4096

char* clean_str(char* str);
size_t split_str(char* str, char* tokens[]);
int set_envs(char* tokens[]);

#endif
