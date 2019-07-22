/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   swap.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ldedier <ldedier@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/06/22 21:07:18 by ldedier           #+#    #+#             */
/*   Updated: 2019/07/22 16:52:37 by ldedier          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "mach_o.h"

void	swap_bytes(void *to_swap, size_t size, int should_swap)
{
	size_t			i;
	char			tmp;
	unsigned char	*array;

	if (should_swap)
	{
		array = (unsigned char *)to_swap;
		i = 0;
		while (i < size / 2)
		{
			tmp = array[i];
			array[i] = array[size - i - 1];
			array[size - i - 1] = tmp;
			i++;
		}
	}
}

void	swap_uint16(uint16_t *value, int should_swap)
{
	if (should_swap)
		*value =  (((*(value) & 0xff) << 8) | (((*value) & 0xff00) >> 8));
}

void	swap_uint32(uint32_t *value, int should_swap)
{
	if (should_swap)
		*value = ((((*value) & 0xff000000) >> 24)
			| (((*value) & 0xff0000) >> 8) | (((*value) & 0xff00) << 8)
				| (((*value) & 0xff) << 24));
}

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

void	swap_fat_header(struct fat_header *fat_header)
{
	swap_uint32(&fat_header->nfat_arch, 1);
}

void	swap_load_command(struct load_command *load_command, int should_swap)
{
	swap_uint32(&load_command->cmd, should_swap);
	swap_uint32(&load_command->cmdsize, should_swap);
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

void	swap_segment_command(struct segment_command *seg, int should_swap)
{
	swap_bytes(&seg->segname, sizeof(seg->segname), should_swap);
	swap_uint32(&seg->vmaddr, should_swap);
	swap_uint32(&seg->vmsize, should_swap);
	swap_uint32(&seg->fileoff, should_swap);
	swap_uint32(&seg->filesize, should_swap);
	swap_bytes(&seg->maxprot, sizeof(vm_prot_t), should_swap);
	swap_bytes(&seg->initprot, sizeof(vm_prot_t), should_swap);
	swap_uint32(&seg->nsects, should_swap);
	swap_uint32(&seg->flags, should_swap);
}

void	swap_symtab_command(struct symtab_command *sym, int should_swap)
{
	swap_uint32(&sym->stroff, should_swap);
	swap_uint32(&sym->nsyms, should_swap);
	swap_uint32(&sym->symoff, should_swap);
	swap_uint32(&sym->strsize, should_swap);
}

void	swap_fat_arch(struct fat_arch *fat_arch, int should_swap)
{
	swap_bytes(&fat_arch->cputype, sizeof(fat_arch->cputype), should_swap);
	swap_bytes(&fat_arch->cpusubtype,
		sizeof(fat_arch->cpusubtype), should_swap);
	swap_uint32(&fat_arch->offset, should_swap);
	swap_uint32(&fat_arch->size, should_swap);
	swap_uint32(&fat_arch->align, should_swap);
}

void	swap_nlist(struct nlist *nlist, int should_swap)
{
	swap_uint32(&nlist->n_value, should_swap);
	swap_uint32(&nlist->n_un.n_strx, should_swap);
	swap_bytes(&nlist->n_desc, sizeof(int16_t), should_swap);
}

void	swap_section(struct section *section , int should_swap)
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

void	swap_section_64(struct section_64 *section , int should_swap)
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
