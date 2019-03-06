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
	ft_printf("%s"BIN_CLT"> ", clt->cmd.buff);
}

void	init_connection(t_clt *clt)
{
	char		*input;
	size_t		len_input;
	char		*request;

	ft_printf("User: ");
	if (get_next_line(0, &input) != 1)
		exit_socket("Error while reading user input", 1, clt->sock);
	len_input = ft_strlen(input);
	input[len_input] = '\n';
	request = malloc(5 + len_input + 1);
	ft_memcpy(request, "USER ", 5);
	ft_memcpy(request + 5, input, len_input + 1);
	if (send(clt->sock, request, 5 + len_input + 1, 0) == -1)
		exit_socket("Error while sending user input", 1, clt->sock);
}

void	handle_connection_clt(t_clt *clt)
{
	char		*input;
	size_t		len_input;

	if (fork() == 0)
	{
		signal(SIGINT, signal_handler_clt);
		init_connection(clt);
		while (42)
		{
			if (get_next_line(0, &input) != 1)
				exit_socket("Error while reading user input", 1, clt->sock);
			len_input = ft_strlen(input);
			input[len_input] = '\n';
			if (send(clt->sock, input, len_input + 1, 0) == -1)
				exit_socket("Error while sending user input", 1, clt->sock);
			if (ft_strcmp("bye", input) == 0 || ft_strcmp("close", input) == 0)
				exit_socket("End of connection", 1, clt->sock);
		}
	}
	else
	{
		while (42)
		{
			recv_server(clt);
		}
	}
}
