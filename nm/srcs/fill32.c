/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fill32.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ldedier <ldedier@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/06/21 01:29:11 by ldedier           #+#    #+#             */
/*   Updated: 2019/06/21 01:29:11 by ldedier          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_nm.h"

int		process_fill_symbols32(t_nm_browser *browser, int nsyms,
			int symoff, int stroff)
{
	int				i;
	char			*stringtable;
	struct nlist	*array;
	t_symbol		*new_symbol;

	array = (void *)(browser->ptr + symoff);
	stringtable = (void *)(browser->ptr + stroff);
	i = 0;
	while (i < nsyms)
	{
		if (!(new_symbol = nm_new_symbol32(&array[i],
			stringtable + array[i].n_un.n_strx)))
			return (1);
		if (add_symbol_to_browser(browser, new_symbol))
		{
			free(new_symbol);
			return (1);
		}
		i++;
	}
	return (0);
}

int	fill_browser32(t_nm_browser *browser)
{
	struct mach_header *header;
	struct load_command *lc;
	struct symtab_command *sym;
	uint64_t i;

	header = browser->header_union.header32;
	i = (uint64_t)(browser->ptr + sizeof(*header));
	while (i < (uint64_t)(browser->ptr + header->sizeofcmds))
	{
		lc = (struct load_command*) i;
	
		if (lc->cmd == LC_SYMTAB)
		{
			sym = (struct symtab_command *)lc;
			if (process_fill_symbols32(browser, sym->nsyms,
				sym->symoff, sym->stroff))
				return (1);
		}
		i += lc->cmdsize;
	}
	return (0);
}
