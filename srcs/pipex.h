#ifndef PIPEX_H
# define PIPEX_H

# include <stdio.h>
# include <unistd.h>
# include <sys/wait.h>
# include <errno.h>
# include <string.h>
# include <stdlib.h>
# include <fcntl.h>

typedef struct s_data
{
	int		argc;
	char	**argv;
	char	**env;
	char	**paths;
}				t_data;

char	**ft_split(char const *s, char c);
char	*create_path_to_cmd(t_data *data, char *cmd, int pathi);
void	first_cmd(t_data *data, int **fd);
void	central_cmd(t_data *data, int **fd, int cmd);
void	last_cmd(t_data *data, int **fd);
void	error_and_exit(const char *str);
void	close_pipes(int **fd, int size);
void	ft_putstr_fd(const char *str, int fd);
int		ft_strncmp(const char *str1, const char *str2, size_t num);
int		ft_strlen(const char *str);
char	**create_paths(const char *path);
void	clear_paths(char **paths, int size);
int		where_is_path(char **env);
void	wait_ids(int *id, int size);
int		do_fork(t_data *data, int **fd, int *id, int i);

#endif
