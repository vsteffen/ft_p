/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_cmd_get_auth.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: magouin <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/04/23 20:06:27 by magouin           #+#    #+#             */
/*   Updated: 2019/04/23 20:06:29 by magouin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <clt.h>

char	*prompt_pass(t_clt *clt)
{
	struct termios	new_termios;
	char			*pass;
	int				ret_gnl;

	if (tcgetattr(0, &clt->old_termios) != 0)
		clt->old_termios_set = 1;
	new_termios = clt->old_termios;
	new_termios.c_lflag &= ~ECHO;
	tcsetattr(0, TCSAFLUSH, &new_termios);
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

int8_t	handle_auth_send_user(t_clt *clt, char *input)
{
	int		ret_gnl;
	char	*user;

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
			return (0);
		}
	}
	send_data(clt, "USER ", user);
	free(user);
	return (1);
}

void	handle_auth(t_clt *clt, char *input)
{
	char		*pass;

	if (!handle_auth_send_user(clt, input))
		return ;
	if (get_response(clt, clt->rsp_buff) < 400)
		ft_printf("Pass: ");
	pass = prompt_pass(clt);
	write(1, "\n", 1);
	send_data(clt, "PASS ", pass);
	free(pass);
	if (get_response(clt, clt->rsp_buff) < 300)
	{
		send_data(clt, "TYPE", " I");
		if (get_response(clt, clt->rsp_buff) >= 300)
			ft_printf("Error binary mode\n");
	}
}

void	handle_get_child(int sock, char *input)
{
	char		*name;
	int			fd;
	ssize_t		rcv_length;
	char		buff[RSP_BUFF + 1];

	name = ft_strrchr(input, '/');
	name = name ? name + 1 : input + 1;
	fd = open(name, O_CREAT | O_WRONLY | O_TRUNC, 0644);
	if (fd == -1)
	{
		ft_printf("Can't open file\n");
		close(sock);
		return ;
	}
	while ((rcv_length = recv(sock, buff, RSP_BUFF, 0)) > 0)
		write(fd, buff, rcv_length);
	close(fd);
	exit(1);
}

void	handle_get(struct s_clt *clt, char *input)
{
	int			sock;
	pid_t		child;
	int			status;

	if (*input != ' ' || (sock = create_pasv(clt)) == -1)
		return ;
	if ((child = fork()))
	{
		send_data(clt, "RETR", *input == 0 ? NULL : input);
		while (get_response(clt, clt->rsp_buff) < 200)
			;
		if (clt->rsp_buff[0] != '2')
		{
			ft_printf("Failed to RETR\n");
			close(sock);
			return ;
		}
		close(sock);
		status = 0;
		wait4(child, &status, 0, NULL);
		return ;
	}
	else
		handle_get_child(sock, input);
}
