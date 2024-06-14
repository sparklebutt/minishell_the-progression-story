/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_env.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vkettune <vkettune@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/22 12:56:15 by araveala          #+#    #+#             */
/*   Updated: 2024/06/06 18:29:25 by araveala         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char *find_key_name(char *string)
{
	char *name;
	int i;

	i = 0;
	name = NULL;

	while (string[i])
	{
		
		if (string[i] == '=')
		{
			name = ft_substr(string, 0, i);
			return (name);
		}
		i++;
	}
	printf("error in finding key name\n");
	return (0);
}

t_env	*lst_env(t_env *envs)
{
	extern char **environ;
	int i;
	char *key_name;

	t_env *test;

	i = 0;
	while (environ[i] != NULL)
	{
		key_name = find_key_name(environ[i]);
		test = add_new_node(environ[i], key_name);
		addnode(&envs, test);
		i++;
	}
	return (envs);


}

char *find_key_from_envs(t_env *envs, char *key)
{
	t_env *temp;

	temp = envs;
	while (temp->next != NULL)
	{
		ft_printf("key: %s\n", temp->key); // remove
		ft_printf("value: %s\n", temp->value); // remove

		if (ft_strncmp(temp->key, key, ft_strlen(key)) == 0)
			return (temp->value);
		temp = temp->next;
	}
	return (0);
}
