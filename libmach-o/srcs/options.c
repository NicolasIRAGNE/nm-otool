/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   options.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ldedier <ldedier@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/06/20 22:46:21 by ldedier           #+#    #+#             */
/*   Updated: 2019/07/22 14:25:09 by ldedier          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "mach_o.h"

static int	nm_found_option(char c, t_browser *browser, int *options)
{
	if (c == 'p')
		nm_opt_p(browser, options);
	else if (c == 'n')
		nm_opt_n(browser , options);
	else if (c == 'r')
		nm_opt_r(browser, options);
	else
		return (0);
	return (1);
}

int			process_parse_options(char *str, t_browser *browser,
		int *options, char **argv)
{
	int i;

	if (!ft_strcmp(str, "-help"))
	{
		print_help();
		return (2);
	}
	i = 1;
	while (str[i])
	{
		if (!nm_found_option(str[i], browser, options))
		{
			ft_dprintf(2, "nm: Unknown command line argument '%s'.\n", str);
			ft_dprintf(2, "Try: '%s -help'\n", argv[0]);
			return (1);
		}
		i++;
	}
	return (0);
}

int			parse_options(int *i, int ac, char **av, t_browser *browser)
{
	int options;
	int ret;

	*i = 1;
	options = 0;
	while (*i < ac && describe_option(av[*i]))
	{
		if (!ft_strcmp("--", av[*i]))
		{
			(*i)++;
			break ;
		}
		if ((ret = process_parse_options(av[*i], browser, &options, av)))
			return (ret);
		(*i)++;
	}
	return (0);
}
