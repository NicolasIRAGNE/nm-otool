/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fill_fat32.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ldedier <ldedier@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/06/22 19:16:02 by ldedier           #+#    #+#             */
/*   Updated: 2019/06/22 19:16:02 by ldedier          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_nm.h"

char	*get_cpu_name(cpu_type_t cpu)
{
	if (cpu == CPU_TYPE_I386)
		return ("i386");
	else if (cpu == CPU_TYPE_X86_64)
		return ("x86_64");
	else if (cpu == CPU_TYPE_ARM)
		return ("arm");
	else if (cpu == CPU_TYPE_ARM64)
		return ("arm_64");
	else if (cpu == CPU_TYPE_POWERPC)
		return ("ppc");
	else
		return ("undefined");
}

int		process_browser_fat_arch32(struct fat_arch *fat_arch,
			t_header_parser *parser, t_nm_browser *browser)
{
	t_header_parser new_parser;
	init_parser(&new_parser, (void *)browser->ptr + fat_arch->offset,
		fat_arch->offset, parser->filename);
	new_parser.cputype = fat_arch->cputype;
	if (fill_browser(&new_parser, browser))
		return (1);
	return (0);
}

struct fat_arch	*get_fat_arch_from_fat_header32(struct fat_header *fat_header,
			struct fat_arch *fat_arch_array, int should_swap)
{
	uint32_t			i;

	i = 0;
	while (i < fat_header->nfat_arch)
	{
		swap_fat_arch(&fat_arch_array[i], should_swap);
		if (!(ft_strcmp(ARCH, get_cpu_name(fat_arch_array[i].cputype))))
			return (&fat_arch_array[i]);
		i++;
	}
	return (NULL);
}

int		fill_browser_fat32(t_header_parser *parser, t_nm_browser *browser)
{
	uint32_t			i;
	struct fat_arch		*fat_arch;
	struct fat_header	*fat_header;
	struct fat_arch		*found;

	fat_header = parser->header_union.fat_header;
	fat_arch = (void *)browser->ptr + sizeof(fat_header);
	if ((found = get_fat_arch_from_fat_header32(fat_header, fat_arch, parser->should_swap)))
		return (process_browser_fat_arch32(found, parser, browser));
	i = 0;
	while (i < fat_header->nfat_arch)
	{
		if (process_browser_fat_arch32(&fat_arch[i], parser, browser))
			return (1);
		i++;
	}
	return (0);
}
