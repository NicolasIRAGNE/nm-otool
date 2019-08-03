/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   opt_parse.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: niragne <niragne@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/07/24 13:49:12 by niragne           #+#    #+#             */
/*   Updated: 2019/08/01 16:56:09 by niragne          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int		opt_add_to_parser(t_arg_parser *parser, t_arg_option opt)
{
    if (ft_add_to_list_back(&parser->lst, (void*)&opt, sizeof(opt)))
		return (1);
	else
		return (0);
}

int		opt_add_arg(t_arg_parser *parser, t_arg_parsed opt)
{
    if (ft_add_to_list_back(&parser->parsed, (void*)&opt, sizeof(opt)))
		return (1);
	else
		return (0);
}

void	opt_init_parser(t_arg_parser *parser)
{
	ft_bzero(parser, sizeof(*parser));
}

void    opt_print_parser_opt(t_arg_parser *parser)
{
    t_list  *lst;

    lst = parser->lst;
    while (lst)
    {
        t_arg_option *test;
        test = (t_arg_option*)lst->content;
        ft_printf("long_name = %s, short_name = %c, set = %d, value = %s, index = %d\n",
        test->long_name, test->short_name, test->set, test->value, test->flag_index);
        lst = lst->next;
    }
}

void    opt_print_parsed(t_arg_parser *parser)
{
    t_list  *lst;

    lst = parser->parsed;
    while (lst)
    {
        t_arg_parsed *test;
        test = (t_arg_parsed*)lst->content;
        ft_printf("long_name = %s, short_name = %c, type = %d\n",
        test->long_name, test->short_name, test->type);
        lst = lst->next;
    }
}

t_arg_option		*find_opt_by_short(t_arg_parser *parser, char c)
{
	t_list  *lst;

    lst = parser->lst;
    while (lst)
    {
        t_arg_option *test;
        test = (t_arg_option*)lst->content;
        if (test->short_name == c)
		{
			return (test);
		}
        lst = lst->next;
    }
	return (NULL);
}

int		opt_parse_short(t_arg_parser *parser, char *str)
{
	int i;

	i = 0;
	while (str[i])
	{
		if (opt_add_arg(parser, (t_arg_parsed){str[i], 0, E_OPT}))
			return (1);
		i++;
	}
	return (0);
}

int		opt_parse_long(t_arg_parser *parser, char *str)
{
	if (!*str)
	{
		parser->parsing = 0;
		return (0);
	}
	if (opt_add_arg(parser, (t_arg_parsed){0, str, E_OPT}))
		return (1);
	return (0);
}

int		opt_parse_str(t_arg_parser *parser, char *str)
{

	if (str[0] == '-')
	{
		if (str[1] == '-')
		{
			if (opt_parse_long(parser, str + 2))
				return (1);
		}
		else
		{
			if (opt_parse_short(parser, str + 1))
				return (1);
		}
	}
	opt_add_arg(parser, (t_arg_parsed){0, str, E_ARG});
	return (0);
}


int		opt_parse_args(t_arg_parser *parser, char **av)
{
	int i = 0;
	parser->parsing = 1;

	while (av[i] && parser->parsing)
	{
		if (opt_parse_str(parser, av[i]))
			return (1);
		i++;
	}
	return (0);
}