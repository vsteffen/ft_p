#include "ft_p.h"

void	handle_connection(t_ftp_srv *ftp_srv)
{
	pid_t		child_pid;

	while (42)
	{
		if ((ftp_srv->cs = accept(ftp_srv->sock, (struct sockaddr*)&ftp_srv->csin, &ftp_srv->cs)) == (socklen_t)-1)
			exit_message("Fail to accept connection on socket", 4);
		ft_printf(BIN_SRV": Connection accepted from %s:%d\n", inet_ntoa(ftp_srv->csin.sin_addr), ntohs(ftp_srv->csin.sin_port));

		if ((child_pid = fork()) == 0)
		{
			close(ftp_srv->sock);
			while (42)
			{
				ft_bzero(ftp_srv->cmd.buff, REQUEST_BUFF);
				recv(ftp_srv->cs, ftp_srv->cmd.buff, REQUEST_BUFF, 0);
				ft_printf("Receive: [%s]\n", ftp_srv->cmd.buff);
				if (ft_strcmp("bye", ftp_srv->cmd.buff) == 0 || ft_strcmp("close", ftp_srv->cmd.buff) == 0)
				{
					ft_printf(BIN_SRV": Disconnected from %s:%d\n", inet_ntoa(ftp_srv->csin.sin_addr), ntohs(ftp_srv->csin.sin_port));
					break ;
				}
				else
				{
					send(ftp_srv->cs, ftp_srv->cmd.buff, ft_strlen(ftp_srv->cmd.buff), 0);
				}
			}
			exit_message("DEBUG", 42);
		}
	}
}
