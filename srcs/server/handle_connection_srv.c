#include "ft_p.h"

void	exit_socket(char *message, int ret, int sock)
{
	close(sock);
	exit_message(message, ret);
}

void	handle_connection_srv(t_srv *srv)
{
	ssize_t		rcv_length;

	while (42)
	{
		if ((srv->cs = accept(srv->sock, (struct sockaddr*)&srv->csin, &srv->cs)) == (socklen_t)-1)
			exit_message("Fail to accept connection on socket", 4);
		ft_printf(BIN_SRV": Connection accepted from %s:%d\n", inet_ntoa(srv->csin.sin_addr), ntohs(srv->csin.sin_port));
		if (fork() == 0)
		{
			srv->pid = 1;
			while (42)
			{
				if ((rcv_length = recv(srv->cs, srv->cmd.buff, REQUEST_BUFF, 0)) <= 0)
				{
					if (rcv_length == 0)
						exit_socket("Connection close by client", 1, srv->sock);
					else
						exit_socket("Error while receving data from client", 1, srv->sock);
				}
				srv->cmd.buff[rcv_length] = '\0';
				ft_printf("Receive: [%s]\n", srv->cmd.buff);
				if (ft_strcmp("bye", srv->cmd.buff) == 0 || ft_strcmp("close", srv->cmd.buff) == 0)
				{
					ft_printf(BIN_SRV": Disconnected from ftp client [%s:%d]\n", inet_ntoa(srv->csin.sin_addr), ntohs(srv->csin.sin_port));
					close(srv->cs);
					exit(0);
				}
				else
				{
					if (send(srv->cs, srv->cmd.buff, ft_strlen(srv->cmd.buff), 0) == -1)
						exit_socket("Error while sending user input", 1, srv->cs);
				}
			}
		}
		else
			close(srv->cs);
	}
}
