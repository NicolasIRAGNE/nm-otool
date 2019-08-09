/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print_usage.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: niragne <niragne@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/08/06 15:30:16 by niragne           #+#    #+#             */
/*   Updated: 2019/08/08 18:11:41 by niragne          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char    *get_shorts(t_arg_parser *parser)
{
    size_t i;

	i = 0;
    char    *str;
    str = malloc((parser->opts_size * sizeof(char)));
    if (!str)
        return (NULL);
	while (i * sizeof(t_arg_option) < parser->opts_size)
    {
        if (parser->opts[i].short_name)
        {
            str[i] = parser->opts[i].short_name;
    		i++;
        }
    }
    str[i] = 0;
    return (str);
}

void    print_usage(t_arg_parser *parser)
{
   	size_t i;
    char *shorts;

    shorts = get_shorts(parser);
    ft_dprintf(2, "Usage: %s [-%s] <object file> ...\n", parser->prog_name, shorts);
    free(shorts);
	i = 0;
	while (i * sizeof(t_arg_option) < parser->opts_size)
    {
        if (parser->opts[i].short_name)
        {
            ft_dprintf(2, "		-%c",  parser->opts[i].short_name);
            if (parser->opts[i].long_name)
            {
                ft_dprintf(2, ", --%s",  parser->opts[i].long_name);
            }
        }
        else if (parser->opts[i].long_name)
        {
            ft_dprintf(2, "		--%s",  parser->opts[i].long_name);
        }
        ft_dprintf(2, ": %s\n", parser->opts[i].description);
		i++;
    }

}
