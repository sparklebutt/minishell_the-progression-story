/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pwd.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: araveala <araveala@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/08 15:05:46 by vkettune          #+#    #+#             */
/*   Updated: 2024/06/14 12:12:20 by araveala         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	ft_pwd(t_data *data, t_env *envs)
{
	char	*temp_path;

	temp_path = getcwd(NULL, 0);
	if (temp_path != NULL)
	{
		free(data->path);
		data->path = temp_path;
	}
	else if (temp_path == NULL)
	{
		if (find_key("OLDPWD") != NULL)
		{
			envs = move_list(envs, "OLDPWD");
			if (envs->value != NULL)
				data->path = ft_strdup(envs->value);
			else
				return (call_cmd_error("pwd", "OLDPWD", NULL, -1));
		}
		else
			return (call_cmd_error("pwd", "OLDPWD", NULL, -1));
	}
	ft_printf("%s\n", data->path);
	free(temp_path);
	return (0);
}

t_env	*update_oldpwd(t_data *data, t_env *env, char *temp)
{
	t_env	*temp_env;

	temp_env = data->env;	
	env = move_list(temp_env, "OLDPWD");
	if (env->value != NULL)
		free(env->value);
	env->value = ft_strdup(temp);
	free(temp);
	free_string(data->tmp->env_line);
	return (env);
}

t_env	*fill_old_pwd(t_data *data, t_env *env, char *new_path)
{
	t_env	*temp_env;
	char	*temp;

	temp = NULL;
	temp_env = env;
	if (find_node(env, "PWD", data) == 1)
	{
		temp = ft_strdup(data->tmp->env_line);
		free_string(data->tmp->env_line);
	}
	else
		return (call_env_error("cd", "PWD"));
	// if (ft_strncmp(new_path, temp, ft_strlen(temp)) != 0)
	// {
	// 	free(new_path);
	// 	free(temp);
	// 	return (env);
	// }
	if (find_node(env, "OLDPWD", data) == 1)
		env = update_oldpwd(data, env, temp);
	else
		return (call_env_error("cd", "OLDPWD"));
	env = move_list(temp_env, "PWD");
	if (env->value != NULL)
		free(env->value);
	env->value = ft_strdup(new_path);
	free(new_path);
	return (env);
}
