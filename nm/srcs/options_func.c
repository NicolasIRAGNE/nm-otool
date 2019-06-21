/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   options_func.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ldedier <ldedier@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/06/21 01:45:37 by ldedier           #+#    #+#             */
/*   Updated: 2019/06/21 01:45:37 by ldedier          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_nm.h"

void	nm_opt_n(t_nm_browser *browser, int *options)
{
	if (*options & SORT_NUMERICAL)
		nm_option_error("-numeric-sort", browser);
	browser->sort_func = cmp_symbol_numerical;
	*options |= SORT_NUMERICAL;
}

void	nm_opt_p(t_nm_browser *browser, int *options)
{
	if (*options & SORT_NONE)
		nm_option_error("-no-sort", browser);
	browser->sort_func = cmp_symbol_none;
	*options |= SORT_NONE;
}

void	nm_opt_r(t_nm_browser *browser, int *options)
{
	if (*options & SORT_REVERSE)
		nm_option_error("-reverse-sort", browser);
	browser->sort_mult = -1;
	*options |= SORT_REVERSE;
}
