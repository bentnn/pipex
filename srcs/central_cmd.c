#include "pipex.h"

static char	**try_to_launch_central_cmd(t_data *data, int fd_stdout, int cmd)
{
	char	**commands;
	char	*path;
	int		i;

	commands = ft_split(data->argv[cmd], ' ');
	if (!commands)
	{
		ft_putstr_fd("Malloc error\n", fd_stdout);
		exit(1);
	}
	i = 0;
	while (data->paths[i] != NULL)
	{
		path = create_path_to_cmd(data, commands[0], i);
		if (!path)
		{
			ft_putstr_fd("Malloc error\n", fd_stdout);
			exit(1);
		}
		execve(path, commands, data->env);
		free(path);
		i++;
	}
	return (commands);
}

void	central_cmd(t_data *data, int **fd, int cmd)
{
	char	**commands;
	int		i;
	int		fd_stdout;

	fd_stdout = dup(STDOUT_FILENO);
	dup2(fd[cmd - 3][0], STDIN_FILENO);
	dup2(fd[cmd - 2][1], STDOUT_FILENO);
	close_pipes(fd, data->argc - 4);
	commands = try_to_launch_central_cmd(data, fd_stdout, cmd);
	i = 0;
	while (commands[i])
		free(commands[i++]);
	free(commands);
	ft_putstr_fd("Error: Problem with command \"", fd_stdout);
	ft_putstr_fd(data->argv[cmd], fd_stdout);
	ft_putstr_fd("\"\n", fd_stdout);
	exit(1);
}

int	where_is_path(char **env)
{
	int	i;

	i = 0;
	while (env[i])
	{
		if (ft_strlen(env[i]) > 5 && ft_strncmp(env[i], "PATH=", 5) == 0)
			return (i);
		i++;
	}
	return (-1);
}

void	wait_ids(int *id, int size)
{
	int	i;

	i = 0;
	while (i < size)
	{
		waitpid(id[i], 0, 0);
		i++;
	}
}

int	do_fork(t_data *data, int **fd, int *id, int i)
{
	int	our_id;

	our_id = fork();
	if (our_id == -1)
	{
		close_pipes(fd, data->argc - 4);
		if (i != 0)
			wait_ids(id, i - 2);
		error_and_exit("Error with fork\n");
	}
	return (our_id);
}
