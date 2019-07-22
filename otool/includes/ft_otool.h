/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_otool.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ldedier <ldedier@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/07/18 11:26:01 by ldedier           #+#    #+#             */
/*   Updated: 2019/07/22 18:42:24 by ldedier          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FT_OTOOL_H
# define FT_OTOOL_H

# include "mach_o.h"
# include "libft.h"

typedef struct		s_text_printer
{
	unsigned char	*data;
	uint64_t		data_size;
	uint64_t		addr;
	int				should_swap;
	int				alignment;
}					t_text_printer;

/*
** print.c
*/
void	otool_print(t_browser *browser);

#endif
