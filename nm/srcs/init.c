/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ldedier <ldedier@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/06/21 02:14:47 by ldedier           #+#    #+#             */
/*   Updated: 2019/06/21 02:14:47 by ldedier          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_nm.h"

int	init_browser(t_nm_browser *browser, char *filename)
{
	int			fd;

	browser->filename = filename;
	browser->symbols = NULL;
	if ((fd = open(filename, O_RDONLY)) < 0)
	{
		ft_dprintf(2, "error opening %s\n", filename);
		return (1);
	}
	if ((fstat(fd, &browser->st)) < 0)
	{
		ft_printf("error fstat\n");
		return (1);
	}
	if ((browser->ptr = mmap(0, browser->st.st_size,
		PROT_READ, MAP_PRIVATE, fd, 0)) == MAP_FAILED)
	{
		ft_dprintf(2, "could not map file %s\n", filename);
		return (1);
	}
	return (0);
}

void	init_browser_general(t_nm_browser *browser)
{
	browser->ret = 0;
	browser->sort_func = cmp_symbol_alpha;
	browser->sort_mult = 1;
}
