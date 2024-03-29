/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   CMD_exec.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: svanmarc <@student.42perpignan.fr>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/30 14:37:24 by mmarie            #+#    #+#             */
/*   Updated: 2024/01/09 16:10:37 by svanmarc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int	ft_is_builltins_cmd(char *cmd)
{
	if (ft_strncmp(cmd, "echo", 5) == 0)
		return (1);
	if (ft_strncmp(cmd, "cd", 3) == 0)
		return (1);
	if (ft_strncmp(cmd, "pwd", 4) == 0)
		return (1);
	if (ft_strncmp(cmd, "export", 7) == 0)
		return (1);
	if (ft_strncmp(cmd, "unset", 6) == 0)
		return (1);
	if (ft_strncmp(cmd, "env", 4) == 0)
		return (1);
	if (ft_strncmp(cmd, "exit", 5) == 0)
		return (1);
	return (0);
}

void	ft_exec_builtins(t_data *data, char **argv)
{
	int	ret;

	ret = 0;
	if (ft_strncmp(argv[0], "echo", 5) == 0)
		ret = ft_echo(ft_tab_size(argv), argv, data->env);
	else if (ft_strncmp(argv[0], "cd", 3) == 0)
		ret = builtin_cd(ft_tab_size(argv), argv, data);
	else if (ft_strncmp(argv[0], "pwd", 4) == 0)
		ret = builtin_pwd(ft_tab_size(argv), argv, data->env);
	else if (ft_strncmp(argv[0], "export", 7) == 0)
		ret = exec_export(ft_tab_size(argv), argv, data);
	else if (ft_strncmp(argv[0], "unset", 6) == 0)
		ret = exec_unset(ft_tab_size(argv), argv, data);
	else if (ft_strncmp(argv[0], "env", 4) == 0)
		ret = exec_env(data);
	else if (ft_strncmp(argv[0], "exit", 5) == 0)
		ret = ft_exit(ft_tab_size(argv), argv, data);
	data->last_exit_status = ret;
}

void	exec_external_command(char *cmd, char **argv, t_data *data)
{
	pid_t	pid;
	int		status;

	signal(SIGINT, &parent_sig_handler);
	signal(SIGQUIT, &parent_sig_handler);
	pid = fork();
	if (pid < 0)
	{
		fprintf(stderr, "fork failed");
		exit(EXIT_FAILURE);
	}
	else if (pid == 0)
	{
		signal(SIGINT, SIG_DFL);
		signal(SIGQUIT, SIG_DFL);
		execve(cmd, argv, data->env);
		exit(EXIT_FAILURE);
	}
	waitpid(pid, &status, 0);
	handle_signal();
	if (WIFEXITED(status))
		data->last_exit_status = WEXITSTATUS(status);
	else
		data->last_exit_status = 128 + WTERMSIG(status);
}

void	ft_exec_ext_command(char **argv, t_data *data)
{
	char	**path;
	char	*cmd;

	cmd = NULL;
	path = NULL;
	if (ft_strchr(argv[0], '/'))
	{
		if (access(argv[0], F_OK) == 0)
			cmd = ft_strdup(argv[0]);
	}
	if (cmd == NULL)
		path = ft_getenvpath(data->env);
	if (path)
		cmd = ft_checkexe(argv[0], path);
	if (cmd == NULL)
	{
		printf("%s : command not found\n", argv[0]);
		data->last_exit_status = 127;
	}
	else
		exec_external_command(cmd, argv, data);
	ft_free_tab(path);
	free(cmd);
}

int	ft_exec(t_data *data)
{
	char	**argv;

	argv = get_cmd_array(&data->tokens);
	if (!argv[0])
		return (1);
	if (apply_redirections(data, &data->tokens) == 1)
	{
		reset_redirections(data);
		return (1);
	}
	if (ft_is_builltins_cmd(argv[0]))
		ft_exec_builtins(data, argv);
	else
		ft_exec_ext_command(argv, data);
	free_tokens(&data->tokens);
	free(argv);
	return (0);
}
