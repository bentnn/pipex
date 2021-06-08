#include "pipex.h"

void	clear_paths(char **paths, int size)
{
	int	i;

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

static char	*full_path_line(const char *path, int start, int i)
{
	int		j;
	char	*res;

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

static char	**full_paths(char **res, int size, const char *path)
{
	int	i;
	int	start;

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

char	**create_paths(const char *path)
{
	int		size;
	int		i;
	char	**res;

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

char	*create_path_to_cmd(t_data *data, char *cmd, int pathi)
{
	int		i;
	int		j;
	char	*res;

	res = malloc(sizeof(char) * (ft_strlen(data->paths[pathi])
				+ ft_strlen(cmd) + 2));
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
