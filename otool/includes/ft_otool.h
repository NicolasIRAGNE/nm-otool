/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_otool.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: niragne <niragne@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/06/24 17:06:14 by niragne           #+#    #+#             */
/*   Updated: 2019/07/01 16:38:51 by niragne          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FT_OTOOL
# define FT_OTOOL

# include <stdio.h>
# include <stdlib.h>
# include "libft.h"
# include <sys/mman.h>
# include <fcntl.h>
# include <sys/stat.h>
# include "shared.h"

# define PRINT_LINE_SIZE 16

void	dump_mem(unsigned char *data, size_t data_size);

#endif