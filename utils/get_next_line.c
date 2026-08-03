/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hounejja <hounejja@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/19 20:21:05 by hounejja          #+#    #+#             */
/*   Updated: 2024/11/27 21:00:38 by hounejja         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/get_next_line.h"

static char	*ft_strchr(const char *str, int search_str)
{
	size_t	i;

	i = 0;
	while (str[i])
	{
		if (str[i] == (unsigned char)search_str)
		{
			break ;
		}
		i++;
	}
	if (str[i] == (unsigned char)search_str)
	{
		return ((char *)str + i);
	}
	return (0);
}

static char	*keep_rest(char *temp, int start)
{
	char	*rest;
	int		len;

	len = ft_strlen(temp) - start;
	if (len <= 0)
	{
		free(temp);
		return (NULL);
	}
	rest = ft_substr(temp, start, len);
	free(temp);
	return (rest);
}

static char	*extract_line(char *temp, int *start_next)
{
	char	*line;
	int		i;

	i = 0;
	while (temp[i] && temp[i] != '\n')
		i++;
	if (temp[i] == '\n')
		*start_next = i + 1;
	else
	{
		*start_next = i;
		if (i == 0 && temp[i] == '\0')
			return (NULL);
	}
	line = ft_substr(temp, 0, *start_next);
	return (line);
}

static char	*new_line(int fd, char *buffer, char *temp)
{
	ssize_t	read_bytes;
	char	*new_temp;

	if (fd < 0 || BUFFER_SIZE <= 0)
		return (NULL);
	while (!ft_strchr(temp, '\n'))
	{
		read_bytes = read(fd, buffer, BUFFER_SIZE);
		if (read_bytes <= 0)
		{
			if (read_bytes < 0 || *temp == '\0')
			{
				free(temp);
				return (NULL);
			}
			return (temp);
		}
		buffer[read_bytes] = '\0';
		new_temp = ft_strjoin(temp, buffer);
		free(temp);
		temp = new_temp;
	}
	return (temp);
}

char	*get_next_line(int fd)
{
	char		*buffer;
	char		*line;
	static char	*temp;
	int			start_next;

	if (fd < 0 || BUFFER_SIZE <= 0)
		return (NULL);
	if (temp == NULL)
		temp = ft_strdup("");
	buffer = malloc(sizeof(char) * (BUFFER_SIZE + 1));
	if (!buffer)
		return (NULL);
	temp = new_line(fd, buffer, temp);
	free(buffer);
	if (!temp)
		return (NULL);
	line = extract_line(temp, &start_next);
	if (!line)
		return (NULL);
	temp = keep_rest(temp, start_next);
	return (line);
}

//int	main(int argc, char **argv)
//{
//	int		fd;
//	char	*line;

//	if (argc != 2)
//	{
//		fprintf(stderr, "Usage: %s <file_name>\n", argv[0]);
//		return (1);
//	}
//	fd = open(argv[1], O_RDONLY);
//	if (fd < 0)
//	{
//		perror("Error opening file");
//		return (1);
//	}
//	while ((line = get_next_line(fd)) != NULL)
//	{
//		printf("%s", line);
//		free(line);
//	}
//	close(fd);
//	return (0);
//}
