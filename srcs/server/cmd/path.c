/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd_path.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: magouin <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/04/23 20:58:44 by magouin           #+#    #+#             */
/*   Updated: 2019/07/31 20:26:28 by vsteffen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <srv.h>

DIR		*init_dir(struct s_srv *srv, char *input, char **tmp, char **tmp2)
{
	DIR			*fd;

	input[ft_strlen(input) - 1] = '\0';
	if (*input == '/')
		*tmp = ft_strjoin(SRV_DOCS, input + 1);
	else
		*tmp = ft_strjoin(srv->user_path, input);
	*tmp2 = *tmp;
	if (!(*tmp = clean_path(*tmp)))
	{
		free(*tmp2);
		send(srv->cs, "550 No such file or directory.\n", 31, 0);
		return (NULL);
	}
	else if (!(fd = opendir(*tmp)))
	{
		send(srv->cs, "550 No such file or directory.\n", 31, 0);
		return (NULL);
	}
	return (fd);
}

void	handle_cwd(struct s_srv *srv, char *input)
{
	char		*tmp;
	char		*tmp2;
	DIR			*fd;

	if (!(fd = init_dir(srv, input, &tmp, &tmp2)))
		return ;
	if (tmp[ft_strlen(tmp) - 1] != '/')
	{
		free(tmp2);
		tmp2 = malloc(ft_strlen(tmp) + 2);
		ft_strcpy(tmp2, tmp);
		tmp = tmp2;
		tmp[ft_strlen(tmp) + 1] = 0;
		tmp[ft_strlen(tmp)] = '/';
	}
	closedir(fd);
	free(srv->user_path);
	srv->user_path = tmp;
	send(srv->cs, "250 CWD command successful.\n", 28, 0);
}

void	handle_pwd(struct s_srv *srv, char *input)
{
	char	*rsp;
	char	*pwd;

	(void)input;
	pwd = srv->user_path + ft_strlen(SRV_DOCS) - 1;
	rsp = malloc(ft_strlen(pwd) + 30);
	ft_strcpy(rsp, "257 \"");
	ft_strcat(rsp, pwd);
	ft_strcat(rsp, "\" is current directory.\n");
	send(srv->cs, rsp, ft_strlen(pwd) + 29, 0);
	free(rsp);
}
