/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   options_tools.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ldedier <ldedier@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/06/21 01:42:29 by ldedier           #+#    #+#             */
/*   Updated: 2019/06/21 01:42:29 by ldedier          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_nm.h"

void	nm_option_error(char *str, t_nm_browser *browser)
{
	ft_printf("nm: for the %s option: may only occur zero or one times!\n",
		str);
	browser->ret = 1;
}

int		describe_option(char *str)
{
	return (str[0] == '-' && ft_strlen(str) > 1);
}

void	print_help(void)
{
	ft_printf("usage: nm [options] <input files>\n\nOPTIONS:\n\n");
	ft_printf("General options:\n\n");
	ft_printf(" -p: %s\n", "- Show symbols in order encountered");
	ft_printf(" -n: %s\n", "- Sort symbols by address");
	ft_printf(" -r: %s\n", "- Sort symbols in reverse order");
}
