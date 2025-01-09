# Makefile for libft

# Compiler and flags
CC = cc
CFLAGS = -Wall -Wextra -Werror

# Library name
NAME = libft.a

# Source files
SRC = ft_atoi.c ft_bzero.c ft_isalnum.c ft_isalpha.c ft_isascii.c \
      ft_isdigit.c ft_isprint.c ft_itoa.c ft_memchr.c ft_memcmp.c ft_memcpy.c \
      ft_memmove.c ft_memset.c ft_putendl_fd.c ft_putnbr_fd.c \
      ft_putchar_fd.c ft_putstr_fd.c ft_split.c ft_strchr.c \
      ft_strjoin.c ft_strlcpy.c ft_strlen.c ft_strmapi.c \
      ft_strnstr.c ft_strncmp.c ft_strrchr.c ft_strtrim.c \
      ft_substr.c ft_tolower.c ft_toupper.c ft_calloc.c ft_strdup.c \
      ft_striteri.c ft_strlcat.c gnl/get_next_line.c gnl/get_next_line_utils.c \
	  ft_strcmp.c

BONUS_SRC = ft_lstadd_back.c ft_lstadd_front.c ft_lstclear.c ft_lstdelone.c ft_lstlast.c \
      ft_lstnew.c ft_lstsize.c

# Object files
OBJ = $(SRC:.c=.o)

BONUS_OBJ = $(BONUS_SRC:.c=.o)

# Rule to create the static library
$(NAME): $(OBJ)
	ar rcs $(NAME) $(OBJ)
      
bonus: $(BONUS_OBJ)
	ar rcs $(NAME) $(BONUS_OBJ)


# Rule to compile all source files
all: $(NAME)

# Clean up object files
clean:
	rm -f $(BONUS_OBJ) $(OBJ)

# Clean up object files and the library
fclean: clean
	rm -f $(NAME)

# Rebuild the library
re: fclean all

# Compile source files to object files
%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

.PHONY: all bonus clean fclean re
