# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: mtarza <mtarza@student.1337.ma>            +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2025/01/24 13:05:56 by mtarza13          #+#    #+#              #
#    Updated: 2025/08/02 20:53:21 by mtarza           ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = minishell

CC = cc -g
CFLAGS = -Wall -Wextra -Werror
INCLUDES = -I./include -I./libft
LIBS = -lreadline -I/usr/include/readline -L./libft -lft

SRC_DIR = src
OBJ_DIR = obj
LIBFT_DIR = libft

MAIN_SRCS = src/main.c

BUILTIN_SRCS = builtins/cd.c builtins/echo.c builtins/env.c builtins/exit.c \
			   builtins/export.c builtins/export_utils.c builtins/pwd.c builtins/unset.c

EXECUTOR_SRCS = executor/execute_ast.c executor/execute_builtin.c \
				executor/execute_command.c executor/execute_utils.c executor/redirections.c

LEXER_SRCS = lexer/tokenize.c lexer/token_utils.c

PARSER_SRCS = parser/input.c parser/parse_command.c parser/parse_pipeline.c \
			  parser/parse_redirections.c parser/syntax_validation.c

UTILS_SRCS = utils/ft_malloc.c utils/ft_printf.c utils/ft_utils.c \
			utils/memory_utils.c utils/string_utils.c

ENV_SRCS = env/env_init.c env/env_utils.c

SIGNALS_SRCS = signals/signal_handler.c signals/signal_setup.c

HEREDOC_SRCS = heredoc/heredoc.c heredoc/heredoc_utils.c
EXPO_SRCS = expo/exp_expand.c expo/exp_count.c expo/exp_quote.c expo/exp_utils.c expo/exp_extract.c 
			
SRCS = $(MAIN_SRCS) $(BUILTIN_SRCS) $(EXECUTOR_SRCS) $(LEXER_SRCS) \
	   $(PARSER_SRCS) $(UTILS_SRCS) $(ENV_SRCS) $(SIGNALS_SRCS) $(HEREDOC_SRCS) $(EXPO_SRCS)

OBJS = $(addprefix $(OBJ_DIR)/, $(SRCS:.c=.o))

LIBFT = $(LIBFT_DIR)/libft.a

all: $(LIBFT) $(NAME)

$(NAME): $(OBJS)
	$(CC) $(OBJS) $(LIBS) -o $(NAME)

# Rule to compile source files inside src/ folder (like src/main.c, src/executor/...)
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

# Rule to compile source files outside src/ folder (builtins, utils, env, etc.)
$(OBJ_DIR)/%.o: %.c
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
