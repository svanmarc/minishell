/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   TOK_tools.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: svanmarc <@student.42perpignan.fr>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/07 06:13:24 by svanmarc          #+#    #+#             */
/*   Updated: 2024/01/07 13:47:57 by svanmarc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void	add_token_back(t_token **tokens, t_token *new_token)
{
	t_token	*tmp;

	if (!tokens || !new_token)
		return ;
	if (!*tokens)
		*tokens = new_token;
	else
	{
		tmp = *tokens;
		while (tmp->next)
			tmp = tmp->next;
		tmp->next = new_token;
		new_token->previous = tmp;
	}
}

t_token	*create_new_token(char *val, int type)
{
	t_token	*new_token;

	new_token = (t_token *)ft_calloc(1, sizeof(t_token));
	if (!new_token)
		return (NULL);
	new_token->type = type;
	free(new_token->val);
	new_token->val = ft_strdup(val);
	if (!new_token->val)
	{
		free(new_token);
		return (NULL);
	}
	new_token->previous = NULL;
	new_token->next = NULL;
	return (new_token);
}

int	make_list_tokens(t_token **tokens, char *val, int type, int space)
{
	t_token	*new_token;

	new_token = create_new_token(val, type);
	if (!new_token)
	{
		free_tokens(tokens);
		return (0);
	}
	new_token->space_before = space;
	add_token_back(tokens, new_token);
	return (1);
}

t_token	*its_last_token(t_token **tokens)
{
	t_token	*tmp;

	if (!tokens || !*tokens)
		return (NULL);
	tmp = *tokens;
	while (tmp)
	{
		if (!tmp->next)
			return (tmp);
		tmp = tmp->next;
	}
	return (NULL);
}

int	ft_tokensize(t_token **lst)
{
	t_token	*tmp;
	int		count;

	tmp = *lst;
	count = 0;
	while (tmp)
	{
		tmp = tmp->next;
		if (tmp && tmp->type != TK_TYPE_RED_IN
			&& tmp->type != TK_TYPE_RED_OUT
			&& tmp->type != TK_TYPE_RED_OUT_APPEND)
			count++;
	}
	return (count);
}
