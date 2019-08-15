/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: niragne <niragne@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/06/19 13:02:13 by niragne           #+#    #+#             */
/*   Updated: 2019/08/15 18:03:29 by ldedier          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_nm.h"

void	print_symbol_tree(t_header_parser *parser, t_tree *tree,
			t_browser *browser)
{
	if (tree)
	{
		print_symbol_tree(parser, tree->left, browser);
		print_symbol(parser, (t_symbol *)tree->content, browser);
		print_symbol_tree(parser, tree->right, browser);
	}
}

void	nm_print_header_parser(t_header_parser *parser,
			t_browser *browser, int len, int force)
{
	if (browser->nb_args > 1 || len > 1 || force)
		print_parser_header_intro(parser);
	print_symbol_tree(parser, parser->symbols, browser);
}

void	process_nm_print_tree(t_browser *browser, t_tree *tree, int len)
{
	t_header_parser *parser;

	if (tree)
	{
		process_nm_print_tree(browser, tree->left, len);
		parser = (t_header_parser *)tree->content;
		nm_print_header_parser(parser, browser, len, browser->force);
		process_nm_print_tree(browser, tree->right, len);
	}
}

void	nm_print(t_browser *browser, int force)
{
	int				len;

	len = ft_treelen(browser->parsers);
	if (force)
		browser->force = 1;
	process_nm_print_tree(browser, browser->parsers, len);
}
