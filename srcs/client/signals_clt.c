
#include "ft_p.h"

t_clt	*get_clt(t_clt *clt, int i)
{
	static t_clt *get_clt = NULL;

	if (i)
		get_clt = clt;
	return (get_clt);
}

void		signal_handler_clt(int s)
{
	t_clt		*clt;

	if (s == SIGABRT || s == SIGALRM || s == SIGFPE || s == SIGHUP || \
		s == SIGILL || s == SIGINT || s == SIGPIPE || s == SIGQUIT || \
		s == SIGSEGV || s == SIGTERM || s == SIGUSR1 || s == SIGUSR2)
	{
		clt = get_clt(NULL, 0);
		close(clt->sock);
		ft_printf("\nClient stopped\n");
		exit(0);
	}
}
