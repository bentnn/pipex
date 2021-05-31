#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>
#include <errno.h>
#include <string.h>
#include <stdlib.h>

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
	res = malloc(sizeof(char *) * (size + 1));
	//something
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
	pathi = where_is_path(env);
	if (pathi == -1)
		return (1);
	paths = create_paths(env[pathi]);
	if (!paths)
		return (1);

	//char * command[] = {"-A"};
	//char *command = strdup("-a");
	//printf("%d",execve("/bin/ls", &command, env));
	//printf("%s",strerror(errno));
	//return(0);
	pipe(fd);
	id = fork();
	if (id == 0)
	{
//		write(fd[1], "salam", 5);
//		dup2(fd[1], STDOUT_FILENO);
//		printf("salam");
		char *command = strdup("-l");
		//char * command[] = {"-L"};
//		execve("ls", command, env);
		execve("/bin/pwd", &command, env);
		//waitpid(id,NULL,0);
		close(fd[0]);
		close(fd[1]);
		//return(0);
//		dup2(fd[1], STDOUT_FILENO);
//		close(fd[0]);
//		close(fd[1]);
//		//char * command[] = {"-l"};

//		execve("ls", command, env);
	}
	else
	{
		waitpid(id,NULL,0);
		return (0);
		//wait(0);
		char buf[6];
		while (read(fd[0], &buf, 5))
		{
			buf[5] = '\0';
			printf("%s", buf);
		}
		close(fd[0]);
		close(fd[1]);
//		wait(0);
//		char buf[10];
//		read(fd[0], &buf, 10);
//		printf("%s", buf);
//		close(fd[0]);
//		close(fd[1]);

	}

	return 0;
}
