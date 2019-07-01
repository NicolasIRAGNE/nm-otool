/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: niragne <niragne@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/06/24 17:13:11 by niragne           #+#    #+#             */
/*   Updated: 2019/07/01 14:22:54 by niragne          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_otool.h"

size_t          align(size_t x, size_t n)
{
        if (x == 0)
                return (n);
        else
                return ((x + n - 1) & ~(n - 1));
}

void	dump_mem(unsigned char *data, size_t data_size)
{
	char			buffer[PRINT_LINE_SIZE];
	size_t			i;

	i = 0;
	ft_printf("%p: ", data);
	while (i < align(data_size, PRINT_LINE_SIZE))
	{
		if (i < data_size)
		{
			if (!((i) % PRINT_LINE_SIZE) && i >= PRINT_LINE_SIZE)
				ft_printf("|%.*s|\n%p: ", PRINT_LINE_SIZE, buffer, data + i);
			ft_printf("%.2x ", data[i]);
			buffer[i % PRINT_LINE_SIZE] = ft_isprint(data[i]) ? data[i] : '.';
		}
		else
		{
			ft_printf("   ");
			buffer[i % PRINT_LINE_SIZE] = 0;
		}
		i++;
	}
	ft_printf("|%.*s|\n", PRINT_LINE_SIZE, buffer);
}