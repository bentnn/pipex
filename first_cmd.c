#include "pipex.h"

static int first_fd_settings(t_data *data, int **fd, int file_fd)
{
	int fd_stdout;

	fd_stdout = dup(STDOUT_FILENO);
	dup2(file_fd, STDIN_FILENO);
	dup2(fd[0][1], STDOUT_FILENO);
	close_pipes(fd, data->argc - 4);
	close(file_fd);
	return (fd_stdout);
}

static void close_and_write(t_data *data, int **fd, int fd_stdout)
{
	close_pipes(fd, data->argc - 4);
	ft_putstr_fd("Malloc error\n", fd_stdout);
}

static char **try_to_launch_first_cmd(t_data *data, int fd_stdout, int **fd)
{
	char **commands;
	char *path;
	int i;

	commands = ft_split(data->argv[2], ' ');
	if (!commands)
	{
		close_and_write(data, fd, fd_stdout);
		exit(1);
	}
	i = 0;
	while (data->paths[i] != NULL)
	{
		path = create_path_to_cmd(data, commands[0], i);
		if (!path)
		{
			close_and_write(data, fd, fd_stdout);
			exit(1);
		}
		execve(path, commands, data->env);
		free(path);
		i++;
	}
	return (commands);
}

void first_cmd(t_data *data, int **fd)
{
	int file_fd;
	char **commands;
	int i;
	int fd_stdout;

	file_fd = open(data->argv[1], O_RDONLY);
	if (file_fd == -1 || read(file_fd, 0, 0) == -1)
	{
		if (file_fd != -1)
			close(file_fd);
		close_pipes(fd, data->argc - 4);
		error_and_exit("Error: Problem with first file\n");
	}
	fd_stdout = first_fd_settings(data, fd, file_fd);
	commands = try_to_launch_first_cmd(data, fd_stdout, fd);
	i = 0;
	while (commands[i])
		free(commands[i++]);
	free(commands);
	ft_putstr_fd("Error: Problem with first command\n", fd_stdout);
	exit(1);
}