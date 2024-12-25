# Variables
NAME = minishell
CC = gcc
CFLAGS = -Wall -Wextra -Werror
SRCS = parsing/main.c parsing/parse_info.c
OBJS = $(SRCS:.c=.o)
INCLUDES = -I include
LIBFT_DIR = libft
LIBFT = $(LIBFT_DIR)/libft.a
READLINE = -lreadline

# Targets
all: $(NAME)

$(NAME): $(OBJS) $(LIBFT)
	$(CC) $(CFLAGS) $(OBJS) $(LIBFT) $(READLINE) -o $(NAME)

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
