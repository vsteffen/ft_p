
#include "ft_p.h"

t_ftp_srv	*singleton(t_ftp_srv *ftp_srv, int i)
{
	static t_ftp_srv *singleton = NULL;

	if (i)
		singleton = ftp_srv;
	return (singleton);
}

void		signal_handler(int s)
{
	if (s == SIGABRT || s == SIGALRM || s == SIGFPE || s == SIGHUP || \
		s == SIGILL || s == SIGINT || s == SIGPIPE || s == SIGQUIT || \
		s == SIGSEGV || s == SIGTERM || s == SIGUSR1 || s == SIGUSR2)
	{
		close(singleton(NULL, 0)->sock);
		exit_message("Server stopped", 0);
	}
}
