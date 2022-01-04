#ifndef MICROSHELL_H
# define MICROSHELL_H

#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>
# include <signal.h>
# include <sys/types.h>
# include <sys/wait.h>
# include <fcntl.h>
# include <dirent.h>
# include <errno.h>

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