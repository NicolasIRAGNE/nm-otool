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

void	print_cpu(cpu_type_t cpu)
{
	if (cpu == CPU_TYPE_I386)
		ft_printf("i386");
	else if (cpu == CPU_TYPE_X86_64)
		ft_printf("x86_64");
	else if (cpu == CPU_TYPE_ARM)
		ft_printf("arm");
	else if (cpu == CPU_TYPE_ARM64)
		ft_printf("arm_64");
	else
		ft_printf("undefined");
}

void	print_arch32(struct fat_arch *fat_arch)
{
	ft_printf("fat_arch offset: %u\n", fat_arch->offset);
	ft_printf("fat_arch size: %u\n", fat_arch->size);
	ft_printf("fat_arch align: %u\n", fat_arch->align);
	ft_printf("fat_arch cpu: ");
	print_cpu(fat_arch->cputype);
	ft_printf("\n");
	ft_printf("\n");
}

int		fill_browser_fat_arch32(struct fat_arch *fat_arch,
			t_nm_browser *browser)
{
	t_header_parser new_parser;
//	print_arch32(fat_arch);
	init_parser(&new_parser, (void *)browser->ptr + fat_arch->offset,
		fat_arch->offset);
	return (fill_browser(&new_parser, browser));
}

int		fill_browser_fat32(t_header_parser *parser, t_nm_browser *browser)
{
	uint32_t			i;
	struct fat_arch		*fat_arch;
	struct fat_header	*fat_header;

	fat_header = parser->header_union.fat_header;
	fat_arch = (void *)browser->ptr + sizeof(fat_header);
	i = fat_header->nfat_arch - 1;
	while ((int)i >= 0)
	{
		swap_fat_arch(&fat_arch[i], parser->should_swap);
		if (fill_browser_fat_arch32(&fat_arch[i], browser))
			return (1);
		i--;
	}
	return (0);
}
