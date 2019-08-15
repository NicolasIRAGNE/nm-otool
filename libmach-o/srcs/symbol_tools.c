/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   symbol_tools.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ldedier <ldedier@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/08/15 16:46:52 by ldedier           #+#    #+#             */
/*   Updated: 2019/08/15 16:47:29 by ldedier          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "mach_o.h"

int			is_same_name_symbol(void *s, void *str)
{
	t_symbol	*symbol;
	char		*name;

	symbol = (t_symbol *)s;
	name = (char *)str;
	return (ft_strcmp(get_symbol_name(symbol), name));
}
