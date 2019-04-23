/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   request_tools.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: magouin <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/04/23 20:24:59 by magouin           #+#    #+#             */
/*   Updated: 2019/04/23 20:25:01 by magouin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <clt.h>

void	get_raw_response(t_clt *clt, char *response, char **next)
{
	ssize_t		rcv_length;

	*next = *next ? *next : response;
	if (*next == response || !**next)
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
		ft_strcpy(response, *next);
}

int		get_response(t_clt *clt, char *response)
{
	int			ret;
	int			len;
	static char	*next = NULL;

	get_raw_response(clt, response, &next);
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
			exit_socket("Error while receving data from ftp sever", 1, \
				clt->sock);
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
	request = malloc(len_cmd + len_param + 2);
	ft_strcpy(request, cmd);
	if (param)
		ft_strcat(request + len_cmd, param);
	request[len_cmd + len_param] = '\n';
	request[len_cmd + len_param + 1] = 0;
	if (send(clt->sock, request, len_cmd + len_param + 1, 0) == -1)
	{
		free(request);
		exit_socket("Error while sending user input", 1, clt->sock);
	}
	free(request);
}
