#include "pipex.h"

int	ft_strncmp(const char *str1, const char *str2, size_t num)
{
	size_t	pos;

	pos = 0;
	if (!num)
		return (0);
	while (pos < num - 1 && str1[pos] && str1[pos] == str2[pos])
		pos++;
	return ((unsigned char)str1[pos] - (unsigned char)str2[pos]);
}

int	ft_strlen(const char *str)
{
	int	size;

	size = 0;
	while (*str)
	{
		size++;
		str++;
	}
	return (size);
}

void ft_putstr_fd(const char *str, int fd)
{
	int i;

	if (str)
	{
		i = 0;
		while (str[i])
		{
			write(fd, &str[i], 1);
			i++;
		}
	}
}

int where_is_path(char **env)
{
	int i;

	i = 0;
	while (env[i])
	{
		if (ft_strlen(env[i]) > 5 && ft_strncmp(env[i], "PATH=", 5) == 0)
			return (i);
		i++;
	}
	return (-1);
}

void clear_paths(char **paths, int size)
{
	int i;

	i = 0;
	if (size == -1)
	{
		while (paths[i])
		{
			free(paths[i]);
			i++;
		}
		free(paths);
		return ;
	}
	while (i < size)
	{
		free(paths[i]);
		i++;
	}
	free(paths);
}

char *full_path_line(const char *path, int start, int i)
{
	int j;
	char *res;

	res = malloc(sizeof(char) * (i - start + 1));
	if (!res)
		return (NULL);
	j = 0;
	while (start < i)
	{
		res[j] = path[start];
		start++;
		j++;
	}
	res[j] = '\0';
	return (res);
}

char **full_paths(char **res, int size, const char *path)
{
	int i;
	int start;

	i = 5;
	size = 0;
	while (path[i])
	{
		start = i;
		while (path[i] && path[i] != ':')
			i++;
		res[size] = full_path_line(path, start, i);
		if (!res[size])
		{
			clear_paths(res, size);
			return (NULL);
		}
		size++;
		i += (path[i] == ':');
	}
	res[size] = NULL;
	return (res);
}

char **create_paths(const char *path)
{
	int size;
	int i;
	char **res;

	i = 5;
	size = 0;
	while (path[i])
	{
		if (path[i] == ':')
			size++;
		i++;
	}
	if (size == 0 && i == 5)
		return (NULL);
	res = malloc(sizeof(char *) * (size + 2));
	if (!res)
		return (NULL);
	res = full_paths(res, size, path);
	if (!res)
		res = NULL;
	return (res);
}

void error_and_exit(const char *str)
{
	ft_putstr_fd(str, 1);
	exit(1);
}

void close_pipes(int **fd, int size)
{
	int i;

	i = 0;
	while (i < size)
	{
		close(fd[i][0]);
		close(fd[i][1]);
		free(fd[i]);
		i++;
	}
	free(fd);
}

char *create_path_to_cmd(t_data *data, char *cmd, int pathi)
{
	int i;
	int j;
	char *res;

	res = malloc(sizeof(char) * (ft_strlen(data->paths[pathi]) +
			ft_strlen(cmd) + 2));
	if (!res)
		return (NULL);
	i = 0;
	j = 0;
	while (data->paths[pathi][j])
	{
		res[i] = data->paths[pathi][j++];
		i++;
	}
	res[i++] = '/';
	j = 0;
	while (cmd[j])
	{
		res[i] = cmd[j++];
		i++;
	}
	res[i] = '\0';
	return (res);
}

void central_cmd(t_data *data, int **fd, int cmd)
{
	char *path;
	char **commands;
	int i;
	int fd_stdout;

	fd_stdout = dup(STDOUT_FILENO);
	dup2(fd[cmd - 3][0], STDIN_FILENO);
	dup2(fd[cmd - 2][1], STDOUT_FILENO);
	close_pipes(fd, data->argc - 4);
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
			ft_putstr_fd("Error with malloc\n", fd_stdout);
			exit(1);
		}
		execve(path, commands, data->env);
		i++;
	}
	ft_putstr_fd("Error: Problem with command \"", fd_stdout);
	ft_putstr_fd(data->argv[cmd], fd_stdout);
	ft_putstr_fd("\"\n", fd_stdout);
	exit(1);
}

