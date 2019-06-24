/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   corrupted.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ldedier <ldedier@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/06/23 19:10:30 by ldedier           #+#    #+#             */
/*   Updated: 2019/06/23 19:10:30 by ldedier          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_nm.h"

int		is_corrupted_string(char *str, t_nm_browser *browser)
{
	int		i;

	if ((void *)str >= browser->ptr + browser->st.st_size || (void *)str < browser->ptr)
		return (1);
	i = 0;
	while (str[i] && (void *)str + i < browser->ptr + browser->st.st_size)
		i++;
	if (str[i])
		return (1);
	return (0);
}

int		is_corrupted_data(void *address, size_t size, t_nm_browser *browser)
{
	if (address + size > browser->ptr + browser->st.st_size
		|| address < browser->ptr)
		return (1);
	return (0);
}
