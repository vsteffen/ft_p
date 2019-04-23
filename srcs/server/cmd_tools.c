/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd_tools.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: magouin <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/04/23 22:29:34 by magouin           #+#    #+#             */
/*   Updated: 2019/04/23 22:29:35 by magouin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <srv.h>

void	free_tab(char **tab)
{
	size_t	i;

	i = 0;
	while (tab[i])
		free(tab[i++]);
	free(tab);
}

int		fork_for_write(struct s_fork_params p, struct s_srv *srv,
	int sock, char *input)
{
	pid_t				child;
	int					ret_child;
	char				*str;
	char				**tmp;

	if ((child = fork()) == 0)
	{
		dup2(sock, 1);
		if (!(*input))
			input = ".";
		str = malloc(p.exec_len + ft_strlen(input));
		ft_strcpy(str, p.exec);
		ft_strcat(str, input);
		tmp = ft_strsplitwhite(str);
		sanitize_path(srv, tmp + p.first_arg);
		execv(p.bin, tmp);
		free_tab(tmp);
		free(str);
		exit(0);
	}
	else
		wait4(child, &ret_child, 0, NULL);
	return (ret_child);
}

void	no_such_file(struct s_srv *srv, char *input)
{
	char				*rsp;

	rsp = malloc(ft_strlen(input) + 34);
	ft_strcpy(rsp, "550 ");
	ft_strcat(rsp, input);
	ft_strcat(rsp, ": No such file or directory.\n");
	send(srv->cs, rsp, ft_strlen(input) + 33, 0);
	free(rsp);
}
