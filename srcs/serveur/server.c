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
	t_ftp_srv			ftp_srv;
	struct sockaddr_in	tmp_sin;
	socklen_t			tmp_slen;
	char				*buff;

	if (ac != 2)
		usage(av[0]);
	ftp_srv.sock = create_server((uint16_t)ft_atoi(av[1]));
	tmp_slen = sizeof(tmp_sin);
	getsockname(ftp_srv.sock, (struct sockaddr *)&tmp_sin, &tmp_slen);
	ftp_srv.port = ntohs(tmp_sin.sin_port);
	ft_printf("ft_p-server: Port use: [%u]\nft_p-server: Waiting for connection ...\n", ftp_srv.port);
	if ((ftp_srv.cs = accept(ftp_srv.sock, (struct sockaddr*)&ftp_srv.csin, &ftp_srv.cs)) == (socklen_t)-1)
		exit_message("Fail to accept connection on socket", 4);
	while (get_next_line(ftp_srv.cs, &buff) > 0)
		ft_printf("Receive: [%s]\n", buff);
	close(ftp_srv.cs);
	close(ftp_srv.sock);
	return (0);
}
