/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_split.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hounejja <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/24 20:26:44 by hounejja          #+#    #+#             */
/*   Updated: 2024/11/03 13:06:48 by hounejja         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static char	**ft_free(char **tab)
{
	unsigned int	i;

	i = 0;
	if (!tab)
		return (NULL);
	while (tab[i])
		free(tab[i++]);
	free(tab);
	return (NULL);
}

static int	count_words(char const *str, char c)
{
	int	count;
	int	in_word;

	in_word = 0;
	count = 0;
	while (*str)
	{
		if (*str == c)
		{
			in_word = 0;
		}
		else if (in_word == 0)
		{
			in_word = 1;
			count++;
		}
		str++;
	}
	return (count);
}

static char	*get_word(const char *s1, int *index, char c)
{
	char		*copy;
	size_t		word_len;
	size_t		i;

	word_len = 0;
	while (s1[*index] == c)
		(*index)++;
	i = *index;
	while (s1[i] && s1[i] != c)
	{
		word_len++;
		i++;
	}
	copy = malloc(sizeof(char) * (word_len + 1));
	if (!copy)
		return (NULL);
	i = 0;
	while (s1[*index] && s1[*index] != c)
		copy[i++] = s1[(*index)++];
	copy[i] = '\0';
	return (copy);
}

char	**ft_split(char const *s, char c)
{
	int		j;
	int		index;
	char	**tab;
	int		count;

	index = 0;
	j = 0;
	if (!s)
		return (NULL);
	count = count_words(s, c);
	tab = malloc(sizeof(char *) * (count + 1));
	if (!tab)
		return (NULL);
	while (j < count)
	{	
		tab[j] = get_word(s, &index, c);
		if (!tab[j])
			return (ft_free(tab));
		j++;
	}
	tab[j] = NULL;
	return (tab);
}
//int main()
//{
//    const char *str = "Hello world this is a test";
//    char delimiter = ',';    
//    char **result = ft_split(str, delimiter);
//    for (int i = 0; result[i]; i++) {
//        printf("%s", result[i]);
//		free(result[i]);
//    }

//    free(result);
//    return 0;
//}
