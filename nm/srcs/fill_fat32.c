/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fill_fat32.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ldedier <ldedier@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/06/22 19:16:02 by ldedier           #+#    #+#             */
/*   Updated: 2019/06/24 19:19:08 by ldedier          ###   ########.fr       */
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

int		check_all_architectures(struct fat_arch *found,
			struct fat_arch *fat_arch_array, t_header_parser *parser,
				t_nm_browser *browser)
{
	uint32_t			i;
	struct fat_header	*fat_header;

	fat_header = parser->header_union.fat_header;	
	i = 0;
	while (i < fat_header->nfat_arch)
	{
		if (is_corrupted_data(&fat_arch_array[i],
			sizeof(struct fat_arch), browser))
		{
			return (0);
		}
		swap_fat_arch(&fat_arch_array[i], parser->should_swap);
		if (is_corrupted_offset(parser->offset + fat_arch_array[i].offset,
			fat_arch_array[i].size, browser))
		{
			ft_dprintf(2, "%s: %s truncated or malformed fat file (offset plus size"
				" of cputype (%d) cpusubtype (%d) extends past the end of the f"
					"ile)\n", browser->progname, browser->filename,
						fat_arch_array[i].cputype,
							fat_arch_array[i].cpusubtype);
			return (0);	
		}
		if (!(ft_strcmp(ARCH, get_cpu_name(fat_arch_array[i].cputype))))
			found = &fat_arch_array[i];
		i++;
	}
	return (0);
}

/*
struct fat_arch	*get_fat_arch_from_fat_header32(struct fat_header *fat_header,
			struct fat_arch *fat_arch_array, t_header_parser *parser,
				t_nm_browser *browser)
{
	uint32_t			i;

	i = 0;
	while (i < fat_header->nfat_arch)
	{
		if (is_corrupted_data(&fat_arch_array[i],
			sizeof(struct fat_arch), browser))
		{
			ft_printf("didier\n");
			return (0);
		}
		swap_fat_arch(&fat_arch_array[i], parser->should_swap);
		
		ft_printf("%lld + %lld = %lld\n", parser->offset, fat_arch_array[i].offset, parser->offset + fat_arch_array[i].offset);
		if (is_corrupted_offset(parser->offset + fat_arch_array[i].offset,
			fat_arch_array[i].size, browser))
		{
			ft_dprintf(2, "%s: %s truncated or malformed fat file (offset plus size"
				" of cputype (%d) cpusubtype (%d) extends past the end of the f"
					"ile)\n", browser->progname, browser->filename,
						fat_arch_array[i].cputype,
							fat_arch_array[i].cpusubtype);
			return (0);	
		}
		if (!(ft_strcmp(ARCH, get_cpu_name(fat_arch_array[i].cputype))))
			return (&fat_arch_array[i]);
		i++;
	}
	return (NULL);
}
*/

int		fill_browser_fat32(t_header_parser *parser, t_nm_browser *browser)
{
	uint32_t			i;
	struct fat_arch		*fat_arch;
	struct fat_header	*fat_header;
	struct fat_arch		*found;

	fat_header = parser->header_union.fat_header;
	fat_arch = (void *)browser->ptr + sizeof(fat_header);
	found = NULL;
	if (check_all_architectures(found, fat_arch, parser, browser))
		return (CORRUPTED);
	if (found)
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
