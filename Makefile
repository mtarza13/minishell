NAME = minishell
CC = cc
CFLAGS = -g #-Wall -Wextra -Werror

OBJ_DIR = obj
LIBFT_DIR = libft
INCLUDE_DIR = include

INCLUDES = -I./$(INCLUDE_DIR) -I./$(LIBFT_DIR)
LIBFT = $(LIBFT_DIR)/libft.a
LIBS = -lreadline -I/usr/include/readline -L./libft -lft

VPATH = src src/builtins src/env src/executor src/expantion src/heredoc \
		src/lexer src/parser src/signals src/utils

MAIN_SRCS = main.c

BUILTIN_SRCS = cd.c echo.c env.c exit.c export.c export_utils.c pwd.c unset.c

EXECUTOR_SRCS = ft_exec_builtin.c ft_exec_utils.c ft_exec_utils2.c ft_execute.c ft_pre_exec.c ft_redir.c

PARSER_SRCS = prompt.c parser.c scan_token.c util_token.c utils_parse.c valid_syntax.c

UTILS_SRCS = ft_malloc.c ft_printf.c ft_utils.c memory_utils.c

ENV_SRCS = env_init.c env_utils.c

SIGNALS_SRCS = signal_handler.c signal_setup.c

HEREDOC_SRCS = ft_heredoc.c

EXPO_SRCS = expantion_utils.c extract.c pross_extract.c

SRCS = $(MAIN_SRCS) $(BUILTIN_SRCS) $(EXECUTOR_SRCS) \
	   $(PARSER_SRCS) $(UTILS_SRCS) $(ENV_SRCS) $(SIGNALS_SRCS) $(HEREDOC_SRCS) $(EXPO_SRCS)

OBJS = $(addprefix $(OBJ_DIR)/, $(SRCS:.c=.o))

all: $(NAME)

$(NAME): $(LIBFT) $(OBJS)
	$(CC) $(OBJS) $(LIBS) -o $(NAME)

$(OBJ_DIR)/%.o: %.c
	mkdir -p $(dir $@)
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

$(LIBFT):
	make -C $(LIBFT_DIR)

clean:
	rm -rf $(OBJ_DIR)
	make -C $(LIBFT_DIR) clean

fclean: clean
	rm -f $(NAME)
	make -C $(LIBFT_DIR) fclean

re:
	$(MAKE) fclean
	$(MAKE) all

.PHONY: all clean fclean re