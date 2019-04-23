/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vsteffen <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/02/18 22:11:47 by vsteffen          #+#    #+#             */
/*   Updated: 2019/02/18 22:11:49 by vsteffen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <clt.h>

const struct s_cmd	g_cmd[] = {
	{"LS", 3, handle_list},
	{"LS ", 3, handle_list},
	{"LOGIN", 6, handle_auth},
	{"LOGIN ", 6, handle_auth},
	{"QUIT", 5, handle_quit},
	{"CD ", 3, handle_cd},
	{"PWD", 4, handle_pwd},
	{"GET ", 4, handle_get},
	{"PUT ", 4, handle_put},
	{NULL, 0, NULL},
};

void	parse_cmd(char *input, t_clt *clt)
{
	size_t		len_input;
	int			i;
	char		*tmp;

	i = 0;
	len_input = ft_strlen(input);
	ft_strtoupper((tmp = ft_strdup(input)));
	while (g_cmd[i].key)
	{
		if (!ft_strncmp(g_cmd[i].key, tmp, g_cmd[i].len_key))
		{
			g_cmd[i].f(clt, input + g_cmd[i].len_key - 1);
			break ;
		}
		i++;
	}
	if (!g_cmd[i].key)
	{
		if (*input != '\0')
			ft_printf("%s: Unkown command.\n", input);
	}
	free(tmp);
	free(input);
	ft_printf(BIN_CLT"> ");
}

void	handle_connection_clt(t_clt *clt)
{
	char		*input;
	int			ret_gnl;

	if (get_response(clt, clt->rsp_buff) != 220)
		exit_socket("Server failed to connect", 1, clt->sock);
	handle_auth(clt, NULL);
	ft_printf(BIN_CLT"> ");
	while (42)
	{
		if ((ret_gnl = get_next_line(0, &input)) == -1)
			exit_socket("Error while reading user input", 1, clt->sock);
		if (ret_gnl != 0)
			parse_cmd(input, clt);
		else
			handle_quit(clt, NULL);
	}
}
