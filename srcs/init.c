/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vkettune <vkettune@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/21 18:11:00 by vkettune          #+#    #+#             */
/*   Updated: 2024/05/22 09:17:53 by vkettune         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void ms_init(t_data *data, char **env)
{
	// fill minishell data struct?
	data->prompt = "\x1b[95mminishell\033[0;37m$ ";
	data->env = NULL;
	data->path = NULL;
	data->pid = 0;
	data->env = env_init(data, env); // copy env to struct
}

// t_env	*env_init(t_data *data, char **env)
// {
// 	// understand env
// 	// copy env to struct
// 	(void)env;
// 	(void)data;
// 	return (0);
// }