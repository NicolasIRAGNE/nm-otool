/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fill_fat32_tools.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ldedier <ldedier@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/08/15 16:48:52 by ldedier           #+#    #+#             */
/*   Updated: 2019/08/15 16:48:53 by ldedier          ###   ########.fr       */
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

int		fat_corrupted_print_error_alignment(struct fat_arch *fat_arch,
				t_browser *browser)
{
	ft_dprintf(2, "%s: %s truncated or malformed "
		"fat file (offset: %d for cputype (%d) cpusubtype (%d) "
			"not aligned on its alignment (2^%d))\n\n", browser->progname,
				browser->filename, fat_arch->offset, fat_arch->cputype,
					fat_arch->cpusubtype, fat_arch->align);
	return (ERROR_CORRUPTED);
}

int		fat_corrupted_print_error(char *str, struct fat_arch *fat_arch,
			t_browser *browser)
{
	ft_dprintf(2, str, browser->progname, browser->filename,
		fat_arch->cputype, fat_arch->cpusubtype);
	return (ERROR_CORRUPTED);
}

int		is_corrupted_fat_arch(struct fat_arch *fat_arch,
			t_header_parser *parser, t_browser *browser)
{
	if (is_corrupted_offset(parser->offset + fat_arch->offset,
		fat_arch->size, browser))
	{
		return (fat_corrupted_print_error("%s: %s truncated or malformed "
		"fat file (offset plus size of cputype (%d) cpusubtype (%d) "
		"extends past the end of the file)\n\n",
		fat_arch, browser));
		return (1);
	}
	if (fat_arch->offset < sizeof(struct fat_header))
	{
		ft_dprintf(2, "%s: %s truncated or malformed fat file "
			"(cputype (%d) cpusubtype (%d) offset %d "
			"overlaps universal headers)\n\n", browser->progname,
			browser->filename, fat_arch->cputype,
			fat_arch->cpusubtype, fat_arch->offset);
		return (1);
	}
	return (0);
}
