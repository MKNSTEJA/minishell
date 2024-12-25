# include "../include/minishell.h"
#include <string.h>

char *handle_delimiter(char *string, char c, int (*i));
void append_list(t_split *input, char *string);
void tokenise(t_split *input);
// t_ihead *parse_info(char *string)
// {
// 	char *trim;
// 	char *start;	
//   	t_input input;
//   	int i = 0;
//
//   	input = ft_calloc(1, sizeof(t_input));
//   	trim = string;
// 	start = string;
//   	while(string[i])
//   	{
// 		if(string[i] == '|')
// 		{
//
// 			input->str = malloc(sizeof(char)*ft_strlen(trim));
// 			ft_strlcpy(input->str, trim, ft_strlen(trim));
// 		}
//     	i++;
//   	}
// }
//
// void append_inputs(t_ihead input, char *start)
// {
// 	char *trim;
// 	int length;
// 	trim = ft_strtrim(start, "|");
// 	length = ft_strlen(trim);
// 	input->str = malloc(sizeof(char)*length);
// 	ft_strlcpy(input->str, trim, length);
// }
//
// void allocate_mem(t_ihead *input)
// {
// 	t_input *ptr;
// 	t_input *back;
//
// 	ptr = (input)->start;
// 	if(!ptr)
// 	{
// 		ptr = ft_calloc(1, sizeof(t_input));
// 		if(!ptr)
// 			exit (-1);
// 		ptr->next = NULL;
// 		ptr->prev = NULL;
// 	}
// 	else
// 	{
// 		while(ptr->next)
// 		{
// 			back = ptr;
// 			ptr = ptr->next;
// 			ptr->prev = back;
// 		}
// 		ptr->next = ft_calloc(1, sizeof(t_input));
// 		if(!ptr->next)
// 			exit(-1);
// 		ptr->next->prev = ptr->next;
// 		ptr->next->next = NULL;
// 	}
// }
// t_input *get_input(char *string)
// {
//   int i = 0;
//   t_ihead *beg;
//   t_input input = malloc(sizeof(t_input));
//   if(!input)
//       return NULL;
//   input->prev = NULL;
//   input->next = NULL;
//   beg->start = input;
//   char **split_inputs = ft_split(string, ' ');
//   while(split_inputs[i])
//   {
//     input->str = malloc(sizeof(char)*ft_strlen(split_inputs[i]));
//     if(input->str)
//         return(NULL);
    
//   }
// }

// void append_ihead(t_ihead *ptr, char *string)
// {
//   t_input *pointer;
//   t_input *bfr;
//   if(ptr->start == NULL)
//   {
//     pointer = malloc(sizeof(t_input));
//     if(!pointer)
//       exit(-1);
//     ptr->start = pointer;
//     pointer->prev = NULL;
//     pointer->next = NULL;
//     if(string == NULL)
//       pointer->str = NULL;
//     else
//     {
//       pointer->str = malloc(sizeof(char) * ft_strlen(string));
// 	  if(!pointer->str)
// 	  	exit(-1);
//       ft_strlcpy(pointer->str, string, ft_strlen(string));
//     }
//   }
//   bfr = ptr->start;
//   while(bfr->next)
//       bfr = bfr->next;
//   pointer = malloc(sizeof(t_input));
//   pointer->prev = bfr;
//   pointer->next = NULL;
//   if(!string)
//     pointer->str = NULL;
//   else
//   {
//     pointer->str = malloc(sizeof(char)*ft_strlen(string));
//     ft_strlcpy(pointer->str, string, ft_strlen(string));
//   }

// }

t_split *first_initialise(char *string, int (*i))
{
	t_split *input;
	(void) i;

	input = malloc(sizeof(t_split));
	if(!input)
		exit(-1);
	while(string[(*i)] == ' ' || string[(*i)] == '\t')
		(*i)++;
	if(string[(*i)] == '\"' || string[(*i)] == '\'')
		input->str = handle_delimiter(string, string[(*i)], i);
	else if (ft_isprint(string[(*i)]) == 1)
		input->str = handle_delimiter(string, ' ', i);
	else 
		input->str = NULL;
	input->next = NULL;
	return(input);
}

t_split *split_inputs(char *string)
{
	int i = 0;
	t_split *input;
	input = first_initialise(string, &i);
	while(string[i])
	{
		while(string[i] == ' ' || string[i] == '\t')
			i++;
		if(string[i] == '\"' || string[i] == '\'')
			append_list(input, handle_delimiter(string, string[i], &i));			
		else if (ft_isprint(string[i]))
			append_list(input, handle_delimiter(string, ' ', &i));
		i++;
	}
	tokenise(input);
	return (input);
}

char *handle_delimiter(char *string, char c, int (*i))
{
	char *result = NULL;
	unsigned int start;
	// unsigned int counter = 0;
	start = (unsigned int) (*i);
	// if(c == ' ' || c == '\t')
	while(string[*i] && string[*i] != '\"' && string[*i] != '\'' && string[*i] != ' ')
		(*i)++;
	if(string[*i] == '\"' || string[*i] == '\'')
	{
		c = string[(*i)++];
		while(string[*i] && string[*i] != c)
			(*i)++;
		(*i)++;	
	}
	result = ft_substr(string, start, (unsigned int)(*i) - start);
	// printf("result: %s\n", result);
	// else
	// {
	// 	while(string[(*i)] && string[(*i)] != c)
	// 	{
	// 		(*i)++;
	// 		counter++;
	// 	}
	// 	result = ft_substr(string, start, counter);
	// }
	// (*i)--;
	return (result);
}

void append_list(t_split *input, char *string)
{
	t_split *new;
	t_split *ptr;
	new = malloc(sizeof(t_list));
	if(!new)
		exit(-1);
	ptr = input;
	new->str = ft_strdup(string);
	if(!ptr)
		exit(-1);
	new->next = NULL;
	if(!input->next)
		input->next = new;
	else
	{
		while(ptr->next)
			ptr = ptr->next;
		ptr->next = new;
	}
}
void tokenise(t_split *input)
{
	t_split *ptr;
	int strlen;

	ptr = input;
	strlen = 0;
	while(ptr)
	{
		strlen = ft_strlen(ptr->str);
		if(ft_strncmp(ptr->str, "|", strlen) == 0)
			ptr->type = PIPES;
		else if(ft_strncmp(ptr->str, "<", strlen) == 0)
			ptr->type = IN;
		else if(ft_strncmp(ptr->str, ">", strlen) == 0)
			ptr->type = OUT;
		else if(ft_strncmp(ptr->str, ">>", strlen) == 0)
			ptr->type = APPEND;
		else if(ft_strncmp(ptr->str, "<<", strlen) == 0)
			ptr->type = HEREDOC;
		else
			ptr->type = WORD;
		ptr = ptr->next;
	}
}