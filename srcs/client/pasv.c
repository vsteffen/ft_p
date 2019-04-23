/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pasv.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: magouin <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/04/23 20:26:40 by magouin           #+#    #+#             */
/*   Updated: 2019/04/23 20:26:41 by magouin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <clt.h>

char	*get_address_from_split(char **split)
{
	char	*addr;
	int		i;
	int		curr;

	i = 0;
	curr = 0;
	addr = malloc(ft_strlen(split[0]) + ft_strlen(split[1]) + ft_strlen(
		split[2]) + ft_strlen(split[3]) + 4);
	while (i < 4)
	{
		ft_strcpy(addr + curr, split[i]);
		curr += ft_strlen(split[i]) + 1;
		addr[curr - 1] = '.';
		i++;
	}
	addr[curr - 1] = 0;
	return (addr);
}

void	free_split(char **split)
{
	int	i;

	i = 0;
	while (split[i])
	{
		free(split[i]);
		i++;
	}
	free(split);
}

int		send_pasv(struct s_clt *clt)
{
	send_data(clt, "PASV", NULL);
	if (get_response(clt, clt->rsp_buff) != 227)
	{
		ft_printf("%s\n", clt->rsp_buff);
		return (0);
	}
	return (1);
}

int		create_pasv(struct s_clt *clt)
{
	char		*infos;
	char		**split;
	int			port;
	char		*first_char;
	char		*end_char;

	if (!send_pasv(clt))
		return (-1);
	first_char = ft_strrchr(clt->rsp_buff, '(');
	end_char = ft_strrchr(clt->rsp_buff, ')');
	if (!first_char || !end_char)
		return (-1);
	infos = ft_strsub(clt->rsp_buff, first_char - clt->rsp_buff + 1,
		end_char - first_char - 1);
	split = ft_strsplit(infos, ',');
	free(infos);
	if (!split || !split[0] || !split[1] || !split[2]
		|| !split[3] || !split[4] || !split[5] || split[6])
		return (-1);
	port = ft_atoi(split[4]) * 256 + ft_atoi(split[5]);
	infos = get_address_from_split(split);
	free_split(split);
	port = create_client(infos, port);
	free(infos);
	return (port);
}
