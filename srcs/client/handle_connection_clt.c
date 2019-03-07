#include "ft_p.h"

void	exit_socket(char *message, int ret, int socket)
{
	close(socket);
	exit_message(message, ret);
}

int		get_response(t_clt *clt, char *response)
{
	ssize_t		rcv_length;

	if ((rcv_length = recv(clt->sock, response, RSP_BUFF, 0)) <= 0)
	{
		if (rcv_length == 0)
			exit_socket("Connection close by ftp sever", 1, clt->sock);
		else
			exit_socket("Error while receving data from ftp sever", 1, clt->sock);
	}
	response[rcv_length] = '\0';
	ft_putstr(response);
	if (response[0] == '1')
		return (get_response(clt, response));
	return (ft_atoi(response));
}

void	recv_server(t_clt *clt)
{
	ssize_t		rcv_length;

	if ((rcv_length = recv(clt->sock, clt->rsp_buff, RSP_BUFF, 0)) <= 0)
	{
		if (rcv_length == 0)
			exit_socket("Connection close by ftp sever", 1, clt->sock);
		else
			exit_socket("Error while receving data from ftp sever", 1, clt->sock);
	}
	clt->rsp_buff[rcv_length] = '\0';
}

char	*prompt_pass(t_clt *clt)
{
	struct termios	new_termios;
	char			*pass;

	if (tcgetattr (0, &clt->old_termios) != 0)
		clt->old_termios_set = 1;
	new_termios = clt->old_termios;
	clt->old_termios_set = 1;
	new_termios.c_lflag &= ~ECHO;
	tcsetattr (0, TCSAFLUSH, &new_termios);
	if (get_next_line(0, &pass) == -1)
	{
		if (clt->old_termios_set == 1)
			tcsetattr(0, TCSAFLUSH, &clt->old_termios);
		exit_socket("Failed to read on stdin", 1, clt->sock);
	}
	tcsetattr(0, TCSAFLUSH, &clt->old_termios);
	clt->old_termios_set = 0;
	return (pass);
}

void	send_data(t_clt *clt, char *cmd, char *param)
{
	size_t		len_param;
	size_t		len_cmd;
	char		*request;

	len_param = ft_strlen(param);
	len_cmd = ft_strlen(cmd);
	param[len_param] = '\n';
	request = malloc(len_cmd + len_param + 1);
	ft_memcpy(request, cmd, len_cmd);
	ft_memcpy(request + len_cmd, param, len_param + 1);
	if (send(clt->sock, request, len_cmd + len_param + 1, 0) == -1)
	{
		free(request);
		exit_socket("Error while sending user input", 1, clt->sock);
	}
	free(request);
}

void	init_connection(t_clt *clt)
{
	char		*input;
	char		*pass;

	ft_printf("User: ");
	if (get_next_line(0, &input) != 1)
		exit_socket("Error while reading user input", 1, clt->sock);
	send_data(clt, "USER ", input);
	free(input);
	if (get_response(clt, clt->rsp_buff) < 400)
		ft_printf("Pass: ");
	pass = prompt_pass(clt);
	write(1, "\n", 1);
	send_data(clt, "PASS ", pass);
	free(pass);
	get_response(clt, clt->rsp_buff);
}

void	handle_connection_clt(t_clt *clt)
{
	char		*input;
	size_t		len_input;

	if (get_response(clt, clt->rsp_buff) != 220)
		exit_socket("Server failed to connect", 1, clt->sock);
	init_connection(clt);
	while (42)
	{
		ft_printf(BIN_CLT"> ");
		if (get_next_line(0, &input) != 1)
			exit_socket("Error while reading user input", 1, clt->sock);
		len_input = ft_strlen(input);
		input[len_input] = '\n';
		if (send(clt->sock, input, len_input + 1, 0) == -1)
			exit_socket("Error while sending user input", 1, clt->sock);
		if (ft_strcmp("bye", input) == 0 || ft_strcmp("close", input) == 0)
			exit_socket("End of connection", 1, clt->sock);
		get_response(clt, clt->rsp_buff);
		free(input);
	}
}
