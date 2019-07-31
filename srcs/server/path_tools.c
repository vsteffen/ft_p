/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   path_tools.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: magouin <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/04/23 22:25:11 by magouin           #+#    #+#             */
/*   Updated: 2019/07/31 20:22:51 by vsteffen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <srv.h>

size_t	count_depth(char *s)
{
	size_t	i;
	int		depth;
	int		nb_dot;

	depth = 0;
	i = 0;
	while (s[i])
	{
		if (s[i] == '/')
		{
			i++;
			nb_dot = depth;
			if (s[i] == '.')
				while (s[++i] == '.')
					if (depth-- == 1)
						return (0);
			if (s[i] != '/' && s[i] != '\0')
				depth = nb_dot + 1;
			i--;
		}
		i++;
	}
	return (depth);
}

void	update_tab_depth(struct s_fln *n, char *s)
{
	if (s[n->i] != '/' && s[n->i] != '\0')
		n->depth = n->tmp_depth + 1;
	else
	{
		ft_strcpy(s + n->tab_depth[n->curr - (n->
			tmp_depth - n->depth)], s + n->i);
		n->i = n->tab_depth[n->curr - (n->tmp_depth - n->depth)];
		n->curr = n->tmp_depth - 1;
	}
}

char	*clean_path(char *s)
{
	struct s_fln n;

	n.depth = 0;
	n.i = 0;
	n.curr = 0;
	while (s[n.i])
	{
		if (s[n.i] == '/')
		{
			n.tab_depth[n.curr] = (n.i)++;
			n.tmp_depth = n.depth;
			if (s[n.i] == '.')
				while (s[++(n.i)] == '.')
					if (n.depth-- == 1)
						return (NULL);
			update_tab_depth(&n, s);
			n.curr++;
			n.i--;
		}
		n.i++;
	}
	return (s);
}

void	sanitize_path(t_srv *srv, char **tab)
{
	size_t	i;
	char	*tmp;

	i = 0;
	while (tab[i])
	{
		if (*tab[i] == '/')
			tmp = ft_strjoin(SRV_DOCS, tab[i] + 1);
		else
			tmp = ft_strjoin(srv->user_path, tab[i]);
		tmp = clean_path(tmp);
		free(tab[i]);
		if (tmp)
			tab[i] = tmp;
		else
			tab[i] = ft_strdup(SRV_DOCS);
		i++;
	}
}
