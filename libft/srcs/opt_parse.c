/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   opt_parse.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: niragne <niragne@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/07/24 13:49:12 by niragne           #+#    #+#             */
/*   Updated: 2019/08/17 14:09:11 by niragne          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void			opt_print_parsed(t_arg_parser *parser)
{
	t_list			*lst;
	t_arg_parsed	*test;

	lst = parser->parsed;
	while (lst)
	{
		test = (t_arg_parsed*)lst->content;
		ft_printf("long_name = %s, short_name = %c, type = %d\n",
		test->long_name, test->short_name, test->type);
		lst = lst->next;
	}
}

t_arg_option	*find_opt_by_short(t_arg_parser *parser, char c)
{
	size_t i;

	i = 0;
	while (i * sizeof(t_arg_option) < parser->opts_size)
	{
		if (parser->opts[i].short_name == c)
		{
			return (parser->opts + i);
		}
		i++;
	}
	return (NULL);
}

t_arg_option	*find_opt_by_long(t_arg_parser *parser, char *str)
{
	size_t i;

	i = 0;
	while (i * sizeof(t_arg_option) < parser->opts_size)
	{
		if (ft_strcmp(parser->opts[i].long_name, str + 2) == 0)
		{
			return (parser->opts + i);
		}
		i++;
	}
	return (NULL);
}

t_arg_option	*find_opt(t_arg_parser *parser, t_arg_parsed *opt)
{
	if (opt->type == E_OPT_SHORT)
		return (find_opt_by_short(parser, opt->short_name));
	if (opt->type == E_OPT_LONG)
		return (find_opt_by_long(parser, opt->long_name));
	return (NULL);
}

void			process_opt(t_arg_parser *parser, void *flags)
{
	t_list			*lst;
	t_arg_option	*opt;
	t_arg_parsed	*test;

	lst = parser->parsed;
	while (lst)
	{
		test = (t_arg_parsed*)lst->content;
		parser->current = test;
		if (test->type & (E_OPT_LONG & E_OPT_SHORT))
		{
			if ((opt = find_opt(parser, test)))
				opt->f(parser, flags);
			else
				parser->invalid(parser, flags);
		}
		lst = lst->next;
	}
}
