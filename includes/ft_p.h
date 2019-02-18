#ifndef FT_P_H
# define FT_P_H

# include "libft.h"
# include <sys/socket.h>
# include <netdb.h>
# include <arpa/inet.h>

# define SOCK_CONNECTION_QUEUE 42

typedef struct		s_elem {
	struct s_elem	*prec;
	struct s_elem	*next;
}					t_elem;

typedef struct		s_project {
	t_elem			*elem;
	t_elem			*current;
	t_elem			*last;
}					t_project;

typedef struct		s_ftp_srv {
	int 				sock;
	uint16_t			port;
	socklen_t			cs;
	struct sockaddr_in	csin;
}					t_ftp_srv;

typedef struct		s_ftp_clt {
}					t_ftp_clt;

#endif
