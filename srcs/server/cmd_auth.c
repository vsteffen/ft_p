/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd_auth.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: magouin <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/04/23 22:38:13 by magouin           #+#    #+#             */
/*   Updated: 2019/04/23 22:38:14 by magouin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <srv.h>

void	handle_user(t_srv *srv, char *input)
{
	char *tmp;

	send(srv->cs, "331 Password required for admin.\n", 33, 0);
	if (!(tmp = ft_strrchr(input, '\n')))
	{
		send(srv->cs, "500 : command not understood.\n", 30, 0);
		return ;
	}
	*tmp = 0;
	srv->id_user = search_user(srv, input);
}

void	handle_pass(t_srv *srv, char *input)
{
	char	*rsp;
	char	*tmp;

	if (!(tmp = ft_strrchr(input, '\n')))
	{
		send(srv->cs, "530 Login incorrect.\n", 21, 0);
		return ;
	}
	*tmp = '\0';
	if (srv->id_user == -1u || srv->id_user > SOCK_CONNECTION_QUEUE)
		send(srv->cs, "500 : command not understood.\n", 30, 0);
	else if (check_passwd(srv, input))
	{
		rsp = malloc(ft_strlen(srv->user[srv->id_user][0]) + 22);
		ft_strcpy(rsp, "230 User ");
		ft_strcat(rsp, srv->user[srv->id_user][0]);
		ft_strcat(rsp, " logged in.\n");
		send(srv->cs, rsp, ft_strlen(srv->user[srv->id_user][0]) + 21, 0);
		free(rsp);
		srv->auth = 1;
		return ;
	}
	else
		send(srv->cs, "530 Login incorrect.\n", 21, 0);
	srv->auth = 0;
}
