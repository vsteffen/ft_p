#include "ft_p.h"

void	exit_message_and_fd(char *message, int ret, int fd)
{
	close(fd);
	exit_message(message, ret);
}

void	handle_connection_clt(t_clt *clt)
{
	char		*input;
	ssize_t		rcv_length;

	while (42)
	{
		ft_printf(BIN_CLT": ");
		if (get_next_line(0, &input) != 1)
			exit_message_and_fd("Error while reading user input", 1, clt->sock);
		if (send(clt->sock, input, ft_strlen(input), 0) == -1)
			exit_message_and_fd("Error while sending user input", 1, clt->sock);
		if (ft_strcmp("bye", input) == 0 || ft_strcmp("close", input) == 0)
			exit_message_and_fd("End of connection", 1, clt->sock);
		if ((rcv_length = recv(clt->sock, clt->cmd.buff, REQUEST_BUFF, 0)) <= 0)
		{
			if (rcv_length == 0)
				exit_message_and_fd("Connection close by ftp sever", 1, clt->sock);
			else
				exit_message_and_fd("Error while receving data from ftp sever", 1, clt->sock);
		}
		clt->cmd.buff[rcv_length] = '\0';
		ft_printf("Receive [%s]\n", clt->cmd.buff);
	}
}
