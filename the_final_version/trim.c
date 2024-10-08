/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   trim.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tparratt <tparratt@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/16 17:58:55 by tparratt          #+#    #+#             */
/*   Updated: 2024/07/16 18:00:29 by tparratt         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	set_meta(char replace, char *copy, char c, int k)
{
	if (c == '\'' && replace == '$')
		copy[k] = DOLLAR;
	else if (replace == '|')
		copy[k] = PIPE;
	else if (replace == '>')
		copy[k] = RED_OUT;
	else if (replace == '<')
		copy[k] = RED_IN;
	else
		copy[k] = replace;
}

static char	*trim_copy(t_mini *line, char *copy, int i, int j)
{
	int		k;
	char	c;

	k = 0;
	c = line->metaed[i][j];
	while (k < j)
	{
		copy[k] = line->metaed[i][k];
		k++;
	}
	j++;
	while (line->metaed[i][j] != c)
	{
		set_meta(line->metaed[i][j], copy, c, k);
		j++;
		k++;
	}
	j++;
	while (line->metaed[i][j] != '\0')
		copy[k++] = line->metaed[i][j++];
	copy[k] = '\0';
	free(line->metaed[i]);
	return (copy);
}

static int	snip_snip(t_mini *line, int i, int j)
{
	char	*trimmed_string;
	int		len;
	char	c;
	int		count;

	len = ft_strlen(line->metaed[i]);
	c = line->metaed[i][j];
	count = j + 1;
	trimmed_string = malloc((sizeof(char)) * (len - 1));
	if (!trimmed_string)
		malloc_failure_without_token(line);
	while (line->metaed[i][count] != c)
		count++;
	count = count - 1;
	line->metaed[i] = trim_copy(line, trimmed_string, i, j);
	return (count);
}

void	trim_quotes(t_mini *line)
{
	int	i;
	int	j;

	i = 0;
	while (line->metaed[i] != NULL)
	{
		j = 0;
		while (line->metaed[i][j] != '\0')
		{
			if (ft_strncmp(line->metaed[i], "\"\"", 3) == 0
				|| ft_strncmp(line->metaed[i], "''", 3) == 0)
			{
				free(line->metaed[i]);
				line->metaed[i] = ft_strdup("");
				if (!line->metaed[i])
					malloc_failure_without_token(line);
				break ;
			}
			else if (line->metaed[i][j] == '\'' || line->metaed[i][j] == '\"')
				j = snip_snip(line, i, j);
			else
				j++;
		}
		i++;
	}
}
