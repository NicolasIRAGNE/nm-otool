/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fill32.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ldedier <ldedier@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/08/15 21:05:50 by ldedier           #+#    #+#             */
/*   Updated: 2019/08/16 15:12:10 by ldedier          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "mach_o.h"

/*
** store sections by index of apparition and symbol tables sorted
** in a tree structure (stocks the debug Character (T, U, ...) by peeking
** at the matching section (mainly)
*/

int		fill_browser32(t_header_parser *parser, t_browser *browser)
{
	int ret;

	if ((ret = get_sections32(parser, browser)))
		return (ret);
	if ((ret = fill_symbol_table32(parser, browser)))
	{
		if (ret == ERROR_CORRUPTED)
		{
			if (add_parser(browser, parser))
			{
				free_parser(parser);
				return (1);
			}
		}
		else
			free_parser(parser);
		return (ret);
	}
	if (add_parser(browser, parser))
	{
		free_parser(parser);
		return (1);
	}
	return (0);
}
