/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memcpy.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: niragne <niragne@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/11/06 20:14:52 by ldedier           #+#    #+#             */
/*   Updated: 2019/06/11 17:34:48 by niragne          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_memcpy(void *dest, const void *src, size_t n)
{
	while (n >= 8)
	{
		*(unsigned long *)(dest) = *(unsigned long *)(src);
		dest += 8;
		src += 8;
		n -= 8;
	}
	while (n > 0)
	{
		*(char *)(dest) = *(char *)(src);
		dest++;
		src++;
		n -= 1;
	}
	return (dest);
}
