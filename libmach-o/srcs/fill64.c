/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fill64.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: niragne <niragne@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/06/21 01:29:27 by ldedier           #+#    #+#             */
/*   Updated: 2019/08/16 14:36:24 by ldedier          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "mach_o.h"

/*
** store sections by index of apparition and symbol tables sorted
** in a tree structure (stocks the debug Character (T, U, ...) by peeking
** at the matching section (mainly)
*/

int		fill_browser64(t_header_parser *parser, t_browser *browser)
{
	int ret;

	if ((ret = get_sections64(parser, browser)))
		return (ret);
	if ((ret = fill_symbol_table64(parser, browser)))
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
