/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   md5_parsing.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: magouin <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/01/24 16:40:04 by magouin           #+#    #+#             */
/*   Updated: 2019/01/24 16:40:07 by magouin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <ft_ssl.h>

int		padd_buffer(int original_file_size, int r, char *buffer)
{
	size_t	size;

	original_file_size += r;
	size = (r / 64 + 1) * 64;
	if (size - r <= 8)
		size += 64;
	((uint8_t*)buffer)[r] = 0x80;
	ft_bzero(buffer + r + 1, size - r - 8 - 1);
	*(uint64_t*)(buffer + size - 8) = original_file_size << 3;
	return (size);
}

char	*md5(char *str)
{
	size_t			original_len;
	size_t			end_len;
	void			*buffer;
	t_params_md5	params;
	char			*ret;

	initialize_buffer(params.buffer);
	initialize_t(params.t);
	original_len = ft_strlen(str);
	if (!(buffer = malloc((original_len + 64) * sizeof(char))))
		return (0);
	ft_strcpy(buffer, str);
	end_len = padd_buffer(0, original_len, buffer);
	hash_buffer_md5(end_len, &params, buffer);
	ret = print_result_32(params.buffer);
	free(buffer);
	return (ret);
}

int		if_r_smaller_than_zero(ssize_t *r, int *fd)
{
	if (*fd == 0)
	{
		*r = 0;
		*fd = -1;
		return (1);
	}
	else
	{
		close(*fd);
		ft_putstr_fd("Read error\n", 2);
		return (0);
	}
}
