#ifndef FT_P_H
# define FT_P_H

# include "libft.h"
# include <sys/socket.h>
# include <netdb.h>
# include <arpa/inet.h>
# include <signal.h>

# define BIN_SRV "serveur"
# define BIN_CLT "client"
# define SOCK_CONNECTION_QUEUE 42
# define REQUEST_BUFF 1024

typedef struct		s_cmd {
	char			buff[REQUEST_BUFF];
}					t_cmd;

typedef struct		s_ftp_srv {
	int 				sock;
	uint16_t			port;
	socklen_t			cs;
	struct sockaddr_in	csin;
	struct s_cmd		cmd;
}					t_ftp_srv;

typedef struct		s_ftp_clt {
	int 				sock;
	char				*addr;
	uint16_t			port;
	socklen_t			cs;
	struct sockaddr_in	csin;
	struct s_cmd		cmd;
}					t_ftp_clt;


void				handle_connection(t_ftp_srv *ftp_srv);
void				exit_message(char *message, uint8_t ret);


t_ftp_srv			*singleton(t_ftp_srv *ftp_srv, int i);
void				signal_handler(int s);

#endif
