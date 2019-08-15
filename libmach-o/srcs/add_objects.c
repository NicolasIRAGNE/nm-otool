/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fill_tools.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: niragne <niragne@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/06/21 01:35:21 by ldedier           #+#    #+#             */
/*   Updated: 2019/08/15 16:58:31 by ldedier          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "mach_o.h"

int			add_symbol_to_browser(t_header_parser *parser,
		t_browser *browser, t_symbol *new_symbol)
{
	return (ft_tree_add_sorted(&parser->symbols, new_symbol,
				browser, browser->sort_func));
}

int			should_add_symbol(uint8_t n_type, uint16_t n_desc, char *name,
		t_browser *browser)
{
	(void)browser;
	(void)name;
	(void)n_desc;
	if (n_type & N_STAB)
		return (0);
	return (1);
}

int			add_parser(t_browser *browser, t_header_parser *parser)
{
	int ret;

	if (parser->parser_enum == PARSER_ENUM_OBJECT)
	{
		if ((ret = (ft_tree_add_sorted_value_no_doubles(&browser->parsers,
			parser, sizeof(t_header_parser), cmp_parser_ran_off))) == 1)
			return (1);
		else if (ret == 2)
			free_parser(parser);
		return (0);
	}
	else
	{
		return (ft_add_to_tree_back(&browser->parsers, parser,
			sizeof(t_header_parser)));
	}
}
