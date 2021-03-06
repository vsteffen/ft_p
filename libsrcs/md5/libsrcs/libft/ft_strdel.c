/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strdel.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: magouin <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2015/11/24 13:38:10 by magouin           #+#    #+#             */
/*   Updated: 2016/02/01 15:34:21 by magouin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>

void	ft_strdel(char **ap)
{
	if (!ap)
		return ;
	if (!*ap)
		return ;
	free((void*)*ap);
	*ap = NULL;
}
