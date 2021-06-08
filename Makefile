NAME = pipex

SRCS = srcs/main.c srcs/central_cmd.c srcs/first_cmd.c srcs/ft_split.c srcs/last_cmd.c srcs/utils.c srcs/work_with_path.c
HEADER = srcs/pipex.h


OBJS := $(SRCS:.c=.o)

FLAGS = -Wall -Wextra -Werror

all: 		$(NAME)


$(NAME): 	$(OBJS) $(HEADER)
			gcc $(FLAGS) $(OBJS) -o $(NAME)


clean:
			rm -rf $(OBJS)

fclean:	clean
			rm -rf $(NAME)

re: fclean all

.PHONY: all clean fclean re