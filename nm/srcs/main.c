/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: niragne <niragne@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/06/19 13:02:13 by niragne           #+#    #+#             */
/*   Updated: 2019/07/17 16:27:08 by ldedier          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_nm.h"

/*
** dump obj filename by creating a list of the parsed architectures
** in fill_browser, then printing it one by one in nm_print
*/
int		process_nm(char *filename, t_nm_browser *browser)
{
	t_header_parser	parser;

	if (init_browser(browser, filename))
		return (1);
	init_parser(&parser, browser->ptr, 0, filename);
	if (fill_browser(&parser, browser))
		return (1);
	nm_print(browser, 0);
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
	t_nm_browser	browser;
	int				i;
	int				ret;

	init_browser_general(&browser, av[0]);
	if ((ret = parse_options(&i, ac, av, &browser)))
		return (ret == 1 ? 1 : 0);
	if (browser.ret)
		return (browser.ret);
	if (i == ac)
	{
		browser.nb_args = 1;
		if (process_nm(DEFAULT_NM_FILE, &browser))
			return (EXIT_FAILURE);
		return (browser.ret);
	}
	browser.nb_args = ac - i;
	while (i < ac)
	{
		if (process_nm(av[i++], &browser))
			return (EXIT_FAILURE);
	}
	return (browser.ret);
}
