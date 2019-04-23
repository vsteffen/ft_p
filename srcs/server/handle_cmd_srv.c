/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_cmd_srv.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: magouin <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/04/23 22:31:57 by magouin           #+#    #+#             */
/*   Updated: 2019/04/23 22:31:58 by magouin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <srv.h>

struct s_cmd	g_cmd[] = {
	{"LIST\n", 5, handle_list, 1},
	{"LIST ", 5, handle_list, 1},
	{"USER ", 5, handle_user, 0},
	{"PASS ", 5, handle_pass, 0},
	{"QUIT\n", 5, handle_quit, 0},
	{"CWD ", 4, handle_cwd, 1},
	{"PWD\n", 4, handle_pwd, 1},
	{"RETR ", 5, handle_retr, 1},
	{"STOR ", 5, handle_stor, 1},
	{"PASV\n", 5, handle_pasv, 1},
	{"TYPE I\n", 7, handle_type, 1},
	{"SYST\n", 5, handle_syst, 1},
	{NULL, 0, NULL, 0},
};

void	command_not_found(t_srv *srv, char *request, int i)
{
	char		*response_tmp;

	if (!g_cmd[i].key && *request != '\n')
	{
		response_tmp = malloc(ft_strlen(request) + 30);
		ft_strcpy(response_tmp, "500 ");
		request[ft_strlen(request) - 1] = 0;
		ft_strcat(response_tmp, request);
		ft_strcat(response_tmp, ": command not understood.\n");
		send(srv->cs, response_tmp, ft_strlen(request) + 29, 0);
		free(response_tmp);
	}
}

void	handle_request(t_srv *srv, char *request)
{
	int			i;
	char		*tmp;

	i = 0;
	if (ft_strlen(request) > 2 && request[ft_strlen(request) - 2] == 0xd)
	{
		request[ft_strlen(request) - 2] = '\n';
		request[ft_strlen(request) - 1] = 0;
	}
	ft_strtoupper((tmp = ft_strdup(request)));
	while (g_cmd[i].key)
	{
		if (!ft_strncmp(g_cmd[i].key, tmp, g_cmd[i].len_key))
		{
			if (!g_cmd[i].auth || (g_cmd[i].auth && srv->auth))
				g_cmd[i].f(srv, request + g_cmd[i].len_key);
			else
				send(srv->cs, "530 Please login with USER and PASS.\n", 37, 0);
			break ;
		}
		i++;
	}
	command_not_found(srv, request, i);
	free(tmp);
}
