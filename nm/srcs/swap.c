/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   swap.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ldedier <ldedier@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/06/22 21:07:18 by ldedier           #+#    #+#             */
/*   Updated: 2019/06/22 21:07:18 by ldedier          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_nm.h"

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

void	swap_uint32(uint32_t *value, int should_swap)
{
	if (should_swap)
		*value = ((((*value) & 0xff000000) >> 24)
			| (((*value) & 0xff0000) >> 8) | (((*value) & 0xff00) << 8)
				| (((*value) & 0xff) << 24));
}

void	swap_mach_header64(struct mach_header_64 *header64)
{
	(void)header64;
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

void	swap_fat_arch(struct fat_arch *fat_arch, int should_swap)
{
	swap_bytes(&fat_arch->cputype, sizeof(fat_arch->cputype), should_swap);
	swap_bytes(&fat_arch->cpusubtype,
		sizeof(fat_arch->cpusubtype), should_swap);
	swap_uint32(&fat_arch->offset, should_swap);
	swap_uint32(&fat_arch->size, should_swap);
	swap_uint32(&fat_arch->align, should_swap);
}
