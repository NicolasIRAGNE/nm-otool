/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fill64_tools.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ldedier <ldedier@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/08/16 14:35:52 by ldedier           #+#    #+#             */
/*   Updated: 2019/08/16 14:36:02 by ldedier          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "mach_o.h"

int		process_process_fill_symbol64(struct nlist_64 *nlist,
			char *stringtable, t_header_parser *parser, t_browser *browser)
{
	t_symbol				*new_symbol;

	if (should_add_symbol(nlist->n_type, nlist->n_desc,
		stringtable + nlist->n_un.n_strx, browser))
	{
		if (!(new_symbol = nm_new_symbol64(nlist,
			stringtable + nlist->n_un.n_strx,
				nlist->n_un.n_strx, browser)))
			return (1);
		if (fill_debug64(new_symbol, parser->section_arr, browser))
		{
			free_symbol(new_symbol);
			return (2);
		}
		if (add_symbol_to_browser(parser, browser, new_symbol))
		{
			free_symbol(new_symbol);
			return (1);
		}
	}
	return (0);
}

int		process_process_fill_symbols64(struct symtab_command *sym,
		struct nlist_64 *array, t_header_parser *parser, t_browser *browser)
{
	uint32_t				i;
	char					*stringtable;
	int						ret;

	stringtable = (void *)((void *)parser->header_union.header64 + sym->stroff);
	i = 0;
	while (i < sym->nsyms)
	{
		swap_nlist64(&array[i], parser->should_swap);
		if ((ret = process_process_fill_symbol64(&array[i],
			stringtable, parser, browser)))
			return (ret == 1);
		i++;
	}
	return (0);
}

/*
** create and append a new symbol to the symbol tree as it is sorted by the
** appropriate function
**
** also fill its debug character by peeking at its corresponding section
*/

int		process_fill_symbols64(t_header_parser *parser, t_browser *browser,
			struct symtab_command *sym)
{
	struct nlist_64			*array;
	struct mach_header_64	*header;
	int						ret;

	header = parser->header_union.header64;
	array = (void *)((void *)header + sym->symoff);
	browser->strsize = sym->strsize;
	if ((ret = is_corrupted_fill_symbol64(sym, header, browser)))
		return (ret);
	return (process_process_fill_symbols64(sym, array, parser, browser));
}

int		fill_symbol_table64(t_header_parser *parser, t_browser *browser)
{
	struct load_command		*lc;
	struct symtab_command	*sym;
	uint64_t				i;
	struct mach_header_64	*header;
	int						ret;

	header = parser->header_union.header64;
	i = (uint64_t)((void *)header + sizeof(*header));
	while (i < (uint64_t)((void *)header + header->sizeofcmds))
	{
		lc = (struct load_command*)i;
		if (lc->cmdsize == 0)
			return (macho_perror(ERROR_CORRUPTED, browser));
		if (lc->cmd == LC_SYMTAB)
		{
			sym = (struct symtab_command *)lc;
			swap_symtab_command(sym, parser->should_swap);
			if ((ret = process_fill_symbols64(parser, browser, sym)))
				return (ret);
		}
		i += lc->cmdsize;
	}
	return (0);
}

int		get_total_sections64(t_list *segments64)
{
	t_list						*ptr;
	struct segment_command_64	*seg;
	int							total;

	total = 0;
	ptr = segments64;
	while (ptr != NULL)
	{
		seg = (struct segment_command_64 *)ptr->content;
		total += seg->nsects;
		ptr = ptr->next;
	}
	return (total);
}
