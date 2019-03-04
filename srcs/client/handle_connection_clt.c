#include "ft_p.h"

void	exit_socket(char *message, int ret, int socket)
{
	close(socket);
	exit_message(message, ret);
}

void	recv_server(t_clt *clt)
{
	ssize_t		rcv_length;

	if ((rcv_length = recv(clt->sock, clt->cmd.buff, REQUEST_BUFF, 0)) <= 0)
	{
		if (rcv_length == 0)
			exit_socket("Connection close by ftp sever", 1, clt->sock);
		else
			exit_socket("Error while receving data from ftp sever", 1, clt->sock);
	}
	clt->cmd.buff[rcv_length] = '\0';
	ft_printf("%s", clt->cmd.buff);
}

// void	recv_server(t_clt *clt)
// {
// 	ssize_t		rcv_length;
// 	int8_t			init;

// 	rcv_length = recv(clt->sock, clt->cmd.buff, REQUEST_BUFF, 0);
// 	init = 1;
// 	while (init || rcv_length == REQUEST_BUFF)
// 	{
// 		if (rcv_length <= 0)
// 		{
// 			if (rcv_length == 0)
// 				exit_socket("Connection close by ftp sever", 1, clt->sock);
// 			else
// 				exit_socket("Error while receving data from ftp sever", 1, clt->sock	);
// 		}
// 		clt->cmd.buff[rcv_length] = '\0';
// 		ft_printf("%s", clt->cmd.buff);
// 		init = 0;
// 		rcv_length == REQUEST_BUFF ? rcv_length = recv(clt->sock, clt->cmd.buff, REQUEST_BUFF, 0) : 0;
// 	}
// }

void	handle_connection_clt(t_clt *clt)
{
	char		*input;
	size_t		len_input;

	recv_server(clt);
	while (42)
	{
		ft_printf(BIN_CLT": ");
		if (get_next_line(0, &input) != 1)
			exit_socket("Error while reading user input", 1, clt->sock);
		len_input = ft_strlen(input);
		input[len_input] = '\n';
		if (send(clt->sock, input, len_input + 1, 0) == -1)
			exit_socket("Error while sending user input", 1, clt->sock);
		if (ft_strcmp("bye", input) == 0 || ft_strcmp("close", input) == 0)
			exit_socket("End of connection", 1, clt->sock);
		recv_server(clt);
	}
}
