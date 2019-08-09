/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ldedier <ldedier@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/06/24 01:32:45 by ldedier           #+#    #+#             */
/*   Updated: 2019/07/17 16:26:45 by ldedier          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "mach_o.h"

void	free_symbol(t_symbol *symbol)
{
	if (symbol->symbol_enum == E_SYMBOL_64)
		free(symbol->symbol_union.symbol64.name);
	else
		free(symbol->symbol_union.symbol32.name);
	free(symbol);
}

void	free_symbol_tree(void *content)
{
	free_symbol((t_symbol *)content);
}

void	free_parser(t_header_parser *parser)
{
	ft_tree_del(&parser->symbols, &free_symbol_tree);
	if (parser->section_arr.sections)
		free(parser->section_arr.sections);
}

void	free_parser_tree(void *content)
{
	free_parser((t_header_parser *)content);
	free(content);
}

int		free_browser(t_browser *browser)
{
	if (munmap(browser->ptr, browser->st.st_size) < 0)
	{
		ft_dprintf(2, "could not munmap the file %s\n", browser->filename);
		return (1);
	}
	return (0);
}
