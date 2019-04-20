/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   serveur.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vsteffen <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/02/18 22:11:47 by vsteffen          #+#    #+#             */
/*   Updated: 2019/02/18 22:11:49 by vsteffen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <clt.h>

t_clt	*get_clt(t_clt *clt, int i)
{
	static t_clt *get_clt = NULL;

	if (i)
		get_clt = clt;
	return (get_clt);
}

void	signal_handler_clt(int s)
{
	t_clt		*clt;

	if (s == SIGABRT || s == SIGALRM || s == SIGFPE || s == SIGHUP || \
		s == SIGILL || s == SIGINT || s == SIGPIPE || s == SIGQUIT || \
		s == SIGSEGV || s == SIGTERM || s == SIGUSR1 || s == SIGUSR2)
	{
		clt = get_clt(NULL, 0);
		close(clt->sock);
		if (clt->old_termios_set == 1)
			tcsetattr(0, TCSAFLUSH, &clt->old_termios);
		ft_printf("\nClient stopped\n");
		exit(0);
	}
}
