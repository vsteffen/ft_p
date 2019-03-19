/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mallocp.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: magouin <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/09/02 00:00:04 by magouin           #+#    #+#             */
/*   Updated: 2016/11/20 14:02:30 by magouin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void					*mallocp(size_t size)
{
	void	*ret;

	ret = malloc(size);
	if (!ret)
		exit(EXIT_FAILURE);
	return (ret);
}
