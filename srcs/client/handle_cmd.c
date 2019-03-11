#include "ft_p.h"

void	handle_list(struct s_clt *clt, char *input)
{
	int			sock;
	ssize_t		rcv_length;
	char		buff[RSP_BUFF + 1];
	int			ret;

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

void	handle_auth(t_clt *clt, char *input)
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
	send_data(clt, "TYPE", " I");
	if (get_response(clt, clt->rsp_buff) >= 300)
		ft_printf("Error binary mode\n");
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

void	handle_get(struct s_clt *clt, char *input)
{
	int			sock;
	ssize_t		rcv_length;
	char		buff[RSP_BUFF + 1];
	int			fd;
	char		*name;
	int			ret;
	pid_t		child;
	int			status;

	if (*input != ' ')
		return ;
	name = ft_strrchr(input, '/');
	name = name ? name + 1 : input + 1;
	if ((sock = create_pasv(clt)) == -1)
		return ;
	if ((child = fork()))
	{
		send_data(clt, "RETR", *input == 0 ? NULL : input);
		while ((ret = get_response(clt, clt->rsp_buff)) < 200)
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
	{
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
}

void	handle_put(struct s_clt *clt, char *input)
{
	int			sock;
	ssize_t		rcv_length;
	char		buff[RSP_BUFF + 1];
	int			fd;
	char		*name;

	if (*input != ' ')
		return ;
	name = ft_strrchr(input, '/');
	name = name ? name + 1 : input + 1;
	if ((sock = create_pasv(clt)) == -1)
		return ;
	send_data(clt, "STOR", *input == 0 ? NULL : input);
	get_response(clt, clt->rsp_buff);
	fd = open(name, O_RDONLY);
	if (fd == -1)
	{
		ft_printf("Can't open file\n");
		close(sock);
		exit(0);
	}
	while ((rcv_length = read(fd, buff, RSP_BUFF)) > 0)
		send(sock, buff, rcv_length, 0);
	close(fd);
	close(sock);
	get_response(clt, clt->rsp_buff);
}
