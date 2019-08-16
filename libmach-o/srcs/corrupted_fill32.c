/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fill32.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ldedier <ldedier@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/08/15 21:05:50 by ldedier           #+#    #+#             */
/*   Updated: 2019/08/16 15:04:09 by ldedier          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "mach_o.h"

int		overlaps_symtab_command32(struct symtab_command *sym)
{
	uint32_t symbol_table_size;

	symbol_table_size = sym->nsyms * sizeof(struct nlist);
	if ((sym->symoff + symbol_table_size > sym->stroff
		&& sym->symoff < sym->stroff)
			|| (sym->stroff + sym->strsize > symbol_table_size
				&& sym->stroff < sym->symoff))
		return (1);
	else
		return (0);
}

int		is_corrupted_fill_symbol32(struct symtab_command *sym,
			struct mach_header *header, t_browser *browser)
{
	char	*stringtable;

	stringtable = (char *)((void *)header + sym->stroff);
	if (is_corrupted_data(stringtable, sym->strsize, browser))
	{
		ft_dprintf(2, "%s: %s: stringtable size (%u) exceeds end of file\n",
			browser->progname, browser->filename, sym->strsize);
		return (ERROR_CORRUPTED);
	}
	if (overlaps_symtab_command32(sym))
	{
		ft_dprintf(2, "%s: %s truncated or malformed object (string table at "
			"offset %d with a size of %d, overlaps symbol table at offset %d "
				"with a size of %d)\n\n", browser->progname, browser->filename,
					sym->stroff, sym->strsize, sym->symoff,
						sym->nsyms * sizeof(struct nlist));
		return (1);
	}
	return (0);
}

int		corrupted_load_command32(struct load_command *lc, t_browser *browser,
			struct mach_header *header, int index)
{
	if (lc->cmdsize == 0)
	{
		ft_dprintf(2, "%s: %s: truncated or malformed object (load commands"
				" extend past the end of the file)\n", browser->progname,
				browser->filename);
		return (1);
	}
	if (is_corrupted_data(lc, lc->cmdsize, browser)
			|| (void *)lc + max_uint32(lc->cmdsize, 1) > (void *)header
			+ sizeof(*header) + header->sizeofcmds)
	{
		ft_dprintf(2, "%s: %s truncated or malformed object "
				"(load command %d extends past the end of all"\
				"load commands in the file)\n\n",
				browser->progname, browser->filename, index);
		return (1);
	}
	if (lc->cmdsize % 4)
	{
		ft_dprintf(2, "%s: %s truncated or malformed object "
				"(load command %d fileoff not a multiple of 4)\n\n",
				browser->progname, browser->filename, index);
		return (1);
	}
	return (0);
}

int		is_corrupted_segment_command_32(struct segment_command *seg,
		t_header_parser *parser, t_browser *browser, int index)
{
	if (is_corrupted_data(seg, sizeof(struct segment_command),
				browser))
		return (1);
	swap_segment_command(seg, parser->should_swap);
	if (seg->cmdsize != sizeof(*seg) + sizeof(struct section)
			* seg->nsects)
	{
		ft_dprintf(2, "%s: %s truncated or malformed object "
				"(load command %d inconsistent cmdsize in LC_SEGMENT "
				"for the number of sections)\n\n",
				browser->progname, browser->filename, index);
		return (1);
	}
	if (is_corrupted_offset(parser->offset + seg->fileoff,
				seg->filesize, browser))
	{
		ft_dprintf(2, "%s: %s truncated or malformed object "
				"(load command %d fileoff filed plus filesize "
				"field in LC_SEGMENT extends past the end of the "
				"file)\n\n", browser->progname, browser->filename, index);
		return (1);
	}
	return (0);
}
