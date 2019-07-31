/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   serveur.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vsteffen <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/02/18 22:11:47 by vsteffen          #+#    #+#             */
/*   Updated: 2019/07/31 20:21:16 by vsteffen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <srv.h>

void	usage(const char *prog)
{
	ft_printf("Usage: %s <port>\n", prog);
	exit(-1);
}

void	exit_message(char *message, uint8_t ret, t_srv *srv)
{
	if (srv)
	{
		free(srv->docs);
		free(srv->user_path);
	}
	ft_printf(BIN_SRV": %s\n", message);
	exit(ret);
}

void	init_document_directory(t_srv *srv)
{
	DIR	*dir;

	mkdir(SRV_DOCS, 0755);
	if ((dir = opendir(SRV_DOCS)) == NULL)
	{
		free(srv->docs);
		exit_message("Failed to create docs folder", 0, srv);
	}
	if (closedir(dir) == -1)
	{
		free(srv->docs);
		exit_message("Can't close "SRV_DOCS" directory", 1, srv);
	}
}

int		create_server(uint16_t port, int client_nbr)
{
	int					sock;
	struct protoent		*proto;
	struct sockaddr_in	sin;

	if ((proto = getprotobyname("tcp")) == 0)
		exit_message("Failed to get protocol requested", 1, NULL);
	sock = socket(PF_INET, SOCK_STREAM, proto->p_proto);
	sin.sin_family = AF_INET;
	sin.sin_port = htons(port);
	sin.sin_addr.s_addr = htonl(INADDR_ANY);
	if (bind(sock, (const struct sockaddr *)&sin, sizeof(sin)) == -1)
		exit_message("Failed to bind socket", 2, NULL);
	if (listen(sock, client_nbr) == -1)
		exit_message("Failed to listen socket", 3, NULL);
	return (sock);
}

int		main(int ac, const char **av)
{
	t_srv				srv;
	struct sockaddr_in	tmp_sin;
	socklen_t			tmp_slen;

	if (ac != 2)
		usage(av[0]);
	close(2);
	init_document_directory(&srv);
	init_users(&srv);
	srv.sock = create_server((uint16_t)ft_atoi(av[1]), SOCK_CONNECTION_QUEUE);
	srv.auth = 0;
	srv.sock_pasv = -1;
	srv.user_path = ft_strdup(SRV_DOCS);
	tmp_slen = sizeof(tmp_sin);
	getsockname(srv.sock, (struct sockaddr *)&tmp_sin, &tmp_slen);
	srv.port = ntohs(tmp_sin.sin_port);
	ft_printf(BIN_SRV": Port use: [%u]\n"BIN_SRV": Wa\
iting for connection ...\n", srv.port);
	srv.pid = 0;
	get_srv(&srv, 1);
	signal(SIGINT, signal_handler_srv);
	handle_all_connection_srv(&srv);
	close(srv.cs);
	return (0);
}
