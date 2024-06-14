/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmds.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vkettune <vkettune@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/22 09:28:04 by vkettune          #+#    #+#             */
/*   Updated: 2024/06/08 14:51:50 by vkettune         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int ft_pwd(t_data *data)
{
	char *temp;

	// ft_printf("%s\n", data->path);
	// trash_dir = "$HOME/.Trash"; // a guess, probably doesn't work
	temp = getcwd(NULL, 0);

	// if (temp == NULL || ft_strncmp(temp, trash_dir, ft_strlen(trash_dir) + 1) == 0)
	if (temp != NULL) //remove
	{
		free(data->path);
		data->path = temp;
	}
	else
	{
		ft_printf("error\n");
		return (1);
	}
	ft_printf("%s\n", data->path);
	free(temp);
	// free(data->path);
	return (0);
}

void ft_cd(t_data *data, t_env *envs, char *rl)
{
	char *temp;
	char *temp2;
	t_tokens *tokens;
	(void)envs;
	(void)rl;

	tokens = data->tokens;
	ft_printf("old data->path: %s\n", data->path);
	if (ft_strncmp(tokens->args[0], "cd", 3) == 0 && tokens->args[1] == NULL)
	{
		find_passage(data, "HOME", 2);
		if (chdir(data->tmp->filename) == 0)
		{
			free(data->tmp->filename);
			free(data->path);
			data->path = getcwd(NULL, 0);
		}
		return ;
	}
	free(tokens->args[0]); // idk how this fixes leaks
	temp = getcwd(NULL, 0);
	if (temp != NULL)
		data->path = temp;
	if (ft_strncmp(tokens->args[1], "/", 1) != 0)
	{
		ft_printf("no / at the start of path\n"); //remove
		temp2 = ft_strjoin("/", tokens->args[1]);
		ft_printf("temp2: %s\n", temp2); //remove
	}
	else
	{
		ft_printf("slash in path\n"); //remove
		temp2 = ft_strdup(tokens->args[1]);
		ft_printf("temp2: %s\n", temp2); //remove
	}
	free(tokens->args[1]); // idk how this fixes leaks
	free(data->path); // idk how this fixes leaks, and actually works
	temp = ft_strjoin(data->path, temp2);
	free(temp2);
	ft_printf("temp: %s\n", temp); //remove
	if (chdir(temp) == 0)
	{
		data->path = getcwd(NULL, 0);
	}
	else
	{
		ft_printf("error\n"); // what error?? we need "return (cmd_error(cmd, msg));"" function
	}
	free(temp);
	ft_printf("new pwd: %s\n", data->path); //remove
	free(data->path);
}

void ft_echo(char *rl)
{
	char	*temp;
	// int i;

	ft_printf("before trim: '%s'\n", rl);
	temp = trim_start(rl);
	ft_printf("after trim: '%s'\n", temp);
	// i = 0;
	// while (args[i++] != '|' || args[i] != NULL)
	// {
	// 	temp = ft_strjoin(args[i], " ");
	// }
	handle_quotes(&temp);
	ft_printf("after handling quotes: '%s'\n", temp);
	ft_printf("%s\n", temp);
}

char	*trim_start(char *str)
{
	char	*c;
	int		i;
	int		j;

	i = 0;
	j = 0;
	c = "echo  ";
	while (str[i] == ' ' || str[i] == '\t')
		i++;
	while (str[i] == c[j] && c[j] != '\0')
	{
		i++;
		j++;
	}
	return (str + i);
}

void	handle_quotes(char **str)
{
	char	*temp;
	char	*temp2;
	int		i;

	i = 0;
	temp = *str;
	// ft_printf("before handling quotes temp: %s\n", temp);
	while (temp[i] != '\0')
	{
		// \" means the string is inside quotes
		if (temp[i] == '\"')
		{
			temp2 = ft_strtrim(temp, "\"");
			temp = temp2;
			free(temp2); // maybe
			break ;
		}
		i++;
	}
	// ft_printf("after temp: %s\n", temp);
	*str = temp;
}

int ft_exit(char *cmd)
{
	free(cmd);
	ft_printf("exit\n");
	exit(0);
	return (1);
}

void	ft_env(t_data *data)
{
	t_env	*env;

	env = data->env;
	// make so that "Env ghjk" does not work and throws an error
	// if (ft_strncmp(rl, "env", 3) != 0)
	// {
	// 	ft_printf("env: command not found\n");
	// 	return ;
	// }
	// if (data->tokens->array_count >= 1)
	// {
	// 	ft_printf("print env in alphabetical order\n");
	// }
	while (env != NULL)
	{
		printf("%s=%s\n", env->key, env->value);
		env = env->next;
	}
}

void	ft_export(t_data *data)
{
	t_env	*env;
	t_tokens *tokens;
	int		i;

	i = 1;
	tokens = data->tokens;
	env = data->env;
	if (tokens->args[1] == NULL)
		printf("no args\n");
	if (data->tokens->array_count == 1)
		printf("print env in alphabetical order\n");
	while (i < data->tokens->array_count)
	{
		handle_arg(data, i, data->tokens);
		i++;
	}
}

void handle_arg(t_data *data, int arg_i, t_tokens *tokens)
{
	char	*arg;
	t_env	*env;
	char	*key;
	
	env = data->env;
	arg = tokens->args[arg_i];
	if (ft_strchr(arg, '=') == NULL)
	{
		ft_printf("export: not a valid identifier\n"); // fix error message
		return ;
	}
	check_char(data, arg_i, 0);
	key = ft_strtrim_front(arg, '=');
	while (env->next != NULL)
	{
		env = env->next;
		if (ft_strncmp(env->key, key, ft_strlen(key)) == 0)
		{
			ft_printf("key exists\n");
			free(env->value); // if issue, use free_str
			env->value = find_value(arg);
			free(key);
			return ;
		}
	}
	ft_printf("key does not exist\n");
	insert_node(&env, key, find_value(arg));
}

char	*find_value(char *arg)
{
	char *value;
	char *temp;

	temp = ft_strchr(arg, '=');
	value = ft_substr(temp, 1, ft_strlen(arg));
	if (value == NULL)
		return (NULL);
	return (value);
}

char	*ft_strtrim_front(char *s1, char set)
{
	int		i;
	char	*trimmed_str;

	i = 0;
	if (!s1 || !set)
		return (NULL);
	while (ft_strchr(&s1[i], set))
	{
		if (s1[i] == '\0')
				return (NULL);
		i++;
	}
	trimmed_str = ft_substr(s1, 0, i - 1);
	if (trimmed_str == NULL)
		return (NULL);
	return (trimmed_str);
}
