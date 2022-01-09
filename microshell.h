#ifndef MICROSHELL_H
# define MICROSHELL_H

#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>
#include <dirent.h>

typedef struct s_shell
{
	char *com;
	int coming;
	int previous;	
}			t_shell;


typedef struct s_command
{
	char **com;
	int start;
	int end;	
}			t_command;

#endif