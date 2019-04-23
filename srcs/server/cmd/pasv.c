/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd_pasv.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: magouin <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/04/23 22:39:41 by magouin           #+#    #+#             */
/*   Updated: 2019/04/23 22:39:42 by magouin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <srv.h>

void	convert_port_to_string(uint16_t port, char *buff)
{
	char	*str_nb;

	ft_strcat(buff, str_nb = ft_itoa(port >> 8));
	free(str_nb);
	ft_strcat(buff, ",");
	ft_strcat(buff, str_nb = ft_itoa(port & 0xFF));
	free(str_nb);
}

void	handle_pasv(struct s_srv *srv, char *input)
{
	char				ret[47];
	struct sockaddr_in	tmp_sin;
	socklen_t			tmp_slen;

	(void)input;
	if (srv->sock_pasv != -1)
		close(srv->sock_pasv);
	srv->sock_pasv = create_server(0, 1);
	getsockname(srv->sock_pasv, (struct sockaddr *)&tmp_sin, &tmp_slen);
	ft_strcpy(ret, "227 Entering Passive Mode (127,0,0,1,");
	convert_port_to_string(ntohs(tmp_sin.sin_port), ret + 37);
	ft_strcat(ret, ")\n");
	send(srv->cs, ret, ft_strlen(ret), 0);
}
