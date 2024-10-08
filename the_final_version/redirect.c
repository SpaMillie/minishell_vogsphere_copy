/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirect.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mspasic <mspasic@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/16 18:03:09 by tparratt          #+#    #+#             */
/*   Updated: 2024/07/17 14:47:48 by mspasic          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	redirect_output(t_mini *line, t_tokens *token, int *output)
{
	if (*output == -2 || *output == -1)
		return ;
	if (dup2(*output, STDOUT_FILENO) == -1)
	{
		ft_putendl_fd("minishell: dup2 failed", 2);
		cleanup(line, token, 1);
		exit (1);
	}
	if (close (*output) == -1)
		cleanup_close(line, token);
	*output = -2;
	if (has_output(&token[line->i]))
		line->output_fd = -2;
}

static void	redirect_input(t_mini *line, t_tokens *token, int *input)
{
	if (*input == -2 || *input == -1)
		return ;
	if (dup2(*input, STDIN_FILENO) == -1)
	{
		ft_putendl_fd("minishell: dup2 failed", 2);
		cleanup(line, token, 1);
		exit (1);
	}
	if (close (*input) == -1)
		cleanup_close(line, token);
	*input = -2;
	if (has_input(&token[line->i]))
		line->input_fd = -2;
}

void	redirections(t_mini *line, t_tokens *token, t_fds *cur)
{
	redirect_input(line, token, &cur->in);
	redirect_output(line, token, &cur->out);
	if (cur->close != -2 && close(cur->close) == -1)
		cleanup_close(line, token);
}
