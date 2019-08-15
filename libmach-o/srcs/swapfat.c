/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   swapfat.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ldedier <ldedier@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/08/15 16:18:58 by ldedier           #+#    #+#             */
/*   Updated: 2019/08/15 16:20:07 by ldedier          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "mach_o.h"

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
