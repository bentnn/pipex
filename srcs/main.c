#include "pipex.h"

int	**create_fd(t_data *data)
{
	int	i;
	int	**fd;
	int	res;

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

int	*create_id(t_data *data)
{
	int	*id;

	id = malloc(sizeof(int) * (data->argc - 3));
	if (!id)
		error_and_exit("Malloc error\n");
	return (id);
}

void	use_cmd(t_data *data)
{
	int	**fd;
	int	i;
	int	*id;

	id = create_id(data);
	fd = create_fd(data);
	id[0] = do_fork(data, fd, id, 0);
	if (id[0] == 0)
		first_cmd(data, (int **) fd);
	i = 3;
	while (i < data->argc - 2)
	{
		id[i - 2] = do_fork(data, fd, id, i);
		if (id[i - 2] == 0)
			central_cmd(data, fd, i);
		i++;
	}
	id[i - 2] = do_fork(data, fd, id, i);
	if (id[i - 2] == 0)
		last_cmd(data, fd);
	close_pipes(fd, data->argc - 4);
	wait_ids(id, data->argc - 3);
	free(id);
}

t_data	*create_data(int argc, char **argv, char **env, int pathi)
{
	t_data	*data;

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

int	main(int argc, char **argv, char **env)
{
	int		pathi;
	t_data	*data;

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
	sleep(8);
	exit(0);
}
