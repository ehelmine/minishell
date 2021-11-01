/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   amount_of_whitespaces.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ehelmine <ehelmine@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/02 16:14:58 by ehelmine          #+#    #+#             */
/*   Updated: 2021/10/02 16:15:40 by ehelmine         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "includes/minishell.h"

int	amount_of_whitespaces(char *str)
{
	int	i;

	i = 0;
	while (ft_isspace(str[i]) && str[i] != '\0')
		i++;
	return (i);
}
