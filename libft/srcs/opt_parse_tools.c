/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   opt_parse_tools.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: niragne <niragne@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/08/17 14:05:39 by niragne           #+#    #+#             */
/*   Updated: 2019/08/17 14:11:47 by niragne          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int		opt_free(t_arg_parser *parser, int ret)
{
	ft_lstdel_value(&parser->parsed);
	return (ret);
}

void	opt_add_to_parser(t_arg_parser *parser, t_arg_option *opt, size_t size)
{
	parser->opts = opt;
	parser->opts_size = size;
}

int		opt_add_arg(t_arg_parser *parser, t_arg_parsed opt)
{
	if (ft_add_to_list_back(&parser->parsed, (void*)&opt, sizeof(opt)))
		return (1);
	else
		return (0);
}

void	opt_init_parser(t_arg_parser *parser, void (*f)(t_arg_parser *, void *),
	char *progname)
{
	ft_bzero(parser, sizeof(*parser));
	parser->invalid = f;
	parser->prog_name = progname;
}

void	opt_print_parser_opt(t_arg_parser *parser)
{
	size_t i;

	i = 0;
	while (i * sizeof(t_arg_option) < parser->opts_size)
	{
		ft_printf("long_name = %s, short_name = %c\n",
			parser->opts[i].long_name, parser->opts[i].short_name);
		i++;
	}
}
