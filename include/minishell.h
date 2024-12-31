/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mknsteja <mknsteja@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/25 02:44:01 by mknsteja          #+#    #+#             */
/*   Updated: 2024/12/31 07:27:51 by mknsteja         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
#define MINISHELL_H
#define MAX_ARGS 100

#include <readline/readline.h>
#include <readline/history.h>
#include <signal.h>
#include <string.h> // For memset
#include <unistd.h> // For write
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <stdlib.h>
#include "../libft/libft.h"

typedef enum s_type
{
  NONE,
  PIPES,
  IN,
  OUT,
  APPEND,
  HEREDOC,
  WORD,
  END
}t_type;

typedef struct s_input
{
  char *str;
  struct s_input *prev;
  struct s_input *next;
}t_input;

typedef struct s_ihead
{
	t_input *start;
}t_ihead;

typedef struct s_split
{
	char *str;
	t_type type;
	struct s_split *next;
  struct s_split *prev;
}t_split;

typedef struct s_op
{
	char **str;
	int append;
	int fd_in;
	int fd_out;
	struct s_op *next;
} t_op;

t_split *split_inputs(char *string);
int split_errors(t_split *input);
t_op *initialise_cmd(t_split *input);

#endif // MINISHELL_H
