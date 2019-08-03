/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: niragne <niragne@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/06/19 13:02:13 by niragne           #+#    #+#             */
/*   Updated: 2019/08/01 16:40:22 by niragne          ###   ########.fr       */
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
	int ret;

	if (init_browser(browser, filename))
		return (1);
	init_parser(&parser, browser->ptr, 0, filename);
	if ((ret = fill_browser(&parser, browser)))
		return (1);
	otool_print(browser);
	ft_tree_del(&browser->parsers, free_parser_tree);
	if (munmap(browser->ptr, browser->st.st_size) != 0)
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

    t_arg_parser parser;

	opt_init_parser(&parser);
    opt_add_to_parser(&parser, (t_arg_option){"reverse", 'r', 0, 0, 0, 0, 1});
    opt_add_to_parser(&parser, (t_arg_option){"banane", 'b', 0, 0, 0, 0, 2});
    opt_add_to_parser(&parser, (t_arg_option){"pute", 'p', 0, 0, 0, 0, 2});
	opt_print_parser_opt(&parser);
	opt_parse_args(&parser, av);
	opt_print_parsed(&parser);
	
	
/*
	init_browser_general(&browser, av[0]);
	i = 1;
	if (ac <= 1)
		ft_dprintf(2, "%s: at least one file must be specified\n", browser.progname);
	while (i < ac)
	{
		if (process_otool(av[i++], &browser))
			return (EXIT_FAILURE);
	}
	return (browser.ret);*/
}
