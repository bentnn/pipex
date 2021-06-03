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

size_t	ft_strlen(const char *str)
{
	size_t	size;

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
			write(fd, &str[i], 2);
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

char **full_paths(char **res, int size, const char *path)
{
	int i;
	int j;
	int start;

	i = 5;
	size = 0;
	while (path[i])
	{
		start = i;
		while (path[i] && path[i] != ':')
			i++;
		res[size] = malloc(sizeof(char) * (i - start + 1));
		if (!res[size])
		{
			clear_paths(res, size);
			return (NULL);
		}
		j = 0;
		while (start < i)
		{
			res[size][j] = path[start];
			start++;
			j++;
		}
		res[size++][j] = '\0';
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

void use_cmd(t_data *data)
{
	int fd[data->argc - 4][2];
	int i;
	int res;
	int id[data->argc - 3];

	i = 0;
	while (i < data->argc - 4)
	{
		res = pipe(fd[i]);
		if (res == -1)
			error_and_exit("Error with pipes\n");
		i++;
	}

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
		return (0);
	pathi = where_is_path(env);
	if (pathi == -1)
		return (1);
	data = create_data(argc, argv, env, pathi);
	if (!data)
		return (1);
	use_cmd(data);
	clear_paths(data->paths, -1);
	free(data);
	return (0);
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
