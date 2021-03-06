/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   read_input.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ehelmine <ehelmine@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/08/04 16:02:38 by ehelmine          #+#    #+#             */
/*   Updated: 2021/09/30 13:40:59 by ehelmine         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "includes/minishell.h"

/*
** Read input until it hits newline.
*/

void	loop_input_to_string(char *buf)
{
	int		i;
	int		x;
	char	c;

	i = 0;
	x = read(0, &c, 1);
	if (c == '\0')
	{
		write(1, "\n", 1);
		exit (1);
	}
	while (x > 0)
	{
		buf[i++] = c;
		if (c == '\n')
			break ;
		x = read(0, &c, 1);
	}
	if (x == 0)
	{
		buf[i] = '\0';
		ft_putstr(buf);
		exit (0);
	}
	buf[i - 1] = '\0';
}

static char	*set_buf_and_get_input(void)
{
	char	quote;
	char	*buf;

	buf = (char *)malloc(sizeof(char) * 500);
	check_if_null_ptr((void *)buf);
	ft_memset((void *)buf, 0, 500);
	loop_input_to_string(buf);
	quote = check_quotes_for_input(buf, 0);
	if (quote != 'a')
		loop_more_quotes(buf, quote);
	return (buf);
}

static char	*skip_whitespaces_beginning(char *str)
{
	int		i;
	char	*tmp;

	i = 0;
	while (str[i] != '\0' && ft_isspace(str[i]))
		i++;
	if (str[i] == '\0' || i == 0)
		return (str);
	tmp = ft_strdup(str + i);
	ft_memdel((void *)&str);
	str = ft_strdup(tmp);
	ft_memdel((void *)&tmp);
	return (str);
}

static void	work_with_input(t_shell *data, char **buf_arr)
{
	char	*tmp;

	tmp = NULL;
	buf_arr[0] = skip_whitespaces_beginning(buf_arr[0]);
	if (check_input_array(buf_arr, 0, 0, data) != 0)
	{
		tmp = check_if_built_in(buf_arr);
		if (tmp != NULL)
		{
			execute_built_in(tmp, data, buf_arr);
			ft_memdel((void *)&tmp);
		}
		else
			tmp = check_if_executable(buf_arr, data, 0);
		if (tmp != NULL)
			fork_and_child(data, tmp, buf_arr);
	}
}

/*
** We are in a while (1) -loop until we either get "exit" as input,
** or ctrl + c -keys are used together.
** Set_buf_and_get_input and we get a pointer to char *buf,
** that holds all input (except last newline).
** Split_input_to_array splits input to array (character ';' changes row)
** and checks if it is valid or not.
** If buf_arr isn't NULL (so input is valid), we check len of buf_arr,
** and then we work with the given input in work_with_input. After that
** function is done, we i += 2, so that we skip the next row that might have
** the ';' character, and we go work with next input row.
** After all is executed we free **buf_arr, memdel *buf and output minishell>.
*/

void	while_loop_input(t_shell *data)
{
	char	*buf;
	char	**buf_arr;
	int		len;
	int		i;

	while (1)
	{
		buf = set_buf_and_get_input();
		buf_arr = split_input_to_array(buf, 0);
		if (buf_arr != NULL)
		{
			i = 0;
			len = 0;
			while (buf_arr[len] != NULL)
				len++;
			while (i < len)
			{
				work_with_input(data, buf_arr + i);
				i += 2;
			}
			free_arr((void *)&buf_arr);
		}
		ft_memdel((void *)&buf);
		ft_putstr("minishell> ");
	}
}
