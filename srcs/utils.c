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

void	ft_putstr_fd(const char *str, int fd)
{
	int	i;

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

void	error_and_exit(const char *str)
{
	ft_putstr_fd(str, 1);
	exit(1);
}

void	close_pipes(int **fd, int size)
{
	int	i;

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
