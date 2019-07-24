/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   opt_parse.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: niragne <niragne@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/07/24 13:49:12 by niragne           #+#    #+#             */
/*   Updated: 2019/07/24 19:00:41 by niragne          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	opt_add_to_parser(t_arg_parser *parser, t_arg_option opt)
{
    ft_add_to_list_back(&parser->lst, (void*)&opt, sizeof(opt));
}

void	opt_init_parser(t_arg_parser *parser)
{
	ft_bzero(parser, sizeof(*parser));
}

void    opt_print_parser(t_arg_parser *parser)
{
    t_list  *lst;

    lst = parser->lst;
    while (lst)
    {
        t_arg_option *test;
        test = (t_arg_option*)lst->content;
        ft_printf("long_name = %s, short_name = %c, value = %d, index = %d\n",
        test->long_name, test->short_name, test->value, test->flag_index);
        lst = lst->next;
    }
}

/*void	opt_parse_str(t_arg_parser *parser, char *str)
{
	int i;
	int flag = 0;

	i = 0;
	while (str[i])
	{
		if (str[i] == '-')
		{
			if (flag)
			{
				
			}
			flag = 1;
			i++;
		}
		else
		{
			if (flag)
			{
				int k = 0;
			}
			i++;		
		}
	}
}*/

/*void	opt_parse_args(t_arg_parser *parser, char **av)
{
	int i = 0;

	while (av[i])
	{
		parse_str(parser, av[i]);
		i++;
	}
}*/