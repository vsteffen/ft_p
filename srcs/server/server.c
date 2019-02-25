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

#include "ft_p.h"

void	usage(char *prog)
{
	ft_printf("Usage: %s <port>\n", prog);
	exit(-1);
}

void	exit_message(char *message, uint8_t ret)
{
	ft_printf(BIN_SRV": %s\n", message);
	exit(ret);
}

int		create_server(uint16_t port)
{
	int					sock;
	struct protoent		*proto;
	struct sockaddr_in	sin;

	if ((proto = getprotobyname("tcp")) == 0)
		exit_message("Fail to get protocol requested", 1);
	sock = socket(PF_INET, SOCK_STREAM, proto->p_proto);
	sin.sin_family = AF_INET;
	sin.sin_port = htons(port);
	sin.sin_addr.s_addr = htonl(INADDR_ANY);
	if (bind(sock, (const struct sockaddr *)&sin, sizeof(sin)) == -1)
		exit_message("Fail to bind socket", 2);
	if (listen(sock, SOCK_CONNECTION_QUEUE) == -1)
		exit_message("Fail to listen socket", 3);
	return (sock);
}

int		main(int ac, char **av)
{
	t_srv				srv;
	struct sockaddr_in	tmp_sin;
	socklen_t			tmp_slen;

	if (ac != 2)
		usage(av[0]);
	srv.sock = create_server((uint16_t)ft_atoi(av[1]));
	tmp_slen = sizeof(tmp_sin);
	getsockname(srv.sock, (struct sockaddr *)&tmp_sin, &tmp_slen);
	srv.port = ntohs(tmp_sin.sin_port);
	ft_printf(BIN_SRV": Port use: [%u]\n"BIN_SRV": Waiting for connection ...\n", srv.port);
	srv.pid = 0;
	get_srv(&srv, 1);
	signal(SIGINT, signal_handler_srv);
	handle_connection_srv(&srv);
	close(srv.cs);
	return (0);
}
