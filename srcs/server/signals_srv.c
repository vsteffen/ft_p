
#include "ft_p.h"

t_srv	*get_srv(t_srv *srv, int i)
{
	static t_srv *get_srv = NULL;

	if (i)
		get_srv = srv;
	return (get_srv);
}

void		signal_handler_srv(int s)
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
