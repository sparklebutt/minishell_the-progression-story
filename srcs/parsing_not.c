/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing_not.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vkettune <vkettune@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/22 16:00:43 by araveala          #+#    #+#             */
/*   Updated: 2024/06/12 15:17:57 by araveala         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include  "minishell.h"

// note that running cd .. ls(or other) or cd ../ ls(or other) , only take you back a dir, does not interact with other commands 

// the idea now is that i split the readline by space into a 2d array, i am not considering if there are spaces inbetween quotes with commands eg "ls    " | grep "objs    " , this should command not found anyway
// flags will also be seperate 

// string is rl
void collect_cmd_array(t_tokens *tokens, char *string)
{
	int i;
	int x;
	char *tmp;

	x = total_words_c(string, ' ');
	i = 0;
	// spit adv has some double quotes handing just for example
	tokens->args = ft_split_adv(string, ' ');
	tmp = NULL;
	pipe_collector(tokens, tokens->args); //maybe dont need
	tokens->array_count = x;
	if (tokens->args == NULL)
	{
		ft_printf("malloc fail in parsing , making of array of args\n");
		return ;
	}	
	i = 0;
	mini_parser(tokens, i, x); //changing CD to cd for example
	// some error handling maybe?
}

int	find_passage(t_data *all, char *string, int divert) // check if command is not a built in command
{
	int strlen;
	int path_found;

	path_found = 0;
	ft_printf("cmd = %s\n", all->tokens->args[0]);
	if (all->env->key == NULL || all->env == NULL)
	{
		printf("ENVS NULL OHO \n");
		return (0);
	}
	ft_printf("string = %s\n", string);
	if (string  == NULL)
	{
		printf("STRING OR CMD  NULL OHO \n");
		return (-1);
	}
	strlen = ft_strlen(string);
	if (find_node(all->env, string, all) == 1 && all->tmp->env_line != NULL)
		path_found = check_path(all->tmp->env_line, divert, all);
	if (path_found == 0)
	{
		printf("command not found\n");
		return (-1);
	}
	return (1);
}

static void	set_array(t_data *data)// char *flag, char *arguments)
{
	data->tmp->ex_arr[0] = data->tmp->filename;
	data->tmp->ex_arr[1] = NULL; // flag;
	data->tmp->ex_arr[2] = NULL; // arguments;
	data->tmp->ex_arr[3] = NULL; // last one is null
}

static void	split_diversion(t_data *data, int divert, char *string)
{
	if (divert == 1) // PATH
		data->tmp->array = ft_split(string, ':');
	else if (divert == 2) // HOME
		data->tmp->array = ft_split(string, ' ');
	if (data->tmp->array == NULL)
	{
		printf("temp[i] is null for some reason\n"); // remove?
		// error
	}
}

int null_check(DIR *test, struct dirent *dp, int indicator, char *suffix)
{
	if (test == NULL && indicator == 1)
	{
		printf("empty ur pants\n");
		free_string(suffix);
		return (-1);
	}
	else if (dp == NULL && indicator == 2)
	{
		printf("lift your pants \n");
		free_string(suffix);
		closedir(test);
		return (-1);
	}
	return (1);
}

int	check_dir(char *str)
{
	struct dirent	*dp;
	DIR				*test; 
	
	test = NULL;
	if (access(str, X_OK) == 0)
	{
		test = opendir(str);  // can check with null no leaks
		if (test == NULL)
			return (0); // maybe -1
		dp = readdir(test); // can check with null no leaks
		if (dp == NULL)
		{
			closedir(test);
			return (0); // maybe -1
		}
	}
	closedir(test);
	return (1);
}

int	check_path(char *string, int divert, t_data *all)
{
	struct dirent	*dp;
	DIR				*dir; //test 
	char			*suffix;
	size_t			cmd_len;
	int				i;

	i = 0;
	cmd_len = ft_strlen(all->tokens->args[0]);
	suffix = ft_strjoin("/", all->tokens->args[0]);
	if (suffix == NULL || cmd_len == 0)
	{
		free_string(suffix);
		return (0);
	}
	split_diversion(all, divert, string);
	while (all->tmp->array[i] != NULL)
	{
		if (check_dir(all->tmp->array[i]) == 0)
		{
			free_string(suffix);
			return (-1); // perror?
		}
		if (check_dir(all->tmp->array[i]) == 1)
		{
			dir = opendir(all->tmp->array[i]);  // can check with null no leaks
			dp = readdir(dir); // can check with null no leaks
			if (divert == 2)
			{
				all->tmp->filename = all->tmp->array[i];
				free_string(suffix);
				closedir(dir);
				return (1);
			}
			while (dp != NULL)
			{

				if (ft_strncmp(dp->d_name, all->tokens->args[0], cmd_len) == 0 && ft_strlen(dp->d_name) == cmd_len)
				{			
					all->tmp->filename = ft_strjoin(all->tmp->array[i], suffix);
					set_array(all); //, NULL, NULL, NULL);
//					printf("filename = %s\n", all->tmp->filename); // remove
					if (simple_fork(all) == 0)
					{
						free_string(all->tmp->filename);
						free_string(suffix);
//						free(test);
						closedir(dir); // THE close below here!
						return (1);;
					}
					return (1);
				}
				dp = readdir(dir); // without close below, leaks a lot
			}
			closedir(dir); // THE close below here!
		}
		i++;
	}

	ft_printf("\tAAA command not found\n");
	free_string(all->tmp->env_line);
	free_array(all->tmp->array);
	free_string(suffix); // this fixes "/cmd" leak
	return (0);
}
