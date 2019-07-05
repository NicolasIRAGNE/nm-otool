/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tools.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: niragne <niragne@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/07/01 16:44:40 by niragne           #+#    #+#             */
/*   Updated: 2019/07/01 16:50:10 by niragne          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_otool.h"

void    print_usage(char *name)
{
    ft_printf("error: %s: at least one file must be specified\n", name);
    ft_printf("usage: %s <object file> ...\n", name);
    exit(1);
}