#include "pipex.h"

static int	last_fd_settings(t_data *data, int **fd, int file_fd)
{
	int	fd_stdout;

	fd_stdout = dup(STDOUT_FILENO);
	dup2(fd[data->argc - 5][0], STDIN_FILENO);
	dup2(file_fd, STDOUT_FILENO);
	close_pipes(fd, data->argc - 4);
	close(file_fd);
	return (fd_stdout);
}

static char	**try_to_launch_last_cmd(t_data *data, int fd_stdout)
{
	char	**commands;
	int		i;
	char	*path;

	commands = ft_split(data->argv[data->argc - 2], ' ');
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

void	last_cmd(t_data *data, int **fd)
{
	int		file_fd;
	char	**commands;
	int		i;
	int		fd_stdout;

	file_fd = open(data->argv[data->argc - 1], O_CREAT | O_TRUNC | O_WRONLY);
	if (file_fd == -1 || write(file_fd, 0, 0) == -1)
	{
		if (file_fd != -1)
			close(file_fd);
		close_pipes(fd, data->argc - 4);
		error_and_exit("Error: Problem with last file\n");
	}
	fd_stdout = last_fd_settings(data, fd, file_fd);
	commands = try_to_launch_last_cmd(data, fd_stdout);
	i = 0;
	while (commands[i])
		free(commands[i++]);
	free(commands);
	ft_putstr_fd("Error: Problem with last command\n", fd_stdout);
	exit(1);
}
