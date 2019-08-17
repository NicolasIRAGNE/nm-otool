/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   swap.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: niragne <niragne@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/06/22 21:07:18 by ldedier           #+#    #+#             */
/*   Updated: 2019/08/17 12:40:40 by niragne          ###   ########.fr       */
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
		*value = (((*(value) & 0xff) << 8) | (((*value) & 0xff00) >> 8));
}

void	swap_load_command(struct load_command *load_command, int should_swap)
{
	swap_uint32(&load_command->cmd, should_swap);
	swap_uint32(&load_command->cmdsize, should_swap);
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
