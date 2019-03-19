#include <srv.h>

struct s_cmd	g_cmd[] = {
	{"LIST\n", 5, handle_list, 1},
	{"LIST ", 5, handle_list, 1},
	{"USER ", 5, handle_user, 0},
	{"PASS ", 5, handle_pass, 0},
	{"QUIT\n", 5, handle_quit, 0},
	{"CWD ", 4, handle_cwd, 1},
	{"PWD\n", 4, handle_pwd, 1},
	{"RETR ", 5, handle_retr, 1},
	{"STOR ", 5, handle_stor, 1},
	{"PASV\n", 5, handle_pasv, 1},
	{"TYPE I\n", 7, handle_type, 1},
	{NULL, 0, NULL, 0},
};

void	handle_request(t_srv *srv, char *request)
{
	int			i;
	char		*tmp;
	char		*response_tmp;

	i = 0;
	ft_strtoupper((tmp = ft_strdup(request)));
	while (g_cmd[i].key)
	{
		if (!ft_strncmp(g_cmd[i].key, tmp, g_cmd[i].len_key))
		{
			if (!g_cmd[i].auth || (g_cmd[i].auth && srv->auth))
				g_cmd[i].f(srv, request + g_cmd[i].len_key);
			else
				send(srv->cs, "530 Please login with USER and PASS.\n", 37, 0);
			break ;
		}
		i++;
	}
	if (!g_cmd[i].key)
	{
		if (*request != '\n')
		{
			response_tmp = malloc(ft_strlen(request) + 30);
			ft_strcpy(response_tmp, "500 ");
			request[ft_strlen(request) - 1] = 0;
			ft_strcat(response_tmp, request);
			ft_strcat(response_tmp, ": command not understood.\n");
			send(srv->cs, response_tmp, ft_strlen(request) + 29, 0);
			free(response_tmp);
		}
	}
	free(tmp);
}

void	handle_list(struct s_srv *srv, char *input)
{
	int					sock;
	socklen_t			cs;
	struct sockaddr_in	csin;

	ft_printf("LIST\n");
	(void)srv;
	(void)input;
	if (srv->sock_pasv == -1)
	{
		send(srv->cs, "425 Can't build data connection\n", 32, 0);
		return ;
	}
	send(srv->cs, "150 Opening BINARY mode data connection for 'file list'.\n", 57, 0);
	if ((sock = accept(srv->sock_pasv, (struct sockaddr*)&csin, &cs)) == (socklen_t)-1)
			send(srv->cs, "425 Can't build data connection\n", 32, 0);
	send(sock, "POULET\n", 7, 0);
	send(srv->cs, "226 Transfer complete.\n", 23, 0);
	if (sock != -1)
		close(sock);
	close(srv->sock_pasv);
	srv->sock_pasv = -1;
}

void	handle_user(t_srv *srv, char *input)
{
	char	*tmp;

	send(srv->cs, "331 Password required for admin.\n", 33, 0);
	if (!(tmp = ft_strrchr(input, '\n')))
	{
		send(srv->cs, "500 : command not understood.\n", 30, 0);
		return ;
	}
	tmp[0] = '\0';
	srv->id_user = search_user(srv, input);
	ft_printf("Id found = [%zu]\n", srv->id_user);
}

void	handle_pass(t_srv *srv, char *input)
{
	char	*response_tmp;
	char	*tmp;

	if (!(tmp = ft_strrchr(input, '\n')))
	{
		send(srv->cs, "530 Login incorrect.\n", 21, 0);
		return ;
	}
	tmp[0] = '\0';
	if (srv->id_user == -1u || srv->id_user > SOCK_CONNECTION_QUEUE)
		send(srv->cs, "500 : command not understood.\n", 30, 0);
	else if (check_passwd(srv, input))
	{
		response_tmp = malloc(ft_strlen(srv->user[srv->id_user][0]) + 22);
		ft_strcpy(response_tmp, "230 User ");
		ft_strcat(response_tmp, srv->user[srv->id_user][0]);
		ft_strcat(response_tmp, " logged in.\n");
		send(srv->cs, response_tmp, ft_strlen(srv->user[srv->id_user][0]) + 21, 0);
		free(response_tmp);
		srv->auth = 1;
		return ;
	}
	else
		send(srv->cs, "530 Login incorrect.\n", 21, 0);
	srv->auth = 0;
}

void	handle_cwd(struct s_srv *srv, char *input)
{
	ft_printf("CWD\n");
	(void)srv;
	(void)input;
}

void	handle_pwd(struct s_srv *srv, char *input)
{
	ft_printf("PWD\n");
	(void)srv;
	(void)input;
}

void	handle_quit(t_srv *srv, char *input)
{
	(void)input;
	ft_printf(BIN_SRV": Disconnected from ftp client [%s:%d]\n", inet_ntoa(srv->csin.sin_addr), ntohs(srv->csin.sin_port));
	close(srv->cs);
	exit(0);
}

void	handle_retr(struct s_srv *srv, char *input)
{
	ft_printf("RETR\n");
	(void)srv;
	(void)input;
}

void	handle_stor(struct s_srv *srv, char *input)
{
	ft_printf("STOR\n");
	(void)srv;
	(void)input;
}

void	convert_port_to_string(uint16_t port, char *buff)
{
	char	*str_nb;

	ft_strcat(buff, str_nb = ft_itoa(port >> 8));
	free(str_nb);
	ft_strcat(buff, ",");
	ft_strcat(buff, str_nb = ft_itoa(port & 0xFF));
	free(str_nb);
}

void	handle_pasv(struct s_srv *srv, char *input)
{
	char	ret[47];
	struct sockaddr_in	tmp_sin;
	socklen_t			tmp_slen;

	(void)input;
	if (srv->sock_pasv != -1)
		close(srv->sock_pasv);
	srv->sock_pasv = create_server(0, 1);
	getsockname(srv->sock_pasv, (struct sockaddr *)&tmp_sin, &tmp_slen);
	ft_strcpy(ret, "227 Entering Passive Mode (127,0,0,1,");
	ft_printf("PASV: port [%zu] / sock [%d]\n", ntohs(tmp_sin.sin_port), srv->sock_pasv);
	convert_port_to_string(ntohs(tmp_sin.sin_port), ret + 37);
	ft_strcat(ret, ")\n");
	send(srv->cs, ret, ft_strlen(ret), 0);
}

void	handle_type(struct s_srv *srv, char *input)
{
	ft_printf("TYPE\n");
	(void)input;
	send(srv->cs, "200 Type set to I.\n", 19, 0);
}
