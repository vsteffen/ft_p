/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   serveur.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vsteffen <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/02/18 22:11:47 by vsteffen          #+#    #+#             */
/*   Updated: 2019/02/18 22:11:49 by vsteffen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <srv.h>

void	exit_socket(char *message, int ret, int sock, t_srv *srv)
{
	close(sock);
	exit_message(message, ret, srv);
}

char	*get_response(t_srv *srv, char *response)
{
	ssize_t		rcv_length;
	int			len;
	static char	*next = NULL;

	next = next ? next : response;
	if (next == response || !*next)
	{
		if ((rcv_length = recv(srv->cs, response, RSP_BUFF, 0)) <= 0)
			rcv_length == 0 ? exit_socket("Connection close by ftp client", 1,
srv->cs, srv) : exit_socket("Error while receving data from ftp client",
1, srv->cs, srv);
		response[rcv_length] = '\0';
	}
	else
		ft_strcpy(response, next);
	next = ft_strchr(response, '\n') + 1;
	next = !next || !*next ? NULL : next;
	len = next ? next - response : ft_strlen(response);
	if (next)
		while (next[1] == '\n' || next[0] == '\n')
			next = ft_strchr(next, '\n') + 1;
	response[len] = 0;
	return (response);
}

void	handle_connection(t_srv *srv)
{
	char		request[RSP_BUFF + 1];

	srv->pid = 1;
	send(srv->cs, "220 FTP server ready.\n", 22, 0);
	while (42)
		handle_request(srv, get_response(srv, request));
}

void	handle_all_connection_srv(t_srv *srv)
{
	while (42)
	{
		if ((srv->cs = accept(srv->sock,
			(struct sockaddr*)&srv->csin, &srv->cs)) == (socklen_t)(-1))
			exit_message("Failed to accept connection on socket", 4, srv);
		if (fork() == 0)
			handle_connection(srv);
		else
			close(srv->cs);
	}
}
