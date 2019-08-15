/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   swap64.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ldedier <ldedier@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/08/15 16:13:00 by ldedier           #+#    #+#             */
/*   Updated: 2019/08/15 16:13:02 by ldedier          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "mach_o.h"

void	swap_uint64(uint64_t *value, int should_swap)
{
	swap_bytes(value, sizeof(*value), should_swap);
}

void	swap_mach_header64(struct mach_header_64 *header64)
{
	swap_bytes(&header64->cputype, sizeof(header64->cputype), 1);
	swap_bytes(&header64->cpusubtype,
			sizeof(header64->cpusubtype), 1);
	swap_uint32(&header64->filetype, 1);
	swap_uint32(&header64->ncmds, 1);
	swap_uint32(&header64->sizeofcmds, 1);
	swap_uint32(&header64->flags, 1);
	swap_uint32(&header64->reserved, 1);
}

void	swap_segment_command_64(struct segment_command_64 *seg, int should_swap)
{
	swap_bytes(&seg->segname, sizeof(seg->segname), should_swap);
	swap_uint64(&seg->vmaddr, should_swap);
	swap_uint64(&seg->vmsize, should_swap);
	swap_uint64(&seg->fileoff, should_swap);
	swap_uint64(&seg->filesize, should_swap);
	swap_bytes(&seg->maxprot, sizeof(vm_prot_t), should_swap);
	swap_bytes(&seg->initprot, sizeof(vm_prot_t), should_swap);
	swap_uint32(&seg->nsects, should_swap);
	swap_uint32(&seg->flags, should_swap);
}

void	swap_section_64(struct section_64 *section, int should_swap)
{
	swap_uint64(&section->addr, should_swap);
	swap_uint64(&section->size, should_swap);
	swap_uint32(&section->offset, should_swap);
	swap_uint32(&section->align, should_swap);
	swap_uint32(&section->reloff, should_swap);
	swap_uint32(&section->reloff, should_swap);
	swap_uint32(&section->nreloc, should_swap);
	swap_uint32(&section->flags, should_swap);
	swap_uint32(&section->reserved1, should_swap);
	swap_uint32(&section->reserved2, should_swap);
}

void	swap_nlist64(struct nlist_64 *nlist, int should_swap)
{
	swap_uint32(&nlist->n_un.n_strx, should_swap);
	swap_bytes(&nlist->n_desc, sizeof(int16_t), should_swap);
	swap_uint64(&nlist->n_value, should_swap);
}
