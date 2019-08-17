/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   opt_parse_2.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: niragne <niragne@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/08/17 14:07:38 by niragne           #+#    #+#             */
/*   Updated: 2019/08/17 14:08:12 by niragne          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int		opt_parse_short(t_arg_parser *parser, char *str)
{
	int i;

	i = 1;
	while (str[i])
	{
		if (opt_add_arg(parser, (t_arg_parsed){str[i], str, E_OPT_SHORT}))
			return (1);
		i++;
	}
	return (0);
}

int		opt_parse_long(t_arg_parser *parser, char *str)
{
	if (!*(str + 2))
	{
		parser->parsing = 0;
		return (0);
	}
	if (opt_add_arg(parser, (t_arg_parsed){0, str, E_OPT_LONG}))
		return (1);
	return (0);
}

int		opt_parse_str(t_arg_parser *parser, char *str)
{
	if (str[0] == '-')
	{
		if (str[1] == '-')
		{
			if (opt_parse_long(parser, str))
				return (1);
		}
		else
		{
			if (opt_parse_short(parser, str))
				return (1);
		}
	}
	else
	{
		parser->nb_args++;
		opt_add_arg(parser, (t_arg_parsed){0, str, E_ARG});
	}
	return (0);
}

int		opt_parse_args(t_arg_parser *parser, char **av)
{
	int				i;

	i = 0;
	parser->parsing = 1;
	while (av[i])
	{
		if (parser->parsing)
		{
			if (opt_parse_str(parser, av[i]))
				return (1);
		}
		else
			opt_add_arg(parser, (t_arg_parsed){0, av[i], E_ARG});
		i++;
	}
	return (0);
}
