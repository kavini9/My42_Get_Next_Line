/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wweerasi <wweerasi@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/26 17:05:58 by wweerasi          #+#    #+#             */
/*   Updated: 2024/06/02 20:25:08 by wweerasi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"
#include <string.h>
#include <stdio.h>
#include <fcntl.h>

char	*ft_free(char *s)
{
	free(s);
	s = NULL;
	return (s);
}

static char *l_construct(char **stash)
{
	char *line;
	char *res;

	if (**stash)
	{
		line = ft_substr(*stash, 0, (ft_strchr(*stash,'\n') - *stash  + 1));
		if (line)
        {
            res = ft_strdup(ft_strchr(*stash, '\n') + 1);
			ft_free(*stash);
            *stash = ft_strdup(res);
            ft_free(res);
        }
        else
            ft_free(*stash);
	}
	else
		line  = ft_strdup("");
	return (line);
}

static char *l_read(int fd, char *buf, char **stash)
{
    int     bytes_read;
    char    *tmp_stash;
    char    *line; 

	line = NULL;
    bytes_read = BUFFER_SIZE;
	if (!*stash || eof)
		return (NULL);
    while(!ft_strchr(*stash, '\n'))//bytes_read are useless here because it updates.
    {
        bytes_read = read(fd, buf, BUFFER_SIZE);
		printf("bytes_read: %i\n", bytes_read);
		if (bytes_read == -1)
            return (ft_free(*stash));
        if (bytes_read == 0)
			return (ft_free(*stash));
        buf[bytes_read] = '\0';
		tmp_stash = *stash;
        *stash = ft_strjoin(tmp_stash, buf);
        ft_free(tmp_stash);
		if (!stash)
			return(ft_free(*stash));
	}
	printf("stash: %s\n", *stash);
	line = l_construct(stash);
	return(line);
}

char *get_next_line(int fd)
{
	static char	*stash;
	char		buffer[BUFFER_SIZE + 1];
	char		*line;

    if (fd < 0 || BUFFER_SIZE <= 0 || BUFFER_SIZE > INT_MAX || read(fd, NULL, 0) < 0)
        return (NULL);
    if (!stash)
        stash = ft_strdup("");
	if (!stash)
		return (NULL);
    line = l_read(fd, buffer, &stash);
    if (!line)
	{
        line = ft_free(line);
		if (!stash)
			return(ft_free(stash));
	}
    return (line);
}

/*
//main to see the output of strjoin
int main(void)
{
	char *s1;
	char *s2 = "abc";
	char *j;

	j = ft_strjoin(s1, s2);
	printf("ft_strjoin: %s\n", j);
	free(j);
}*/
/*
//main to test output of a empty line
int main(void)
{
	int	fd;
	int		bytes_read;
	char		buf[BUFFER_SIZE + 1];

	fd = open("Empty.txt", O_RDONLY);
	bytes_read = read(fd, buf, BUFFER_SIZE);
	printf("Buffer: -%s-, Bytes_read: %i\n", buf, bytes_read);
}*/

//main to test GNL function
int main()
{
	int fd;
	char *line;
	static int i;
	char *test = "1234567k9abcdef";
	int len;

	printf("test len: %lu\n", strlen(test));
	len = ft_strchr(test, 'k') - test;
	printf("strchr_len: %i\n", len);
	line = ft_strdup("");
	fd = open("ISF.txt", O_RDONLY);
	while (i < 30)
	{
		line = get_next_line(fd);
		printf("line [%i] : %s", i++, line);
		free(line);
	}
}
/*
//main to test if  line extraction logic works
int main(void)
{
	int fd;
	char *stash;
	char *line;
	char *res;

	line = ft_strdup("");
	fd = open("ISF.txt", O_RDONLY);
	while (line)
	{
		stash = get_next_line(fd);
		printf("stash: %s", stash);

		line = ft_substr(stash, 0, (ft_strchr(stash,'\n') - stash  + 1));
		printf("line: %s", line);
		if (line)
		{
			res = ft_strdup(ft_strchr(stash, '\n') + 1);
			stash = ft_strdup(res);
			printf("res: %s\n", res);
			ft_free(res);
		}
		else 
			ft_free(stash);
	}
}*/



//Theres a error when EOF is reached but called the function again.
//This ->: a.out(9610,0x117b515c0) malloc: *** error for object 0x7ff65d402a10: pointer being freed was not allocated
//This code does not consider the matter when there is no new line but a null terminator ending. and we cannot add a check to while loop for null terminator because the stash itself is initiated with a null terminator.

