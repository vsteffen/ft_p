#include <srv.h>

size_t	search_user(t_srv *srv, char *user)
{
	size_t	id;

	id = 0;
	while (srv->user[id][0] != NULL)
	{
		ft_printf("Pouet -> [%s] / user [%s]\n", srv->user[id][0], user);
		if (!ft_strcmp(srv->user[id][0], user))
			return (id);
		id++;
	}
	return (-1);
}

int8_t	check_passwd(t_srv *srv, char *pass)
{
	char	*hash;
	int8_t	ret;

	hash = md5(pass);
	if (srv->id_user == -1u || srv->id_user > SOCK_CONNECTION_QUEUE)
		return (0);
	if (!ft_strcmp(srv->user[srv->id_user][1], hash))
		ret = 1;
	else
		ret = 0;
	free(hash);
	return (ret);
}

void	free_users(char *users[SOCK_CONNECTION_QUEUE + 1][2])
{
	size_t	id;

	id = 0;
	while (users[id][0] != NULL)
	{
		free(users[id][0]);
		id++;
	}
}

void	init_users(char *av_0, t_srv *srv)
{
	int		fd;
	char	*line;
	char	*pass;
	size_t	id;

	fd = open(get_path(av_0, USER_FILE), O_RDONLY);
	if (fd == -1)
		exit_message("No "USER_FILE" or can't read it", 1);
	id = 0;
	while (get_next_line(fd, &line) > 0 && id < SOCK_CONNECTION_QUEUE)
	{
		pass = ft_strrchr(line, ':') + 1;
		if (!pass)
			exit_message("Invalid users file ("USER_FILE")", 1);
		*(pass - 1) = '\0';
		srv->user[id][0] = line;
		srv->user[id][1] = pass;
		id++;
	}
	srv->user[id][0] = NULL;
	srv->user[id][1] = NULL;
	close(fd);
}
