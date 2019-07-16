/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fill_fat32.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ldedier <ldedier@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/06/22 19:16:02 by ldedier           #+#    #+#             */
/*   Updated: 2019/07/16 07:57:34 by ldedier          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_nm.h"

char	*get_cpu_name(cpu_type_t cpu, cpu_subtype_t sub)
{
	if (cpu == CPU_TYPE_I386)
		return ("i386");
	else if (cpu == CPU_TYPE_X86_64)
		return ("x86_64");
	else if (cpu == CPU_TYPE_ARM)
		return ("arm");
	else if (cpu == CPU_TYPE_ARM64)
		return ("arm_64");
	else if (cpu == CPU_TYPE_POWERPC && (sub == 0))
		return ("ppc");
	else
		return ("");
}

int		process_browser_fat_arch32(struct fat_arch *fat_arch,
			t_header_parser *parser, t_nm_browser *browser)
{
	t_header_parser new_parser;
	init_parser(&new_parser, (void *)browser->ptr + fat_arch->offset,
		fat_arch->offset, parser->filename);
	new_parser.cputype = fat_arch->cputype;
	new_parser.cpusubtype = fat_arch->cpusubtype;
	if (fill_browser(&new_parser, browser))
		return (1);
	return (0);
}
int			fat_corrupted_print_error_alignment(struct fat_arch *fat_arch,
				t_nm_browser *browser)
{
	ft_dprintf(2, "%s: %s truncated or malformed "
		"fat file (offset: %d for cputype (%d) cpusubtype (%d) "
			"not aligned on its alignment (2^%d))\n\n", browser->progname,
				browser->filename, fat_arch->offset, fat_arch->cputype,
					fat_arch->cpusubtype, fat_arch->align);
	return (CORRUPTED);
}


int		fat_corrupted_print_error(char *str, struct fat_arch *fat_arch,
			t_nm_browser *browser)
{
	ft_dprintf(2, str, browser->progname, browser->filename,
		fat_arch->cputype, fat_arch->cpusubtype);
	return (CORRUPTED);
}

int		check_all_architectures(struct fat_arch **found,
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
			return (1);
		}
		swap_fat_arch(&fat_arch_array[i], parser->should_swap);
		if (fat_arch_array[i].offset % (1 << fat_arch_array[i].align))
		{
			return (fat_corrupted_print_error_alignment(&fat_arch_array[i],
				browser));
		}
		if (is_corrupted_offset(parser->offset + fat_arch_array[i].offset,
			fat_arch_array[i].size, browser))
		{
			return (fat_corrupted_print_error("%s: %s truncated or malformed "
			"fat file (offset plus size of cputype (%d) cpusubtype (%d) extends"
				"past the end of the file)\n\n", &fat_arch_array[i], browser));
			return (1);
		}
		if (!*found && (!ft_strcmp(ARCH, get_cpu_name(fat_arch_array[i].cputype, fat_arch_array[i].cpusubtype))))
			*found = &fat_arch_array[i];
		i++;
	}
	return (0);
}

int		fill_browser_fat32(t_header_parser *parser, t_nm_browser *browser)
{
	uint32_t			i;
	struct fat_arch		*fat_arch;
	struct fat_header	*fat_header;
	struct fat_arch		*found;

	fat_header = parser->header_union.fat_header;
	fat_arch = (void *)browser->ptr + sizeof(fat_header);
	found = NULL;
	if (check_all_architectures(&found, fat_arch, parser, browser))
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