void last_cmd(t_data *data, int **fd)
{
	int file_fd;
	char *path;
	char **commands;
	int i;
	int fd_stdout;

	file_fd = open(data->argv[data->argc - 1], O_CREAT | O_TRUNC| O_WRONLY);
	if (file_fd == -1 || write(file_fd, 0, 0) == -1)
	{
		if (file_fd != -1)
			close(file_fd);
		close_pipes(fd, data->argc - 4);
		error_and_exit("Error: Problem with last file\n");
	}
	fd_stdout = dup(STDOUT_FILENO);
	dup2(fd[data->argc - 5][0], STDIN_FILENO);
	dup2(file_fd, STDOUT_FILENO);
	close_pipes(fd, data->argc - 4);
	close(file_fd);
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
			ft_putstr_fd("Error with malloc\n", fd_stdout);
			exit(1);
		}
		execve(path, commands, data->env);
		free(path);
		i++;
	}
	ft_putstr_fd("Error: Problem with last command\n", fd_stdout);
	exit(1);
}

void wait_ids(int *id, int size)
{
	int i;

	i = 0;
	while (i < size)
	{
		waitpid(id[i], 0, 0);
		i++;
	}
}

int **create_fd(t_data *data)
{
	int i;
	int **fd;
	int res;

	fd = malloc(sizeof(int *) * (data->argc - 3));
	if (!fd)
		return (NULL);
	i = 0;
	while (i < data->argc - 4)
	{
		fd[i] = malloc(sizeof(int) * 2);
		if (!fd[i])
			error_and_exit("Malloc error\n");
		res = pipe(fd[i]);
		if (res == -1)
			error_and_exit("Error with pipes\n");
		i++;
	}
	fd[i] = NULL;
	return (fd);
}

void use_cmd(t_data *data)
{
	int **fd;
	int i;
	int id[data->argc - 3];

	fd = create_fd(data);
	id[0] = fork();
	if (id[0] == -1)
	{
		close_pipes(fd, data->argc - 4);
		error_and_exit("Error with fork\n");
	}
	if (id[0] == 0)
		first_cmd(data, (int **) fd);
	i = 3;
	while (i < data->argc - 2)
	{
		id[i - 2] = fork();
		if (id[i - 2] == -1)
		{
			close_pipes(fd, data->argc - 4);
			wait_ids(id, i - 2);
			error_and_exit("Error with fork\n");
		}
		if (id[i - 2] == 0)
			central_cmd(data, fd, i);
		i++;
	}
	id[i - 2] = fork();
	if (id[i - 2] == -1)
	{
		close_pipes(fd, data->argc - 4);
		error_and_exit("Error with fork\n");
	}
	if (id[i - 2] == 0)
		last_cmd(data, fd);
	close_pipes(fd, data->argc - 4);
	wait_ids(id, data->argc - 3);
}

t_data *create_data(int argc, char **argv, char **env, int pathi)
{
	t_data *data;

	data = malloc(sizeof(t_data));
	if (!data)
	{
		ft_putstr_fd("Malloc error\n", 1);
		return (NULL);
	}
	data->paths = create_paths(env[pathi]);
	if (!data->paths)
	{
		free(data);
		ft_putstr_fd("Error with PATH\n", 1);
		return (NULL);
	}
	data->argc = argc;
	data->argv = argv;
	data->env = env;
	return (data);
}

int main(int argc, char **argv, char **env)
{
	int pathi;
	t_data *data;


	if (argc < 5)
		error_and_exit("Five or more arguments expected\n");
	pathi = where_is_path(env);
	if (pathi == -1)
		error_and_exit("Expected PATH in env\n");
	data = create_data(argc, argv, env, pathi);
	if (!data)
		error_and_exit("Malloc error\n");
	use_cmd(data);
	clear_paths(data->paths, -1);
	free(data);
	exit(0);
}

