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
	ft_printf("Usage: %s <addr> <port>\n", prog);
	exit(-1);
}

void	exit_message(char *message, uint8_t ret)
{
	ft_printf(BIN_CLT": %s\n", message);
	exit(ret);
}

int		create_client(char *addr, uint16_t port)
{
	int					sock;
	struct protoent		*proto;
	struct sockaddr_in	sin;

	if ((proto = getprotobyname("tcp")) == 0)
		exit_message("Fail to get protocol requested", 1);
	sock = socket(PF_INET, SOCK_STREAM, proto->p_proto);
	sin.sin_family = AF_INET;
	sin.sin_port = htons(port);
	sin.sin_addr.s_addr = inet_addr(addr);
	if (connect(sock, (const struct sockaddr *)&sin, sizeof(sin)) == -1)
		exit_message("Fail to connect on socket", 2);
	return (sock);
}

int		main(int ac, char **av)
{
	t_ftp_clt			ftp_clt;

	if (ac != 3)
		usage(av[0]);
	ftp_clt.port = (uint16_t)ft_atoi(av[2]);
	ftp_clt.addr = av[1];
	ftp_clt.sock = create_client(ftp_clt.addr, ftp_clt.port);
	// ft_printf("%{FD}Hello world!", ftp_clt.sock);
	write(ftp_clt.sock, "Hello world!", ft_strlen("Hello world!"));
	close(ftp_clt.sock);
	return (0);
}
