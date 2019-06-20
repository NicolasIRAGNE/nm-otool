/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   symbols.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ldedier <ldedier@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/06/20 17:45:01 by ldedier           #+#    #+#             */
/*   Updated: 2019/06/20 18:40:16 by ldedier          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_nm.h"

int		cmp_symbol_alpha(void *s1, void *s2)
{
	t_symbol *symbol1;
	t_symbol *symbol2;

	symbol1 = (t_symbol *)s1;
	symbol2 = (t_symbol *)s2;
	return (ft_strcmp(symbol1->name, symbol2->name));
}
