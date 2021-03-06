/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals_srv.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: magouin <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/04/23 20:31:54 by magouin           #+#    #+#             */
/*   Updated: 2019/04/23 20:31:59 by magouin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <srv.h>

t_srv	*get_srv(t_srv *srv, int i)
{
	static t_srv *get_srv = NULL;

	if (i)
		get_srv = srv;
	return (get_srv);
}

void	signal_handler_srv(int s)
{
	t_srv		*srv;

	if (s == SIGABRT || s == SIGALRM || s == SIGFPE || s == SIGHUP || \
		s == SIGILL || s == SIGINT || s == SIGPIPE || s == SIGQUIT || \
		s == SIGSEGV || s == SIGTERM || s == SIGUSR1 || s == SIGUSR2)
	{
		srv = get_srv(NULL, 0);
		if (srv->pid == 0)
		{
			close(srv->sock);
			if (srv->sock_pasv != -1)
				close(srv->sock_pasv);
			ft_printf("\nServer stopped\n");
			exit(0);
		}
		else
		{
			close(srv->cs);
			exit(0);
		}
	}
}
