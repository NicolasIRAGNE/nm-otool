/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fill_fat32.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: niragne <niragne@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/06/22 19:16:02 by ldedier           #+#    #+#             */
/*   Updated: 2019/08/12 15:02:31 by ldedier          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "mach_o.h"

char	*get_cpu_name(cpu_type_t cpu, cpu_subtype_t sub)
{
	ft_printf("");
	if (cpu == CPU_TYPE_I386)
		return ("i386");
	else if (cpu == CPU_TYPE_X86_64)
		return ("x86_64");
	else if (cpu == CPU_TYPE_ARM)
	{
		if (sub == CPU_SUBTYPE_ARM_V6)
			return ("armv6");
		else if (sub == CPU_SUBTYPE_ARM_V7)
			return ("armv7");
		else if (sub == CPU_SUBTYPE_ARM_V7S)
			return ("armv7s");
		else if (sub == CPU_SUBTYPE_ARM_V8)
			return ("armv8");
		else 
			return ("arm");
	}
	else if (cpu == CPU_TYPE_ARM64)
		return ("arm64");
	else if (cpu == CPU_TYPE_POWERPC && (sub == 0))
		return ("ppc");
	else
		return ("");
}

int		process_browser_fat_arch32(struct fat_arch *fat_arch,
			t_header_parser *parser, t_browser *browser)
{
	t_header_parser	new_parser;
	int				ret;

	init_parser(&new_parser, (void *)browser->ptr + fat_arch->offset,
		fat_arch->offset, parser->filename);
	new_parser.parser_enum = PARSER_ENUM_ARCHI;
	new_parser.parser_union.arch.cputype = fat_arch->cputype;
	new_parser.parser_union.arch.cpusubtype = fat_arch->cpusubtype;
	new_parser.parser_union.arch.relevant = 1;
	if ((ret = fill_browser(&new_parser, browser, 0)) == 0)
		return (0);
	else if ((ret == CORRUPTED && browser->bin == E_BIN_OTOOL) || ret == 1)
	{
		return (ret);
	}
	return (0);
}
int			fat_corrupted_print_error_alignment(struct fat_arch *fat_arch,
				t_browser *browser)
{
	ft_dprintf(2, "%s: %s truncated or malformed "
		"fat file (offset: %d for cputype (%d) cpusubtype (%d) "
			"not aligned on its alignment (2^%d))\n\n", browser->progname,
				browser->filename, fat_arch->offset, fat_arch->cputype,
					fat_arch->cpusubtype, fat_arch->align);
	return (CORRUPTED);
}


int		fat_corrupted_print_error(char *str, struct fat_arch *fat_arch,
			t_browser *browser)
{
	ft_dprintf(2, str, browser->progname, browser->filename,
		fat_arch->cputype, fat_arch->cpusubtype);
	return (CORRUPTED);
}

int		check_all_architectures(struct fat_arch **found,
			struct fat_arch *fat_arch_array, t_header_parser *parser,
				t_browser *browser)
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
		if (fat_arch_array[i].offset < sizeof(fat_header))
		{
			ft_dprintf(2, "%s: %s truncated or malformed fat file "
				"(cputype (%d) cpusubtype (%d) offset %d "
					"overlaps universal headers)\n\n",
						browser->progname, browser->filename,
							fat_arch_array[i].cputype,
								fat_arch_array[i].cpusubtype, 
									fat_arch_array[i].offset);
			return (1);
		}
		if (!*found && (!ft_strcmp(ARCH, get_cpu_name(fat_arch_array[i].cputype, fat_arch_array[i].cpusubtype))))
			*found = &fat_arch_array[i];
		i++;
	}
	return (0);
}

int		fill_browser_fat32(t_header_parser *parser, t_browser *browser)
{
	uint32_t			i;
	struct fat_arch		*fat_arch;
	struct fat_header	*fat_header;
	struct fat_arch		*found;
	int					ret;

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
		if ((ret = process_browser_fat_arch32(&fat_arch[i], parser, browser)))
			return (ret);
		i++;
	}
	return (0);
}
