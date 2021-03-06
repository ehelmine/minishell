/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   check_if_something_functions.c                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ehelmine <ehelmine@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/08/17 17:49:21 by ehelmine          #+#    #+#             */
/*   Updated: 2021/09/30 14:48:14 by ehelmine         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "includes/minishell.h"

char	*check_if_built_in(char **buf_arr)
{
	int		i;
	char	*if_built_in;
	int		len;

	i = 0;
	while (ft_isspace(buf_arr[0][i]) && buf_arr[0][i] != '\0')
		i++;
	len = 0;
	while (!ft_isspace(buf_arr[0][i + len]) && buf_arr[0][i + len] != '\0')
		len++;
	if_built_in = return_string_before_given_char(buf_arr[0] + i,
			buf_arr[0][i + len]);
	if (if_built_in == NULL)
		if_built_in = ft_strdup(buf_arr[0]);
	if (ft_strequ("cd", if_built_in) || ft_strequ("exit", if_built_in)
		|| ft_strequ("env", if_built_in) || ft_strequ("unsetenv", if_built_in)
		|| ft_strequ("setenv", if_built_in) || ft_strequ("echo", if_built_in))
		return (if_built_in);
	ft_memdel((void *)&if_built_in);
	return (NULL);
}

int	only_slashes_and_dots(char *command)
{
	int	i;

	i = 0;
	if (ft_strequ(command, "."))
		ft_printf(".: not enough arguments\n");
	else if (ft_strequ(command, ".."))
		ft_printf("minishell: ..: command not found\n");
	else
	{
		while (command[i] != '\0')
		{
			if (command[i] != '.' && command[i] != '/')
				return (1);
			i++;
		}
	}
	ft_memdel((void *)&command);
	return (-1);
}

char	*pair_path_and_exec(char *if_exec, t_shell *data, struct stat b)
{
	char	*path;
	char	*path_and_exec;

	data->i = 0;
	if (data->path_array == NULL)
		return (NULL);
	while (data->path_array[data->i] != NULL)
	{
		path = ft_strjoin(data->path_array[data->i], "/");
		path_and_exec = ft_strjoin(path, if_exec);
		if (stat(path_and_exec, &b) == 0 && b.st_mode & S_IXUSR
			&& (S_ISREG(b.st_mode)))
		{
			if (data->original_exec == NULL)
				data->original_exec = ft_strdup(if_exec);
			free_two((void *)&if_exec, (void *)&path);
			return (path_and_exec);
		}
		free_two((void *)&path, (void *)&path_and_exec);
		data->i++;
	}
	return (NULL);
}

char	*check_if_executable(char **arr, t_shell *data, int len)
{
	struct stat	b;
	char		*if_exec;
	char		*path_and_exec;

	update_path_array(data);
	while (!ft_isspace(arr[0][data->i + len]) && arr[0][data->i + len] != '\0')
		len++;
	if_exec = return_string_before_given_char(arr[0] + data->i,
			arr[0][data->i + len]);
	if (only_slashes_and_dots(if_exec) == -1)
		return (NULL);
	if (stat(if_exec, &b) == 0 && b.st_mode & S_IXUSR && S_ISREG(b.st_mode)
		&& ft_strchr(if_exec, '/') != NULL)
	{
		if (data->original_exec == NULL)
			data->original_exec = ft_strdup(if_exec);
		return (if_exec);
	}
	path_and_exec = pair_path_and_exec(if_exec, data, b);
	if (path_and_exec == NULL)
		return (error_message_not_exec(if_exec));
	return (path_and_exec);
}

char	*check_if_exec_with_quotes(char *if_exec, t_shell *data)
{
	struct stat	b;
	char		*path_and_exec;

	update_path_array(data);
	if (stat(if_exec, &b) == 0 && b.st_mode & S_IXUSR && S_ISREG(b.st_mode)
		&& ft_strchr(if_exec, '/') != NULL)
	{
		if (data->original_exec == NULL)
			data->original_exec = ft_strdup(if_exec);
		path_and_exec = ft_strdup(if_exec);
		ft_memdel((void *)&if_exec);
		return (path_and_exec);
	}
	path_and_exec = pair_path_and_exec(if_exec, data, b);
	if (path_and_exec == NULL)
		return (error_message_not_exec(if_exec));
	return (path_and_exec);
}
