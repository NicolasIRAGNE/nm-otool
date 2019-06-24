/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: niragne <niragne@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/06/19 13:02:13 by niragne           #+#    #+#             */
/*   Updated: 2019/06/20 19:45:42 by ldedier          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_nm.h"

void	get_header(t_header_parser *parser)
{
	parser->magic = *(uint32_t*)parser->ptr;
	if (parser->magic == MH_MAGIC_64 || parser->magic == MH_CIGAM_64)
	{
//		ft_printf("Executable 64 bits\n");
		parser->type = E_64;
		parser->header_union.header64 = (struct mach_header_64*)parser->ptr;
	}
	else if (parser->magic == MH_MAGIC || parser->magic == MH_CIGAM)
	{
//		ft_printf("Executable 32 bits\n");
		parser->type = E_32;
		parser->header_union.header32 = (struct mach_header*)parser->ptr;
	}
	else if (parser->magic == FAT_MAGIC || parser->magic == FAT_CIGAM)
	{
//		ft_printf("Executable fat 32 bits\n");
		parser->type = E_FAT32;
		parser->header_union.fat_header = (struct fat_header*)parser->ptr;
	}
	else if (parser->magic == FAT_MAGIC_64 || parser->magic == FAT_CIGAM_64)
	{
//		ft_printf("Executable fat 64 bits\n");
		parser->type = E_FAT64;
		parser->header_union.fat_header = (struct fat_header*)parser->ptr;
	}
	else
		parser->type = E_UNKNOWN;
}

void	swap_header(t_header_parser *parser)
{
	if (parser->magic == MH_CIGAM_64 || parser->magic == MH_CIGAM
		|| parser->magic == FAT_CIGAM_64 || parser->magic == FAT_CIGAM)
	{
		parser->should_swap = 1;
		if (parser->magic == MH_CIGAM_64)
			swap_mach_header64(parser->header_union.header64);
		else if (parser->magic == MH_CIGAM)
			swap_mach_header(parser->header_union.header32, 1);
		else
			swap_fat_header(parser->header_union.fat_header);
	}
	else
		parser->should_swap = 0;
}

int	fill_browser(t_header_parser *parser, t_nm_browser *browser)
{
	get_header(parser);
	if (parser->type == E_UNKNOWN)
		return (nm_perror("unknown magic file", parser));
	swap_header(parser);
	if (parser->type == E_64)
		return (fill_browser64(parser, browser));
	else if (parser->type == E_32)
		return (fill_browser32(parser, browser));
	else if (parser->type == E_FAT32)
		return (fill_browser_fat32(parser, browser));
//	else
//		return (fill_parser_fat64(parser, browser));
	return (0);
}
