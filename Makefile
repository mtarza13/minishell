NAME = minishell
CC = cc
CFLAGS = -Wall -Wextra -Werror

OBJ_DIR = obj
LIBFT_DIR = libft
INCLUDE_DIR = include

INCLUDES = -I./$(INCLUDE_DIR) -I./$(LIBFT_DIR)
LIBFT = $(LIBFT_DIR)/libft.a
LIBS = -lreadline -I/usr/include/readline -L./libft -lft

VPATH = src src/builtins src/env src/executor src/expantion src/heredoc \
		src/lexer src/parser src/signals src/utils

MAIN_SRCS = main.c

BUILTIN_SRCS = ft_cd.c ft_echo.c ft_env.c ft_exit.c ft_export.c ft_export_utils.c ft_pwd.c ft_unset.c

EXECUTOR_SRCS = ft_exec_builtin.c ft_exec_utils.c ft_exec_utils2.c ft_execute.c ft_pre_exec.c ft_redir.c

PARSER_SRCS = ft_prompt.c ft_parser.c ft_scan_token.c ft_util_token.c ft_utils_parse.c ft_valid_syntax.c

UTILS_SRCS = ft_malloc.c ft_printf.c ft_utils.c ft_memory_utils.c

ENV_SRCS = ft_env_init.c ft_env_utils.c

SIGNALS_SRCS = ft_signal_handler.c ft_signal_setup.c

HEREDOC_SRCS = ft_heredoc.c

EXPO_SRCS = ft_expantion_utils.c ft_extract.c ft_pross_extract.c

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