/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: niragne <niragne@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/06/19 13:02:13 by niragne           #+#    #+#             */
/*   Updated: 2019/07/18 16:52:27 by ldedier          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_otool.h"

/*
** dump obj filename by creating a list of the parsed architectures
** in fill_browser, then printing it one by one in nm_print
*/
int		process_otool(char *filename, t_browser *browser)
{
	t_header_parser	parser;

	if (init_browser(browser, filename))
		return (1);
	init_parser(&parser, browser->ptr, 0, filename);
	if (fill_browser(&parser, browser))
		return (1);
	otool_print(browser);
	ft_tree_del(&browser->parsers, free_parser_tree);
	if (munmap(browser->ptr, browser->st.st_size) < 0)
	{
		ft_dprintf(2, "could not munmap the file %s\n", filename);
		return (1);
	}
	return (0);
}

int		main(int ac, char **av)
{
	t_browser	browser;
	int				i;

	init_browser_general(&browser, av[0]);
	i = 1;
	while (i < ac)
	{
		if (process_otool(av[i++], &browser))
			return (EXIT_FAILURE);
	}
	return (browser.ret);
}
