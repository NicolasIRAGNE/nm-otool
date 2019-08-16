/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fill.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: niragne <niragne@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/06/19 13:02:13 by niragne           #+#    #+#             */
/*   Updated: 2019/08/16 16:09:05 by ldedier          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "mach_o.h"

int		identify_as_archive(t_header_parser *parser, t_browser *browser)
{
	int identifier_len;

	identifier_len = ft_strlen(ARCHIVE_IDENTIFIER);
	return (browser->st.st_size >= (off_t)identifier_len
		&& !ft_strncmp(ARCHIVE_IDENTIFIER,
			(char *)parser->ptr, identifier_len));
}

void	get_header_others(t_header_parser *parser, t_browser *browser,
			int verbose)
{
	if (parser->magic == FAT_MAGIC || parser->magic == FAT_CIGAM)
	{
		if (verbose)
			ft_printf("Executable fat 32 bits\n");
		parser->type = E_FAT32;
		parser->header_union.fat_header = (struct fat_header*)parser->ptr;
	}
	else if (parser->magic == FAT_MAGIC_64 || parser->magic == FAT_CIGAM_64)
	{
		if (verbose)
			ft_printf("Executable fat 64 bits\n");
		parser->type = E_FAT64;
		parser->header_union.fat_header = (struct fat_header*)parser->ptr;
	}
	else if (identify_as_archive(parser, browser))
	{
		if (verbose)
			ft_printf("ARCHIVE\n");
		parser->type = E_ARCHIVE;
	}
	else
		parser->type = E_UNKNOWN;
}

void	get_header(t_header_parser *parser, t_browser *browser, int first)
{
	int verbose;

	verbose = 0;
	parser->magic = *(uint32_t*)parser->ptr;
	if (parser->magic == MH_MAGIC_64 || parser->magic == MH_CIGAM_64)
	{
		if (verbose)
			ft_printf("Executable 64 bits\n");
		parser->type = E_64;
		parser->header_union.header64 = (struct mach_header_64*)parser->ptr;
	}
	else if (parser->magic == MH_MAGIC || parser->magic == MH_CIGAM)
	{
		if (verbose)
			ft_printf("Executable 32 bits\n");
		parser->type = E_32;
		parser->header_union.header32 = (struct mach_header*)parser->ptr;
	}
	else
		get_header_others(parser, browser, verbose);
	if (first)
		browser->from = parser->type;
}

/*
** update the should_swap field and swap the header of the file if necessary
*/

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

/*
** fill the parser symbols given an address (parser->ptr) and a mapped file
** in the browser
**
** will also append the parser to the end of the t_list if it contains
** any symbols
**
** in the case of fat_headers, this function will call itself
** recursively for all the headers found in the fat_header
*/

int		fill_browser(t_header_parser *parser, t_browser *browser, int first)
{
	get_header(parser, browser, first);
	if (parser->type == E_UNKNOWN)
	{
		ft_dprintf(2, "%s: %s The file was not recognised as "\
		"a valid object file\n\n", browser->progname, browser->filename);
		return (1);
	}
	swap_header(parser);
	if (parser->type == E_64)
	{
		return (fill_browser64(parser, browser));
	}
	else if (parser->type == E_32)
	{
		return (fill_browser32(parser, browser));
	}
	else if (parser->type == E_FAT32)
		return (fill_browser_fat32(parser, browser));
	else if (parser->type == E_ARCHIVE)
		return (fill_browser_archive(parser, browser));
	return (0);
}
