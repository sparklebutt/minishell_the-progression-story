/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   forking.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: araveala <araveala@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/12 17:25:52 by araveala          #+#    #+#             */
/*   Updated: 2024/06/12 17:26:41 by araveala         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	simple_fork(t_data *data)
{
	int	ret;
	int	status; // ? not sure why yet
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
	}
	ft_printf("this is the parent\n");
	ret = waitpid(data->pid, &status, 0);
//	free_array(data->tmp->ex_arr);
	close(data->pid);
	return (0);
}
