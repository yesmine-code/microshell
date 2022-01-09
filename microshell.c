#include "microshell.h"

int	ft_strlen(char *str)
{
	int		i;

	i = 0;
	while (str[i] != '\0')
		i++;
	return (i);
}

int change_path(char *path)
{
    int ret;
    DIR *d;

    d = opendir(path);
    if (d == NULL)
		return -1;
    ret = chdir(path);
    closedir(d);
    if (ret < 0)
		return -1;
    return (ret);
}

int ft_cd(char **arg)
{
    int ret;

    if (arg[1] == NULL || arg[2] != NULL)
    {
        write(2,"error: cd: bad arguments\n", 26);
        return (0);
    }
    ret = change_path(arg[1]);
	if (ret == -1)
	{
		write(2, "error: cd: cannot change directory to ", 39);
		write(2, arg[1], ft_strlen(arg[1]));
		write(2, "\n", 2);
	}
    return (0);
}

void ft_fatal(void)
{
	write(2, "error: fatal\n", 14);
	exit(EXIT_FAILURE);
}

void	main_func(char **curent_pipe, char **env, int *old_pipe[], t_shell shell)
{
	int new_pipe[2];
	pid_t cpid;
	int ret;

	ret = 0;
	cpid = 0;
	if (strcmp(curent_pipe[0], "cd") == 0)
		ret = ft_cd(curent_pipe);
	else
	{
		if (shell.coming)
		{
			if (pipe(new_pipe) < 0)
				ft_fatal();
		}
		cpid = fork();
		if (cpid < 0)
			ft_fatal();
		else if (cpid == 0) // child
		{
			if (shell.previous == 1 ) // if there is a previous command
			{
				if(dup2(*old_pipe[0], 0) < 0)
					ft_fatal();
				close(*old_pipe[0]);
				close(*old_pipe[1]);
			}
			if (shell.coming == 1) // if there is a coming command
			{
				close(new_pipe[0]);
				if(dup2(new_pipe[1], 1) < 0)
					ft_fatal();
				close(new_pipe[1]);
			}
			ret = execve(curent_pipe[0], curent_pipe, env);
			if (ret == -1)
			{
				write(2, "error: cannot execute ", 23);
				write(2, curent_pipe[0], ft_strlen(curent_pipe[0]));
				write(2, "\n", 1);
			}
			exit(ret);
		}
		else
		{
			if (shell.previous == 1) // previous command
			{
				close(*old_pipe[0]);
				close(*old_pipe[1]);
			}
			if (shell.coming == 1) // comming command
			{
				*old_pipe[0] = new_pipe[0];
				*old_pipe[1] = new_pipe[1];
			}
			waitpid(cpid, NULL, 0);
		}
	}
}

int get_pos_of_c(int ac , char **av, char c)
{
	int i;
	i = 0;
	while (i < ac)
	{
		if (av[i][0] ==  c)
			return i - 1;
		i++;
	}
	return (ac - 1);
}

void get_pipes(t_command com, char **env)
{
	int pos;
	int pipe_start;
	int *old_pipe[2];
	t_shell shell;
	char **curent_pipe;
	int i;

	old_pipe[0]= malloc(sizeof(int));
	old_pipe[1]= malloc(sizeof(int));
	if (old_pipe[0] == NULL || old_pipe[1] == NULL)
		ft_fatal();
	shell.coming = 1;
	shell.previous = 0;
	pos = 0;
	pipe_start = com.start;
	while (pos < com.end)
	{
		pos = pipe_start + get_pos_of_c(com.end - pipe_start + 1, com.com + pipe_start, '|');
		if (pos > com.end)
			break;
		if (pipe_start > com.start)
			shell.previous = 1;
		if (pos == com.end)
			shell.coming = 0;
		curent_pipe = malloc(sizeof(char *) *(pos - pipe_start + 2));
		if (curent_pipe == NULL)
			ft_fatal();
		i = 0;
		while (i < pos - pipe_start + 1)
		{
			curent_pipe[i] = com.com[pipe_start + i];
			i++;
		}
		curent_pipe[i] = NULL;	
		main_func(curent_pipe, env, old_pipe, shell);
		pipe_start = pos + 2;
		free(curent_pipe);
	}
	free(old_pipe[0]);
	free(old_pipe[1]);
}

int main(int ac , char **av, char **env)
{
	int pos;
	int start = 1;
	t_command com;

	com.com = av;
	pos = 0;
	while (pos < ac - 1)
	{
		pos = start + get_pos_of_c(ac - start, av + start, ';');
		com.start = start;
		com.end = pos;
		if (start <= pos)
			get_pipes(com, env);
		start = pos + 2;
	}
	exit(EXIT_SUCCESS);
}