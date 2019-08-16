/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print_section_tools.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ldedier <ldedier@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/08/16 15:38:52 by ldedier           #+#    #+#             */
/*   Updated: 2019/08/16 15:39:38 by ldedier          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_otool.h"

void	print_text_section(unsigned char *data, uint64_t data_size,
		uint64_t addr, int alignment)
{
	size_t			i;
	int				j;

	i = 0;
	while (i < data_size)
	{
		j = 0;
		ft_printf("%0*llx\t", alignment, addr + i);
		while (j < 16 && i + j < data_size)
		{
			ft_printf("%.2x ", data[i + j]);
			j++;
		}
		ft_printf("\n");
		i += 16;
	}
}

void	fill_text_printer(t_section *section, t_header_parser *parser,
			t_text_printer *printer)
{
	printer->alignment = section->section_enum == E_SECTION_32 ? 8 : 16;
	printer->addr = get_section_addr(section);
	printer->data = (void *)parser->ptr + get_section_offset(section);
	printer->data_size = get_section_size(section);
	printer->should_swap = (is_little_endian() != parser->should_swap);
}

void	process_print(t_text_printer *p, size_t i, int j, int k)
{
	if (p->should_swap)
		ft_printf("%.2x", p->data[i + j + 4 - k - 1]);
	else
		ft_printf("%.2x", p->data[i + j + k]);
}

void	print_text_section_4_by_4(t_section *section, t_header_parser *parser)
{
	size_t			i;
	int				j;
	int				k;
	t_text_printer	p;

	fill_text_printer(section, parser, &p);
	i = 0;
	while (i < p.data_size)
	{
		j = 0;
		ft_printf("%0*llx\t", p.alignment, p.addr + i);
		while (j < 16 && i + j < p.data_size)
		{
			k = 0;
			while (k < 4 && i + j + k < p.data_size)
				process_print(&p, i, j, k++);
			j += 4;
			ft_printf(" ");
		}
		ft_printf("\n");
		i += 16;
	}
}
