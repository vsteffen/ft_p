/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   put_and_get.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: magouin <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/04/23 22:35:24 by magouin           #+#    #+#             */
/*   Updated: 2019/04/23 22:35:25 by magouin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <srv.h>

void	handle_retr(struct s_srv *srv, char *input)
{
	int					sock;
	socklen_t			cs;
	int					ret_child;
	struct sockaddr_in	csin;

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
		{"cat ", "/bin/cat", 5, 1}, srv, sock, input);
	if (!ret_child)
		send(srv->cs, "226 Transfer complete.\n", 23, 0);
	else
		no_such_file(srv, input);
	if (sock != -1)
		close(sock);
}

int		check_stor(struct s_srv *srv, char *tmp, int *fd_to_write, int *sock)
{
	socklen_t			cs;
	struct sockaddr_in	csin;

	if ((*fd_to_write = open(tmp, O_RDWR | O_CREAT, 0644)) == -1)
	{
		send(srv->cs, "553 Requested action not taken.\n", 32, 0);
		return (0);
	}
	if (srv->sock_pasv == -1)
	{
		send(srv->cs, "425 Can't build data connection\n", 32, 0);
		return (0);
	}
	send(srv->cs, "150 Opening BINARY mode data connection.\n", 41, 0);
	if ((*sock = accept(srv->sock_pasv, (struct sockaddr*)&csin, &cs)) == -1)
	{
		send(srv->cs, "425 Can't build data connection\n", 32, 0);
		return (0);
	}
	return (1);
}

void	handle_stor(struct s_srv *srv, char *input)
{
	int					sock;
	char				*tmp;
	ssize_t				rcv_length;
	int					fd_to_write;
	char				buff[RSP_BUFF + 1];

	input[ft_strlen(input) - 1] = 0;
	if ((tmp = ft_strrchr(input, '/')))
		input = tmp + 1;
	tmp = ft_strjoin(srv->user_path, input);
	if (!check_stor(srv, tmp, &fd_to_write, &sock))
		return ;
	while ((rcv_length = recv(sock, buff, RSP_BUFF, 0)) > 0)
		write(fd_to_write, buff, rcv_length);
	send(srv->cs, "226 Transfer complete.\n", 23, 0);
	if (sock != -1)
		close(sock);
}
