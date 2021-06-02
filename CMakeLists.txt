cmake_minimum_required(VERSION 3.19)
project(pipex C)

set(CMAKE_C_STANDARD 99)

add_executable(pipex main.c ft_split.c pipex.h)