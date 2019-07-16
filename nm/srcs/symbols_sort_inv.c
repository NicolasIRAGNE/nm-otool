/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   symbols_sort_inv.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ldedier <ldedier@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/07/16 09:24:23 by ldedier           #+#    #+#             */
/*   Updated: 2019/07/16 09:41:06 by ldedier          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_nm.h"

long		cmp_symbol_alpha_inv(void *s1, void *s2, void *env)
{
	return (-cmp_symbol_alpha(s1, s2, env));
}

long		cmp_symbol_numerical_inv(void *s1, void *s2, void *env)
{
	return (-cmp_symbol_numerical(s1, s2, env));
}
