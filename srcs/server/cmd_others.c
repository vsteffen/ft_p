/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd_others.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: magouin <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/04/23 22:36:46 by magouin           #+#    #+#             */
/*   Updated: 2019/04/23 22:36:49 by magouin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <srv.h>

void	handle_type(struct s_srv *srv, char *input)
{
	(void)input;
	send(srv->cs, "200 Type set to I.\n", 19, 0);
}

void	handle_quit(t_srv *srv, char *input)
{
	(void)input;
	ft_printf(BIN_SRV": Disconnected from ftp client [%s:%d]\n",\
		inet_ntoa(srv->csin.sin_addr), ntohs(srv->csin.sin_port));
	close(srv->cs);
	exit(0);
}

void	handle_syst(struct s_srv *srv, char *input)
{
	(void)input;
	send(srv->cs, "215 UNIX Type: L8\n", 18, 0);
}

void	handle_list(struct s_srv *srv, char *input)
{
	int					sock;
	socklen_t			cs;
	struct sockaddr_in	csin;
	int					ret_child;

	input[ft_strlen(input) - 1] = 0;
	if (srv->sock_pasv == -1)
	{
		send(srv->cs, "425 Can't build data connection\n", 32, 0);
		return ;
	}
	send(srv->cs, "150 Opening BINARY mode data connection.\n", 41, 0);
	if ((sock = accept(srv->sock_pasv, (struct sockaddr*)&csin, &cs)) == -1)
		send(srv->cs, "425 Can't build data connection\n", 32, 0);
	ret_child = fork_for_write((struct s_fork_params) \
		{"ls -lLa ", "/bin/ls", 9, 2}, srv, sock, input);
	if (!ret_child)
		send(srv->cs, "226 Transfer complete.\n", 23, 0);
	else
		no_such_file(srv, input);
	if (sock != -1)
		close(sock);
}
