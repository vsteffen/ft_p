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

char	*get_path(char *av_0, char *file)
{
	char	*tmp;
	char	*ret;

	if (!av_0)
		exit_message("Can't find program location", 1);
	tmp = ft_strrchr(av_0, '/');
	if (tmp)
		*(tmp + 1) = '\0';
	else if (tmp)
		av_0 = "./";
	ret = malloc(ft_strlen(av_0) + ft_strlen(file) + 1);
	ret = ft_strcpy(ret, av_0);
	ret = ft_strcat(ret, file);
	return (ret);
}

void	init_document_directory(t_srv *srv, char *av_0)
{
	DIR *		dir;

	srv->docs = get_path(av_0, SRV_DOCS);  // Must free it
	mkdir(srv->docs, 0755);
	if ((dir = opendir(srv->docs)) == NULL)
	{
		free(srv->docs);
		exit_message("Failed to create docs folder", 0);
	}
	if (closedir(dir) == -1)
	{
		free(srv->docs);
		free(dir);
		exit_message("Can't close "SRV_DOCS" directory", 1);
	}
}

int		create_server(uint16_t port, int client_nbr)
{
	int					sock;
	struct protoent		*proto;
	struct sockaddr_in	sin;

	if ((proto = getprotobyname("tcp")) == 0)
		exit_message("Failed to get protocol requested", 1);
	sock = socket(PF_INET, SOCK_STREAM, proto->p_proto);
	sin.sin_family = AF_INET;
	sin.sin_port = htons(port);
	sin.sin_addr.s_addr = htonl(INADDR_ANY);
	if (bind(sock, (const struct sockaddr *)&sin, sizeof(sin)) == -1)
		exit_message("Failed to bind socket", 2);
	if (listen(sock, client_nbr) == -1)
		exit_message("Failed to listen socket", 3);
	return (sock);
}

int		main(int ac, char **av)
{
	t_srv				srv;
	struct sockaddr_in	tmp_sin;
	socklen_t			tmp_slen;

	if (ac != 2)
		usage(av[0]);
	init_document_directory(&srv, av[0]);
	init_users(av[0], &srv);
	srv.sock = create_server((uint16_t)ft_atoi(av[1]), SOCK_CONNECTION_QUEUE);
	srv.auth = 0;
	srv.sock_pasv = -1;
	srv.user_path = ft_strdup(SRV_DOCS);
	tmp_slen = sizeof(tmp_sin);
	getsockname(srv.sock, (struct sockaddr *)&tmp_sin, &tmp_slen);
	srv.port = ntohs(tmp_sin.sin_port);
	ft_printf(BIN_SRV": Port use: [%u]\n"BIN_SRV": Waiting for connection ...\n", srv.port);
	srv.pid = 0;
	get_srv(&srv, 1);
	signal(SIGINT, signal_handler_srv);
	handle_all_connection_srv(&srv);
	close(srv.cs);
	return (0);
}
