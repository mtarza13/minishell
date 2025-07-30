# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: yabarhda <yabarhda@student.1337.ma>        +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2025/01/24 13:05:56 by mtarza13          #+#    #+#              #
#    Updated: 2025/07/30 04:20:10 by yabarhda         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = minishell

CC = cc
CFLAGS = -Wall -Wextra -Werror
INCLUDES = -I./include -I./libft
LIBS = -lreadline -I/usr/include/readline -L./libft -lft

# Directories
SRC_DIR = src
OBJ_DIR = obj
LIBFT_DIR = libft

# Source files
MAIN_SRCS = main.c

BUILTIN_SRCS = builtins/cd.c builtins/echo.c builtins/env.c builtins/exit.c \
			   builtins/export.c builtins/pwd.c builtins/unset.c

EXECUTOR_SRCS = executor/execute_ast.c executor/execute_builtin.c \
				executor/execute_command.c executor/redirections.c \
				executor/execute_utils.c

LEXER_SRCS = lexer/tokenize.c lexer/token_utils.c

PARSER_SRCS = parser/parse_command.c parser/parse_pipeline.c \
			  parser/parse_redirections.c parser/syntax_validation.c \
			  parser/input.c

UTILS_SRCS = utils/ft_utils.c utils/string_utils.c utils/memory_utils.c utils/ft_malloc.c\
			 

ENV_SRCS = env/env_init.c env/env_utils.c env/env_expand.c env/variable_expand.c

SIGNALS_SRCS = signals/signal_handler.c signals/signal_setup.c

HEREDOC_SRCS = heredoc/heredoc.c heredoc/heredoc_utils.c

# All source files
SRCS = $(MAIN_SRCS) $(BUILTIN_SRCS) $(EXECUTOR_SRCS) $(LEXER_SRCS) \
	   $(PARSER_SRCS) $(UTILS_SRCS) $(ENV_SRCS) $(SIGNALS_SRCS) $(HEREDOC_SRCS)

# Object files
OBJS = $(SRCS:%.c=$(OBJ_DIR)/%.o)

# Libft
LIBFT = $(LIBFT_DIR)/libft.a

# Rules
all: $(LIBFT) $(NAME)

$(NAME): $(OBJS)
	$(CC) $(OBJS) $(LIBS) -o $(NAME)

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

$(LIBFT):
	@make -C $(LIBFT_DIR)

clean:
	@rm -rf $(OBJ_DIR)
	@make -C $(LIBFT_DIR) clean

fclean: clean
	@rm -f $(NAME)
	@make -C $(LIBFT_DIR) fclean

re: fclean all

.PHONY: all clean fclean re