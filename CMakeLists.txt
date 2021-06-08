cmake_minimum_required(VERSION 3.19)
project(pipex C)

set(CMAKE_C_STANDARD 99)

add_executable(pipex srcs/main.c srcs/ft_split.c srcs/pipex.h srcs/first_cmd.c srcs/last_cmd.c srcs/central_cmd.c srcs/utils.c srcs/work_with_path.c)