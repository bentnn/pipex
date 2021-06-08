NAME = pipex

SRCS = srcs/main.c srcs/central_cmd.c srcs/first_cmd.c srcs/ft_split.c stcs/last_cmd.c srcs/utils.c srcs/work_with_path.c
HEADER = srcs/pipex.h

OFILES = $(patsubst %.c,%.o,$(FILES))
DEPFLAGS = -MMD -MF $(@:.o=.d)

FLAGS = -O3 -Wall -Werror -Wextra

all: 		$(NAME)

$(NAME): 	$(OFILES)
			gcc $(SRCS) -o $(NAME)

clean:
			rm -rf $(DEPS)
			rm -rf $(OFILES)

fclean:	clean
			rm -rf $(NAME)

re: fclean all

.PHONY: all clean fclean re