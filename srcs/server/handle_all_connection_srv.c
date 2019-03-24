#include <srv.h>

void	exit_socket(char *message, int ret, int sock)
{
	close(sock);
	exit_message(message, ret);
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
		{
			if (rcv_length == 0)
				exit_socket("Connection close by ftp client", 1, srv->cs);
			else
				exit_socket("Error while receving data from ftp client", 1,
					srv->cs);
		}
		response[rcv_length] = '\0';
	}
	else
		ft_strcpy(response , next);
	// ft_printf("Response = [%s]\n", response);
	next = ft_strchr(response, '\n') + 1;
	if (next && *next)
	{
		len = next - response;
		while (next[1] == '\n' || next[0] == '\n')
			next = ft_strchr(next, '\n') + 1;
	}
	else
	{
		len = ft_strlen(response);
		next = NULL;
	}
	write(1, response, len);
	return (response);
}

void	handle_connection(t_srv *srv)
{
	char		request[RSP_BUFF + 1];

	srv->pid = 1;
	send(srv->cs, "220 FTP server ready.\n", 22, 0);
	while (42)
	{
		handle_request(srv, get_response(srv, request));
		// if (send(srv->cs, srv->rsp_buff, ft_strlen(srv->rsp_buff), 0) == -1)
		// 	exit_socket("Error while sending user input", 1, srv->cs);
	}
}

void	handle_all_connection_srv(t_srv *srv)
{
	while (42)
	{
		if ((srv->cs = accept(srv->sock, (struct sockaddr*)&srv->csin, &srv->cs)) == (socklen_t)-1)
			exit_message("Failed to accept connection on socket", 4);
		ft_printf(BIN_SRV": Connection accepted from %s:%d\n", inet_ntoa(srv->csin.sin_addr), ntohs(srv->csin.sin_port));
		if (fork() == 0)
			handle_connection(srv);
		else
			close(srv->cs);
	}
}
