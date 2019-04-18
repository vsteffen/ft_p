#include <clt.h>

struct s_cmd	g_cmd[] = {
	{"LS\n", 3, handle_list},
	{"LS ", 3, handle_list},
	{"LOGIN\n", 6, handle_auth},
	{"LOGIN ", 6, handle_auth},
	{"QUIT\n", 5, handle_quit},
	{"CD ", 3, handle_cd},
	{"PWD\n", 4, handle_pwd},
	{"GET ", 4, handle_get},
	{"PUT ", 4, handle_put},
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
	{
		printf("%s\n", clt->rsp_buff);
		return (-1);
	}
	first_char = ft_strrchr(clt->rsp_buff, '(');
	end_char = ft_strrchr(clt->rsp_buff, ')');
	if (!first_char || !end_char)
		return (-1);
	infos = ft_strsub(clt->rsp_buff, first_char - clt->rsp_buff + 1,
		end_char - first_char - 1);
	split = ft_strsplit(infos, ',');
	free(infos);
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

void	exit_socket(char *message, int ret, int socket)
{
	close(socket);
	exit_message(message, ret);
}

int		get_response(t_clt *clt, char *response)
{
	ssize_t		rcv_length;
	int			ret;
	int			len;
	static char	*next = NULL;

	next = next ? next : response; 
	if (next == response || !*next)
	{
		if ((rcv_length = recv(clt->sock, response, RSP_BUFF, 0)) <= 0)
		{
			if (rcv_length == 0)
				exit_socket("Connection close by ftp sever", 1, clt->sock);
			else
				exit_socket("Error while receving data from ftp sever", 1,
					clt->sock);
		}
		response[rcv_length] = '\0';
	}
	else
		ft_strcpy(response , next);
	next = ft_strchr(response, '\n') + 1;
	if (next - 1 && *next)
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
	ret = ft_atoi(response);
	if (response[0] == '2' || response[0] == '3')
		ft_printf("%{FG_GREEN}SUCESS%{FG_DEFAULT} : ");
	else if (response[0] == '4' || response[0] == '5')
		ft_printf("%{FG_RED}ERROR%{FG_DEFAULT} : ");
	write(1, response, len);
	return (ret);
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
			input[ft_strlen(input) - 1] = 0;
			g_cmd[i].f(clt, input + g_cmd[i].len_key - 1);
			break ;
		}
		i++;
	}
	if (!g_cmd[i].key)
	{
		if (*input != '\0')
			ft_printf("%s: Unkown command.\n", input);
	}
	free(tmp);
	free(input);
	ft_printf(BIN_CLT"> ");
}

void	handle_connection_clt(t_clt *clt)
{
	char		*input;
	int			ret_gnl;

	if (get_response(clt, clt->rsp_buff) != 220)
		exit_socket("Server failed to connect", 1, clt->sock);
	handle_auth(clt, NULL);
	ft_printf(BIN_CLT"> ");
	while (42)
	{
		if ((ret_gnl = get_next_line(0, &input)) == -1)
			exit_socket("Error while reading user input", 1, clt->sock);
		if (ret_gnl != 0)
			parse_cmd(input, clt);
		else
			handle_quit(clt, NULL);

	}
}
