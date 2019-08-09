/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   opt_parse.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: niragne <niragne@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/07/24 13:49:12 by niragne           #+#    #+#             */
/*   Updated: 2019/08/09 15:31:41 by niragne          ###   ########.fr       */
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

void	opt_init_parser(t_arg_parser *parser, void (*f)(t_arg_parser *, void *), char *progname)
{
	ft_bzero(parser, sizeof(*parser));
	parser->invalid = f;
	parser->prog_name = progname;
}

void    opt_print_parser_opt(t_arg_parser *parser)
{
	size_t i;

	i = 0;
	while (i * sizeof(t_arg_option) < parser->opts_size)
    {
        ft_printf("long_name = %s, short_name = %c\n",
        parser->opts[i].long_name,parser->opts[i].short_name);
		i++;
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

t_arg_option		*find_opt_by_long(t_arg_parser *parser, char *str)
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

t_arg_option		*find_opt(t_arg_parser *parser, t_arg_parsed *opt)
{
	if (opt->type == E_OPT_SHORT)
		return (find_opt_by_short(parser, opt->short_name));
	if (opt->type == E_OPT_LONG)
		return (find_opt_by_long(parser, opt->long_name));
	return (NULL);
}

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
	int i = 0;
	parser->parsing = 1;

	while (av[i])
	{
		if  (parser->parsing)
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

void    process_opt(t_arg_parser *parser, void *flags)
{
    t_list  *lst;
	t_arg_option *opt;

    lst = parser->parsed;
    while (lst)
    {
        t_arg_parsed *test;
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

