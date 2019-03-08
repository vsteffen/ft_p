#include "ft_p.h"

struct s_cmd	g_cmd[] = {
	{"LS", 2, handle_list},
	{"LOGIN", 5, init_connection},
	{NULL, 0, NULL},
};

char	*get_address_from_split(char **split)
{
	char	*addr;
	int		i;
	int		curr;

	i = 0;
	curr = 0;
	addr = malloc(ft_strlen(split[0]) + ft_strlen(split[1]) + ft_strlen(
		split[2]) + ft_strlen(split[3]) + 4);
	while (i < 4)
	{
		ft_strcpy(addr + curr, split[i]);
		curr += ft_strlen(split[i]) + 1;
		addr[curr - 1] = '.';
		i++;
	}
	addr[curr - 1] = 0;
	return (addr);
}

void	free_split(char **split)
{
	int	i;

	i = 0;
	while (split[i])
	{
		free(split[i]);
		i++;
	}
	free(split);
}

int		create_pasv(struct s_clt *clt)
{
	char		*infos;
	char		**split;
	int			port;
	char		*first_char;
	char		*end_char;

	send_data(clt, "PASV", NULL);
	if (get_response(clt, clt->rsp_buff) != 227)
		return (-1);
	first_char = ft_strchr(clt->rsp_buff, '(');
	end_char = ft_strchr(clt->rsp_buff, ')');
	if (!first_char || !end_char)
		return (-1);
	infos = ft_strsub(clt->rsp_buff, first_char - clt->rsp_buff + 1,
		ft_strchr(clt->rsp_buff, ')') - first_char - 1);
	split = ft_strsplit(infos, ',');
	if (!split || !split[0] || !split[1] || !split[2]
		|| !split[3] || !split[4] || !split[5] || split[6])
		return (-1);
	port = ft_atoi(split[4]) * 256 + ft_atoi(split[5]);
	infos = get_address_from_split(split);
	free_split(split);
	port = create_client(infos, port);
	free(infos);
	return (port);
}



void	handle_list(struct s_clt *clt, char *input)
{
	int			sock;
	ssize_t		rcv_length;
	char		buff[RSP_BUFF + 1];

	if (*input != ' ' && *input)
		return ;
	if ((sock = create_pasv(clt)) == -1)
		return ;
	send_data(clt, "LIST", *input == 0 ? NULL : input);
	get_response(clt, clt->rsp_buff);
	while ((rcv_length = read(sock, buff, RSP_BUFF)) > 0)
	{
		buff[rcv_length] = '\0';
		ft_printf("%s", buff);
	}
	// while ((rcv_length = recv(sock, buff, RSP_BUFF, 0)) > 0)
	// {
	// 	buff[rcv_length] = '\0';
	// 	ft_printf("%s", buff);
	// }
	close(sock);
}

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
	int				ret_gnl;

	if (tcgetattr (0, &clt->old_termios) != 0)
		clt->old_termios_set = 1;
	new_termios = clt->old_termios;
	new_termios.c_lflag &= ~ECHO;
	tcsetattr (0, TCSAFLUSH, &new_termios);
	if ((ret_gnl = get_next_line(0, &pass)) == -1)
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

	len_param = param ? ft_strlen(param) : 0;
	len_cmd = ft_strlen(cmd);
	request = malloc(len_cmd + len_param + 1);
	ft_strcpy(request, cmd);
	if (param)
		ft_strcpy(request + len_cmd, param);
	request[len_cmd + len_param] = '\n';
	if (send(clt->sock, request, len_cmd + len_param + 1, 0) == -1)
	{
		free(request);
		exit_socket("Error while sending user input", 1, clt->sock);
	}
	free(request);
}

void	init_connection(t_clt *clt, char *input)
{
	char		*pass;
	char		*user;
	int			ret_gnl;

	if (input && *input == ' ')
		user = ft_strdup(input + 1);
	else
	{
		ft_printf("User: ");
		if ((ret_gnl = get_next_line(0, &user)) == -1)
			exit_socket("Error while reading user input", 1, clt->sock);
		else if (ret_gnl == 0)
		{
			ft_printf("EOF received; login aborted.\n");
			return ;
		}
	}
	send_data(clt, "USER ", user);
	free(user);
	if (get_response(clt, clt->rsp_buff) < 400)
		ft_printf("Pass: ");
	pass = prompt_pass(clt);
	write(1, "\n", 1);
	send_data(clt, "PASS ", pass);
	free(pass);
	get_response(clt, clt->rsp_buff);
}

void	parse_cmd(char *input, t_clt *clt)
{
	size_t		len_input;
	int			i;
	char		*tmp;

	i = 0;
	len_input = ft_strlen(input);
	// input[len_input] = '\n';
	ft_strtoupper((tmp = ft_strdup(input)));
	while (g_cmd[i].key)
	{
		if (!ft_strncmp(g_cmd[i].key, tmp, g_cmd[i].len_key))
		{
			g_cmd[i].f(clt, input + g_cmd[i].len_key);
			break ;
		}
		i++;
	}
	free(tmp);
	// if (send(clt->sock, input, len_input + 1, 0) == -1)
	// 	exit_socket("Error while sending user input", 1, clt->sock);
	// if (ft_strcmp("bye", input) == 0 || ft_strcmp("close", input) == 0)
	// 	exit_socket("End of connection", 1, clt->sock);
	// get_response(clt, clt->rsp_buff);
	free(input);
	ft_printf(BIN_CLT"> ");
}

void	handle_connection_clt(t_clt *clt)
{
	char		*input;
	int			ret_gnl;

	if (get_response(clt, clt->rsp_buff) != 220)
		exit_socket("Server failed to connect", 1, clt->sock);
	init_connection(clt, NULL);
	ft_printf(BIN_CLT"> ");
	while (42)
	{
		if ((ret_gnl = get_next_line(0, &input)) == -1)
			exit_socket("Error while reading user input", 1, clt->sock);
		if (ret_gnl != 0)
			parse_cmd(input, clt);
		else
		{
			send_data(clt, "QUIT", NULL);
			write(1, "\n", 1);
			get_response(clt, clt->rsp_buff);
			exit_socket("Bye !", 1, clt->sock);
		}
	}
}
