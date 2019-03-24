#ifndef SRV_H
# define SRV_H

# include <ft_p.h>
# define BIN_SRV "serveur"
# define SRV_DOCS "./docs/"
# define USER_FILE "users.conf"

typedef struct		s_srv {
	int 				sock;
	uint16_t			port;
	socklen_t			cs;
	struct sockaddr_in	csin;
	pid_t				pid;
	char				rsp_buff[RSP_BUFF];
	uint				id_user;
	int8_t				login;
	int8_t				auth;
	char				*user[SOCK_CONNECTION_QUEUE + 1][2];
	char				*docs; // Must free it
	char				*user_path; // Must free it
	int					sock_pasv;
}					t_srv;

typedef void	(t_func_cmd)(t_srv *srv, char *args);

struct		s_cmd {
	char		*key;
	size_t		len_key;
	t_func_cmd	*f;
	int			auth;
};

char				*get_path(char *av_0, char *file);
void				init_users(char *av_0, t_srv *srv);
char				*md5(char *md5);
size_t				search_user(t_srv *srv, char *user);

int8_t				check_passwd(t_srv *srv, char *pass);

void				free_users(char *users[SOCK_CONNECTION_QUEUE + 1][2]);;

void				handle_all_connection_srv(t_srv *srv);
void				handle_connection(t_srv *clt);
void				handle_request(t_srv *srv, char *request);
void				exit_message(char *message, uint8_t ret);

void				handle_request(t_srv *srv, char *request);

t_srv				*get_srv(t_srv *srv, int i);
void				signal_handler_srv(int s);
int					create_client(char *addr, uint16_t port);
char				*get_docs_path();

void				exit_socket(char *message, int ret, int sock);

void				handle_list(struct s_srv *srv, char *input);
void				handle_cwd(struct s_srv *srv, char *input);
void				handle_pwd(struct s_srv *srv, char *input);
void				handle_retr(struct s_srv *srv, char *input);
void				handle_stor(struct s_srv *srv, char *input);
void				handle_user(t_srv *srv, char *input);
void				handle_pass(t_srv *srv, char *input);
void				handle_pasv(struct s_srv *srv, char *input);
void				handle_type(struct s_srv *srv, char *input);
void				handle_quit(t_srv *srv, char *input);
void				handle_syst(struct s_srv *srv, char *input);

int					create_server(uint16_t port, int cient_nbr);

#endif
