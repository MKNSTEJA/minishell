# Variables
NAME = minishell
CC = cc
CFLAGS = -Wall -Wextra -Werror -I/usr/local/include -g -fsanitize=address
# SRCS = src/main.c src/signal.c src/lexer.c src/parser.c src/utils.c src/built-in.c src/execution.c
SRCS = parsing/main.c parsing/lexing/parse_info.c parsing/segregate_info.c parsing/split_errors.c \
       src/signal.c src/utils.c src/echo_exit_pwd.c src/execution.c parsing/expansion/expansion.c parsing/lexing/segments.c \
	   parsing/lexing/handle_parts.c parsing/expansion/append_char_node.c parsing/expansion/expand_utils.c parsing/tokens_utils.c parsing/free_structs.c \
	   parsing/expansion/free_expand.c parsing/expansion/expand_one_token.c parsing/expansion/get_env.c parsing/split_cmds.c parsing/lexing/segments_utils.c \
	   parsing/expansion/expand_utils2.c \
	   src/export.c src/env_utils.c src/handle_cd.c src/apply_redirections.c \
	   src/pipeline_1.c src/pipeline_2.c src/simple_cmd.c src/utils_2.c src/env_utils_2.c src/builtins_env_unset.c src/heredoc_redirections.c 
OBJ_DIR = obj
OBJS = $(addprefix $(OBJ_DIR)/, $(SRCS:.c=.o))
INCLUDES = -I include
LIBFT_DIR = libft
LIBFT = $(LIBFT_DIR)/libft.a
READLINE = -L/usr/local/lib -lreadline

# Targets
all: $(NAME)

$(NAME): $(OBJS) $(LIBFT)
	$(CC) $(CFLAGS) $(OBJS) $(LIBFT) $(READLINE) -o $(NAME)

$(OBJ_DIR)/%.o: %.c
	@mkdir -p $(OBJ_DIR)/$(dir $<)
	$(CC) $(CFLAGS) -c $< -o $@

$(LIBFT):
	$(MAKE) -C $(LIBFT_DIR)

clean:
	$(MAKE) clean -C $(LIBFT_DIR)
	rm -f $(OBJS)

fclean: clean
	$(MAKE) fclean -C $(LIBFT_DIR)
	rm -f $(NAME)

re: fclean all

.PHONY: all clean fclean re