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
	struct hostent		*he;

	if ((proto = getprotobyname("tcp")) == 0)
		exit_message("Failed to get protocol requested", 1);
	sock = socket(PF_INET, SOCK_STREAM, proto->p_proto);
	sin.sin_family = AF_INET;
	sin.sin_port = htons(port);
	if ((he = gethostbyname(addr)) == NULL)
		exit_message("Failed to resolve hostname", 2);
	ft_memcpy(&sin.sin_addr, he->h_addr_list[0], he->h_length);
	if (connect(sock, (const struct sockaddr *)&sin, sizeof(sin)) == -1)
		exit_message("Failed to connect on socket", 2);
	return (sock);
}

int		main(int ac, char **av)
{
	t_clt			clt;

	if (ac != 3)
		usage(av[0]);
	clt.port = (uint16_t)ft_atoi(av[2]);
	clt.addr = av[1];
	clt.sock = create_client(clt.addr, clt.port);
	ft_printf(BIN_CLT": Connected on ftp server [%s:%s]\n", av[1], av[2]);
	get_clt(&clt, 1);
	handle_connection_clt(&clt);
	close(clt.sock);
	return (0);
}
