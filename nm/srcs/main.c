/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: niragne <niragne@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/06/19 13:02:13 by niragne           #+#    #+#             */
/*   Updated: 2019/08/09 14:40:43 by niragne          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_nm.h"

t_arg_option g_opts[] = 
{
	{"no-sort", 'p', nm_opt_p, "Don't sort; display in symbol-table order"},
	{"numeric-sort", 'n', nm_opt_n, "Sort numerically rather than alphabetically"},
	{"reverse-sort", 'r', nm_opt_r, "Sort in reverse order"}
};

/*
** dump obj filename by creating a list of the parsed architectures
** in fill_browser, then printing it one by one in nm_print
*/
int		process_nm(char *filename, t_browser *browser)
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

int		process_args(t_arg_parser *parser, t_browser *browser)
{
	t_list  *lst;

	browser->nb_args = parser->nb_args;
    lst = parser->parsed;
	while (lst)
    {
        t_arg_parsed *test;
        test = (t_arg_parsed*)lst->content;
		if (test->type == E_ARG)
		{
			process_nm(test->long_name, browser);
		}
        lst = lst->next;
    }
	return (0);
}

int		main(int ac, char **av)
{
	t_nm_wrapper	wrapper;
	t_browser		browser;
	t_nm_flags		flags;
	t_arg_parser	parser;

	(void)ac;
	wrapper.flags = &flags;
	wrapper.browser = &browser;
	init_browser_general(&browser, av[0]);
	opt_init_parser(&parser, flag_invalid, av[0]);
	opt_add_to_parser(&parser, g_opts, sizeof(g_opts));
	opt_parse_args(&parser, av + 1);
	process_opt(&parser, &wrapper);
	if (browser.ret)
		return (browser.ret);
	process_args(&parser, &browser);
	if (parser.nb_args == 0)
	{
		browser.nb_args = 1;
		if (process_nm(DEFAULT_NM_FILE, &browser))
			return (EXIT_FAILURE);
		return (browser.ret);
	}
	return (browser.ret);
}
