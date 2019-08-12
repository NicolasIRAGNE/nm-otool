/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: niragne <niragne@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/06/21 02:14:47 by ldedier           #+#    #+#             */
/*   Updated: 2019/08/12 15:30:05 by ldedier          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "mach_o.h"

void	init_parser(t_header_parser *parser, void *ptr,
			uint64_t offset, char *filename)
{
	parser->ptr = ptr;
	parser->offset = offset;
	parser->symbols = NULL;
	parser->section_arr.sections = NULL;
	parser->text_section = NULL;
	parser->data_section = NULL;
	parser->filename = filename;
	parser->parser_enum = PARSER_ENUM_NONE;
	parser->should_swap = 0;
	parser->parser_union.arch.cputype = -1;
	parser->parser_union.arch.relevant = 0;
}

int	init_browser(t_browser *browser, char *filename)
{
	int			fd;

	browser->force = 0;
	browser->filename = filename;
	browser->last_member_name = NULL;
	browser->parsers = NULL;
	browser->from = -1;
	if ((fd = open(filename, O_RDONLY)) < 0)
	{
		ft_dprintf(2, "error opening '%s'\n", filename);
		return (1);
	}
	if ((fstat(fd, &browser->st)) < 0)
	{
		ft_dprintf(2, "error fstat on file '%s'\n", filename);
		return (1);
	}
	if ((browser->ptr = mmap(0, browser->st.st_size,
		PROT_READ | PROT_WRITE, MAP_PRIVATE, fd, 0)) == MAP_FAILED)
	{
		ft_dprintf(2, "could not map file '%s'\n", filename);
		return (1);
	}
	return (0);
}

void	init_browser_general(t_browser *browser, char *progname)
{
	browser->ret = 0;
	browser->has_bad_index = 0;
	browser->sort_func = cmp_symbol_alpha;
	browser->sort_mult = 1;
	browser->progname = progname;
}