//int main(int argc, char **argv, char **env)
//{
//	int fd[2];
//	int fd2[2];
//	int id;
//	int pathi;
//	char **paths;
//
////	if (argc < 5)
////		return (0);
////	pathi = where_is_path(env);
////	if (pathi == -1)
////		return (1);
////	paths = create_paths(env[pathi]);
////	if (!paths)
////		return (1);
////	int i = 0;
////	while (paths[i])
////	{
////		printf("%s\n", paths[i++]);
////	}
////	clear_paths(paths, -1);
////	return (1);
//	//char * command[] = {"-A"};
//	//char *command = strdup("-a");
//	//printf("%d",execve("/bin/ls", &command, env));
//	//printf("%s",strerror(errno));
//	//return(0);
//	pipe(fd);
//	pipe(fd2);
//	char **command;
//	command = malloc(sizeof(char *) * 3);
//	id = fork();
//
//	if (id == -1)
//	{
//		printf("Error\n");
//		return (0);
//	}
//	if (id == 0)
//	{
//		dup2(fd[1], STDOUT_FILENO);
//		close(fd[0]);
//		close(fd[1]);
//		close(fd2[0]);
//		close(fd2[1]);
//		command[0] = "ls";
//		command[1] = "-a";
//		command[2] = NULL;
////		printf("%d\n", execve("/bin/ls", command, env));
////		printf("%s\n", strerror(errno));
//		return (0);
//	}
//	printf("sasasa1\n");
//
//	int id2 = fork();
//	if (id2 == -1)
//	{
//		printf("Error\n");
//		return (0);
//	}
//	if (id2 == 0)
//	{
//		dup2(fd[0], STDIN_FILENO);
////		dup2(fd2[1], STDOUT_FILENO);
//		close(fd[0]);
//		close(fd[1]);
//		close(fd2[0]);
//		close(fd2[1]);
//		command[0] = "grep";
//		command[1] = "out";
//		command[2] = NULL;
//		printf("%d\n", execve("/usr/bin/grep", command, env));
//		printf("%s\n", strerror(errno));
//		return (0);
//	}
////	sleep(5);
////	printf("sasasa2\n");
////	char buf[3];
////	read(fd[0], &buf, 2);
////	buf[2] = '\0';
////	printf("%s\n", buf);
////	close(fd[0]);
////	close(fd[1]);
////	waitpid(id2, 0, 0);
////	return (0);
//	printf("sasasa2\n");
//
//	int id3 = fork();
//	if (id3 == -1)
//	{
//		printf("Error\n");
//		return (0);
//	}
//	if (id3 == 0)
//	{
//		dup2(fd2[0], STDIN_FILENO);
//		close(fd[0]);
//		close(fd[1]);
//		close(fd2[0]);
//		close(fd2[1]);
//		command[0] = "grep";
//		command[1] = "out";
//		command[2] = NULL;
//		printf("%d\n", execve("/usr/bin/grep", command, env));
//		printf("%s\n", strerror(errno));
//		return (0);
//	}
////	dup2(fd[0], STDIN_FILENO);
//	close(fd[0]);
//	close(fd[1]);
//	close(fd2[0]);
//	close(fd2[1]);
////	command[0] = "grep";
////	command[1] = "out";
////	command[2] = NULL;
////	printf("sasasa3\n");
////	printf("%d\n", execve("/usr/bin/grep", command, env));
////	printf("%s\n", strerror(errno));
////	return (0);
//	printf("sasasa3\n");
//
////	close(fd[0]);
////	close(fd[1]);
////	free(command);
//	waitpid(id, 0, 0);
//	printf("sasasa4\n");
//	waitpid(id2, 0, 0);
//	printf("sasasa5\n");
//	waitpid(id3, 0, 0);
//	printf("sasasa6\n");
//	return (0);
//}
