/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   DATA_init.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: svanmarc <@student.42perpignan.fr>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/05 16:22:23 by svanmarc          #+#    #+#             */
/*   Updated: 2024/01/09 19:55:43 by svanmarc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

t_data	*init_data(char **env)
{
	t_data	*data;

	data = (t_data *)ft_calloc(1, sizeof(t_data));
	if (!data)
		exit(1);
	data->env = ft_tab_dup(env);
	data->line = NULL;
	data->exit = 0;
	data->last_exit_status = 0;
	data->current_stdin = STDIN_FILENO;
	data->original_stdin = -1;
	data->current_stdout = STDOUT_FILENO;
	data->default_stdin = dup(STDIN_FILENO);
	data->default_stdout = dup(STDOUT_FILENO);
	data->original_stdout = -1;
	data->tokens = NULL;
	data->nb_pipes = 0;
	return (data);
}

void	free_data(t_data *data)
{
	if (!data)
		return ;
	if (data->env)
		ft_free_tab(data->env);
	if (data->line)
	{
		free(data->line);
		data->line = NULL;
	}
	if (data->tokens)
	{
		free_tokens(&data->tokens);
		data->tokens = NULL;
	}
	free(data);
	data = NULL;
}
