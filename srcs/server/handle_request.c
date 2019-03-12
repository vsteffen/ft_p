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
				g_cmd[i].f(srv, request + g_cmd[i].len_key - 1);
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
	ft_printf("LIST\n");
	(void)srv;
	(void)input;
}

void	handle_user(t_srv *srv, char *input)
{
	ft_printf("USER\n");
	(void)srv;
	(void)input;
	send(srv->cs, "331 Password required for admin.\n", 33, 0);
}

void	handle_pass(t_srv *srv, char *input)
{
	ft_printf("PASS\n");
	(void)srv;
	(void)input;
	send(srv->cs, "230 User admin logged in.\n", 26, 0);
	// srv->auth = 1;
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

void	handle_pasv(struct s_srv *srv, char *input)
{
	ft_printf("PASV\n");
	(void)srv;
	(void)input;
}

void	handle_type(struct s_srv *srv, char *input)
{
	ft_printf("TYPE\n");
	(void)input;
	send(srv->cs, "200 Type set to I.\n", 19, 0);
}
