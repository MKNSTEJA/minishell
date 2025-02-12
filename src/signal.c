/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signal.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ykhattab <ykhattab@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/07 23:44:02 by ykhattab          #+#    #+#             */
/*   Updated: 2025/02/12 23:17:48 by ykhattab         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

void	signal_reset_prompt(int signo)
{
	(void)signo;
	write(1, "\n", 1);
	rl_on_new_line();
	rl_replace_line("", 0);
	// rl_redisplay();
}
/*
SIGQUIT is ctrl + \. So we are ignoring if the user pressed it.
SIGQUIT is used to terminate the process AND dump core (unlike SIGINT). */
void	ignore_sigquit(void)
{
	struct sigaction	act;

	memset(&act, 0, sizeof(act));
	act.sa_handler = SIG_IGN;
	sigaction(SIGQUIT, &act, NULL);
}
/*
sigaction redirects the default behavior of sigint (ctrl+c) to signal_reset_prompt
instead of termination*/
void	set_signals_interactive(void)
{
	struct sigaction	act;

	memset(&act, 0, sizeof(act));
	act.sa_handler = signal_reset_prompt;
	sigaction(SIGINT, &act, NULL);
	ignore_sigquit();
}
