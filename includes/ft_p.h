#ifndef FT_P_H
# define FT_P_H

# include "libft.h"
# include <sys/socket.h>
# include <netdb.h>
# include <arpa/inet.h>
# include <signal.h>
# include <stdio.h>
# include <curses.h>
# include <termios.h>
# include <fcntl.h>

# define BIN_SRV "serveur"
# define BIN_CLT "client"
# define SOCK_CONNECTION_QUEUE 42
# define RSP_BUFF 8192

typedef struct		s_srv {
	int 				sock;
	uint16_t			port;
	socklen_t			cs;
	struct sockaddr_in	csin;
	pid_t				pid;
	char				rsp_buff[RSP_BUFF];
}					t_srv;

typedef struct		s_clt {
	int 				sock;
	char				*addr;
	uint16_t			port;
	socklen_t			cs;
	struct sockaddr_in	csin;
	char				rsp_buff[RSP_BUFF];
	struct termios		old_termios;
	int8_t				old_termios_set;
}					t_clt;

typedef void	(t_func_cmd)(t_clt *clt, char *args);

struct		s_cmd {
	char		*key;
	size_t		len_key;
	t_func_cmd	*f;
};


void				handle_connection_srv(t_srv *srv);
void				handle_connection_clt(t_clt *clt);
void				exit_message(char *message, uint8_t ret);

t_srv				*get_srv(t_srv *srv, int i);
t_clt				*get_clt(t_clt *clt, int i);
void				signal_handler_srv(int s);
void				signal_handler_clt(int s);
int					create_client(char *addr, uint16_t port);

void				exit_socket(char *message, int ret, int sock);
void				send_data(t_clt *clt, char *cmd, char *param);

int					get_response(t_clt *clt, char *response);
int					create_pasv(struct s_clt *clt);

void				handle_auth(t_clt *clt, char *input);
void				handle_list(struct s_clt *clt, char *input);
void				handle_quit(t_clt *clt, char *input);
void				handle_cd(struct s_clt *clt, char *input);
void				handle_pwd(struct s_clt *clt, char *input);
void				handle_get(struct s_clt *clt, char *input);
void				handle_put(struct s_clt *clt, char *input);

#endif
