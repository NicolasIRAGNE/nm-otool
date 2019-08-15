/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   swap32.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ldedier <ldedier@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/08/15 16:15:03 by ldedier           #+#    #+#             */
/*   Updated: 2019/08/15 16:19:09 by ldedier          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "mach_o.h"

void	swap_uint32(uint32_t *value, int should_swap)
{
	if (should_swap)
		*value = ((((*value) & 0xff000000) >> 24)
			| (((*value) & 0xff0000) >> 8) | (((*value) & 0xff00) << 8)
				| (((*value) & 0xff) << 24));
}

void	swap_mach_header(struct mach_header *header32, int should_swap)
{
	swap_bytes(&header32->cputype, sizeof(header32->cputype), should_swap);
	swap_bytes(&header32->cpusubtype,
		sizeof(header32->cpusubtype), should_swap);
	swap_uint32(&header32->filetype, should_swap);
	swap_uint32(&header32->ncmds, should_swap);
	swap_uint32(&header32->sizeofcmds, should_swap);
	swap_uint32(&header32->flags, should_swap);
}

void	swap_nlist(struct nlist *nlist, int should_swap)
{
	swap_uint32(&nlist->n_value, should_swap);
	swap_uint32(&nlist->n_un.n_strx, should_swap);
	swap_bytes(&nlist->n_desc, sizeof(int16_t), should_swap);
}

void	swap_section(struct section *section, int should_swap)
{
	swap_uint32(&section->addr, should_swap);
	swap_uint32(&section->size, should_swap);
	swap_uint32(&section->offset, should_swap);
	swap_uint32(&section->align, should_swap);
	swap_uint32(&section->reloff, should_swap);
	swap_uint32(&section->reloff, should_swap);
	swap_uint32(&section->nreloc, should_swap);
	swap_uint32(&section->flags, should_swap);
	swap_uint32(&section->reserved1, should_swap);
	swap_uint32(&section->reserved2, should_swap);
}
