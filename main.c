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

int main(int argc, char **argv, char **env)
{
	int fd[2];
	int id;
	int pathi;
	char **paths;

//	if (argc < 5)
//		return (0);
//	pathi = where_is_path(env);
//	if (pathi == -1)
//		return (1);
//	paths = create_paths(env[pathi]);
//	if (!paths)
//		return (1);
//	int i = 0;
//	while (paths[i])
//	{
//		printf("%s\n", paths[i++]);
//	}
//	clear_paths(paths, -1);
//	return (1);
	//char * command[] = {"-A"};
	//char *command = strdup("-a");
	//printf("%d",execve("/bin/ls", &command, env));
	//printf("%s",strerror(errno));
	//return(0);
	pipe(fd);

	char **command;
	command = malloc(sizeof(char *) * 3);
	id = fork();
	if (id == 0)
	{
		dup2(fd[1], STDOUT_FILENO);
		close(fd[0]);
		close(fd[1]);
//		printf("salam2\n");
		command[0] = "ls";
		command[1] = "-a";
		command[2] = NULL;
		printf("%d\n", execve("/bin/ls", command, env));
		printf("%s\n", strerror(errno));
		return (0);
	}
//	waitpid(id, 0, 0);
	int id2 = fork();




	if (id2 == 0)
	{
		dup2(fd[0], STDIN_FILENO);
//		dup2(fd[1], STDOUT_FILENO);
		close(fd[0]);
		close(fd[1]);
		command[0] = "grep";
		command[1] = "out";
		command[2] = NULL;
		printf("%d\n", execve("/usr/bin/grep", command, env));
		printf("%s\n", strerror(errno));
		return (0);
	}
//	wait(0);
//	waitpid(id2, 0, 0);

//	wait(0);
	int id3 = fork();
	if (id3 == 0)
	{
		dup2(fd[0], STDIN_FILENO);
		close(fd[0]);
		close(fd[1]);
		command[0] = "grep";
		command[1] = "out";
		command[2] = NULL;
		printf("%d\n", execve("/usr/bin/grep", command, env));
		printf("%s\n", strerror(errno));
		return (0);
	}
	wait(0);
//	wait(0);
//	waitpid(id3, 0, 0);

	printf("sasasa\n");
	close(fd[0]);
	close(fd[1]);
	return (0);
}
