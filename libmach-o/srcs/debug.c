/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   debug.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: niragne <niragne@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/06/19 13:59:56 by niragne           #+#    #+#             */
/*   Updated: 2019/08/17 14:18:58 by niragne          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "mach_o.h"

static void		print64(t_header_parser *parser)
{
	struct mach_header_64	*ptr;

	ptr = (struct mach_header_64*)parser->header_union.header64;
	swap_uint32(&(ptr->magic), parser->should_swap);
	ft_printf("      magic cputype cpusubtype  caps    filetype ncmds "\
	"sizeofcmds      flags\n");
	ft_printf("%#11x %8u %10hu  %#04p %11d %5d %10d %#010x\n", ptr->magic,
	ptr->cputype, ptr->cpusubtype, (ptr->cpusubtype & 0xff000000) >> 24,
	ptr->filetype, ptr->ncmds, ptr->sizeofcmds, ptr->flags);
}

static void		print32(t_header_parser *parser)
{
	struct mach_header_64	*ptr;

	ptr = (struct mach_header_64*)parser->header_union.header32;
	swap_uint32(&(ptr->magic), parser->should_swap);
	ft_printf("      magic cputype cpusubtype  caps    filetype ncmds "\
	"sizeofcmds      flags\n");
	ft_printf("%#11x %7u %10hu  %#04p %11d %5d %10d %#010x\n", ptr->magic,
	ptr->cputype, ptr->cpusubtype, (ptr->cpusubtype & 0xff000000) >> 24,
	ptr->filetype, ptr->ncmds, ptr->sizeofcmds, ptr->flags);
}

void			nm_print_header(t_header_parser *parser)
{
	ft_printf("Mach header\n");
	if (parser->type == E_64)
	{
		print64(parser);
	}
	else if (parser->type == E_32)
	{
		print32(parser);
	}
}

void			print_cpu(struct fat_arch *fat_arch)
{
	ft_printf("%s\n", get_cpu_name(fat_arch->cputype, fat_arch->cpusubtype));
}

void			print_arch32(struct fat_arch *fat_arch)
{
	ft_printf("fat_arch offset: %u\n", fat_arch->offset);
	ft_printf("fat_arch size: %u\n", fat_arch->size);
	ft_printf("fat_arch align: %u\n", fat_arch->align);
	ft_printf("fat_arch cpu: ");
	print_cpu(fat_arch);
	ft_printf("\n");
	ft_printf("\n");
}
