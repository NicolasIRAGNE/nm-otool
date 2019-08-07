/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print_usage.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: niragne <niragne@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/08/06 15:30:16 by niragne           #+#    #+#             */
/*   Updated: 2019/08/07 17:22:08 by niragne          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void    print_usage(t_arg_parser *parser)
{
   	size_t i;

    ft_printf("Usage: %s <object file>\n", parser->prog_name);
	i = 0;
	while (i * sizeof(t_arg_option) < parser->opts_size)
    {
        if (parser->opts[i].short_name)
        {
            ft_printf("		-%c",  parser->opts[i].short_name);
            if (parser->opts[i].long_name)
            {
                ft_printf(", --%s",  parser->opts[i].long_name);
            }
        }
        else if (parser->opts[i].long_name)
        {
            ft_printf("		--%s",  parser->opts[i].long_name);
        }
        ft_printf(": %s\n", parser->opts[i].description);
		i++;
    }

}