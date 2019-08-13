/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: niragne <niragne@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/06/19 13:02:13 by niragne           #+#    #+#             */
/*   Updated: 2019/08/13 18:03:02 by niragne          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_otool.h"

t_arg_option g_opts[] = 
{
	{"mach-header", 'h', flag_h, "print the mach headers"},
	{"text", 't', flag_t, "print the text section"},
	{"data", 'd', flag_d, "print the data section"}
};

/*
** dump obj filename by creating a list of the parsed architectures
** in fill_browser, then printing it one by one in nm_print
*/

int		process_otool(char *filename, t_browser *browser, t_otool_flags *flags)
{
	t_header_parser	parser;
	int ret;

	(void)flags;
	if (init_browser(browser, filename))
		return (1);
	init_parser(&parser, browser->ptr, 0, filename);
	if ((ret = fill_browser(&parser, browser, 1)))
	{
		if (ret == ERROR_CORRUPTED)
		{
			otool_print(browser, flags);
		}
//		free_parser(&parser);
		ft_tree_del(&browser->parsers, free_parser_tree);
		free_browser(browser);
		return (1);
	}
	otool_print(browser, flags);
	ft_tree_del(&browser->parsers, free_parser_tree);
	if (free_browser(browser))
		return (1);
	return (0);
}

int		check_valid(t_arg_parser *parser, t_otool_flags *flags)
{
	char *shorts;

	if (parser->nb_args == 0)
	{
		print_usage(parser);
		return (1);
	}
	if (!flags->valid)
	{
		shorts = get_shorts(parser);
		ft_dprintf(2, "error: %s: one of -%s must be specified\n", parser->prog_name, shorts);
		free(shorts);
		print_usage(parser);
		return (1);
	}
	return (0);
}

int		process_args(t_arg_parser *parser, t_otool_flags *flags, t_browser *browser)
{
	t_list  *lst;

    lst = parser->parsed;
	if (check_valid(parser, flags))
		return (1);
	while (lst)
    {
        t_arg_parsed *test;
        test = (t_arg_parsed*)lst->content;
		if (test->type == E_ARG)
		{
			if (process_otool(test->long_name, browser, flags))
				return (1);
		}
        lst = lst->next;
    }
	return (0);
}

int		main(int ac, char **av)
{
	t_browser	browser;
	t_otool_flags flags;
    t_arg_parser parser;

	(void)ac;
	ft_bzero(&flags, sizeof(flags));
	opt_init_parser(&parser, flag_invalid, av[0]);
    opt_add_to_parser(&parser, g_opts, sizeof(g_opts));
	opt_parse_args(&parser, av + 1);
	process_opt(&parser, &flags);
	init_browser_general(&browser, av[0], E_BIN_OTOOL);
	if (process_args(&parser, &flags, &browser))
		return (opt_free(&parser, EXIT_FAILURE));
	return (opt_free(&parser, browser.ret));
}
