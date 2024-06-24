/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expansion.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tparratt <tparratt@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/24 12:31:15 by tparratt          #+#    #+#             */
/*   Updated: 2024/06/24 15:14:59 by tparratt         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static char	*nothing_to_expand(t_mini *line, char **new_tokens, int loop, int j)
{
	char	*substring;

	substring = get_substring(line->metaed[line->i], j);
	dup_or_join(new_tokens, loop, line->i, substring);
	return (substring);
}

static char	*substring_expand(t_mini *line, char **new_tokens, int loop, int *j, t_tokens *token)
{
	char	*substring;
	char	*env_value;

	(*j)++;
	substring = get_substring(line->metaed[line->i], *j);
	env_value = get_env_value(line->envp, substring, line, token);
	if (!env_value)
		dup_or_join(new_tokens, loop, line->i, "");
	else
		dup_or_join(new_tokens, loop, line->i, env_value);
	free(env_value);
	return (substring);
}

static void	dollars_only(t_mini *line, char **new_tokens, int *loop, int *j)
{
	dup_or_join(new_tokens, *loop, line->i, "$");
	(*j)++;
	(*loop)++;
}

static void	expand(t_mini *line, char **new_tokens, t_tokens *token)
{
	int		j;
	int		loop;
	char	*substring;
	char	*metaed;

	j = 0;
	loop = 0;
	metaed = line->metaed[line->i];
	while (metaed[j])
	{
		if (metaed[j] == '$' && (!ft_isalnum(metaed[j + 1])
				&& metaed[j + 1] != '_' && metaed[j + 1] != '?'))
		{
			dollars_only(line, new_tokens, &loop, &j);
			continue ;
		}
		else if (metaed[j] == '$' && (ft_isalnum(metaed[j + 1])
				|| metaed[j + 1] == '_' || metaed[j + 1] == '?'))
			substring = substring_expand(line, new_tokens, loop, &j, token);
		else
			substring = nothing_to_expand(line, new_tokens, loop, j);
		j += ft_strlen(substring);
		free(substring);
		loop++;
	}
}

void	expansion(t_mini *line, t_tokens *token)
{
	char	**new_tokens;

	line->i = 0;
	new_tokens = malloc_2d(line->metaed);
	while (line->metaed[line->i])
	{
		if (ft_strchr(line->metaed[line->i], '$'))
			expand(line, new_tokens, token);
		else
			duplicate(line, new_tokens, token);
		line->i++;
	}
	new_tokens[line->i] = NULL;
	free_2d(line->metaed);
	line->metaed = new_tokens;
}
