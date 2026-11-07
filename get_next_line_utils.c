/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line_utils.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sbadandi <sbadandi@learner.42.tech>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/11/02 21:02:58 by sbadandi          #+#    #+#             */
/*   Updated: 2026/11/07 15:29:18 by sbadandi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"

size_t	ft_str_len(char *s)
{
	size_t	i;

	if (!s)
		return (0);
	i = 0;
	while (s[i])
		i++;
	return (i);
}

char	*ft_strjoin(char *s1, char *s2)
{
	char	*sup;
	int		i;
	int		j;

	if (!s2)
		return (NULL);
	i = 0;
	sup = malloc(sizeof(char) * (ft_str_len(s1) + ft_str_len(s2) + 1));
	if (!sup)
		return (NULL);
	while (s1 && s1[i])
	{
		sup[i] = s1[i];
		i++;
	}
	j = 0;
	while (s2[j])
		sup[i++] = s2[j++];
	sup[i] = '\0';
	free(s1);
	return (sup);
}

char	*ft_strchr(char *s, int c)
{
	int	i;

	i = 0;
	if (!s)
		return (NULL);
	if (c == '\0')
		return ((char *)&s[ft_str_len(s)]);
	while (s[i])
	{
		if (s[i] == (char)c)
			return ((char *)&s[i]);
		i++;
	}
	return (NULL);
}
