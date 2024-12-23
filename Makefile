# Variables
NAME = minishell
CC = gcc
CFLAGS = -Wall -Wextra -Werror -I/usr/local/include
SRCS = src/main.c src/signal.c
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