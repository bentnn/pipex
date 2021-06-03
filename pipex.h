#ifndef PIPEX_H
#define PIPEX_H

#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>
#include <errno.h>
#include <string.h>
#include <stdlib.h>

typedef struct s_data
{
	int argc;
	char **argv;
	char **env;
	char **paths;
}				t_data;

char	**ft_split(char const *s, char c);

#endif
