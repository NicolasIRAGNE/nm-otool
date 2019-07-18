/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   corrupted.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ldedier <ldedier@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/06/23 19:10:30 by ldedier           #+#    #+#             */
/*   Updated: 2019/07/15 18:00:24 by ldedier          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "mach_o.h"

int		ret_corr(t_browser *browser)
{
	browser->ret = 1;
	return (1);
}

int		is_corrupted_string(char *str, t_browser *browser, int *len)
{
	int		i;
	
	*len = -1;
	if ((void *)str >= browser->ptr + browser->st.st_size
		|| (void *)str < browser->ptr)
		return (1);
	i = 0;
	while ((void *)str + i < browser->ptr + browser->st.st_size - 1 && str[i])
		i++;
	if (str[i])
	{
		*len = i + 1;
		return (1);
	}
	return (0);
}

int		is_corrupted_data(void *address, size_t size, t_browser *browser)
{
	if (address + size > browser->ptr + browser->st.st_size
		|| address < browser->ptr)
		return (ret_corr(browser));
	return (0);
}

int		is_corrupted_offset(uint64_t offset, uint64_t size,
			t_browser *browser)
{
	if (offset + size > (uint64_t)browser->st.st_size)
		return (ret_corr(browser));
	return (0);
}
