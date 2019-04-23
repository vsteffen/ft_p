/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vsteffen <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/02/18 22:11:47 by vsteffen          #+#    #+#             */
/*   Updated: 2019/02/18 22:11:49 by vsteffen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <clt.h>

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
	while ((rcv_length = recv(sock, buff, RSP_BUFF, 0)) > 0)
	{
		buff[rcv_length] = '\0';
		ft_printf("%s", buff);
	}
	get_response(clt, clt->rsp_buff);
	close(sock);
}

void	handle_cd(struct s_clt *clt, char *input)
{
	if (*input != ' ' && *input)
		return ;
	send_data(clt, "CWD", *input == 0 ? NULL : input);
	get_response(clt, clt->rsp_buff);
}

void	handle_pwd(struct s_clt *clt, char *input)
{
	(void)input;
	send_data(clt, "PWD", NULL);
	get_response(clt, clt->rsp_buff);
}

void	handle_quit(t_clt *clt, char *input)
{
	(void)input;
	send_data(clt, "QUIT", NULL);
	get_response(clt, clt->rsp_buff);
	exit_socket("\nBye !", 1, clt->sock);
}

void	handle_put(struct s_clt *clt, char *input)
{
	int			sock;
	ssize_t		rcv_length;
	char		buff[RSP_BUFF + 1];
	int			fd;

	if (*input != ' ')
		return ;
	fd = open(input + 1, O_RDONLY);
	if (fd == -1)
		return ((void)ft_printf("Can't open file\n"));
	if ((sock = create_pasv(clt)) == -1)
		return ;
	send_data(clt, "STOR", *input == 0 ? NULL : input);
	if (get_response(clt, clt->rsp_buff) >= 300)
	{
		close(fd);
		close(sock);
		return ;
	}
	while ((rcv_length = read(fd, buff, RSP_BUFF)) > 0)
		send(sock, buff, rcv_length, 0);
	close(fd);
	close(sock);
	get_response(clt, clt->rsp_buff);
}
