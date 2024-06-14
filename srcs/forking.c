/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   forking.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: araveala <araveala@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/06 15:18:04 by araveala          #+#    #+#             */
/*   Updated: 2024/06/07 11:10:06 by araveala         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	simple_fork(t_data *data)
{
	data->pid = fork();
	if (data->pid == -1)
	{
		ft_printf("fork error\n");
		exit(1);
	}
	if (data->pid == 0)
	{
		ft_printf("this is the child\n");
		execve(data->tmp->filename, data->tmp->ex_arr, NULL);
//		exit (0);
		return (0);

	}
	if (data->pid > 0)
	{
		ft_printf("this is the parent\n");
//		exit (2);
		return (1);
	}
	return (2);
}
