/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   options_func.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: niragne <niragne@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/06/21 01:45:37 by ldedier           #+#    #+#             */
/*   Updated: 2019/08/08 19:33:52 by niragne          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_nm.h"

void	nm_opt_n(t_arg_parser *parser, void *flags)
{
	t_nm_wrapper *ptr;

	ptr = (t_nm_wrapper*)flags;
	(void)parser;
	if (ptr->flags->flag_n)
		nm_option_error("-numeric-sort", ptr->browser);
	ptr->browser->sort_func = cmp_symbol_numerical;
	ptr->flags->flag_n = 1;
}

void	nm_opt_p(t_arg_parser *parser, void *flags)
{
	t_nm_wrapper *ptr;

	(void)parser;
	ptr = (t_nm_wrapper*)flags;
	if (ptr->flags->flag_p)
		nm_option_error("-no-sort", ptr->browser);
	ptr->browser->sort_func = cmp_symbol_none;
	ptr->flags->flag_p = 1;
}

void	nm_opt_r(t_arg_parser *parser, void *flags)
{
	t_nm_wrapper *ptr;

	(void)parser;
	ptr = (t_nm_wrapper*)flags;
	if (ptr->flags->flag_r)
		nm_option_error("-reverse-sort", ptr->browser);
	ptr->browser->sort_func = cmp_symbol_none;
	ptr->flags->flag_r = 1;
	ptr->browser->sort_mult = -1;
	if (ptr->flags->flag_n)
		ptr->browser->sort_func = cmp_symbol_numerical_inv;
	else
		ptr->browser->sort_func = cmp_symbol_alpha_inv;
}
