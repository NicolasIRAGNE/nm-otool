/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   debug.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: niragne <niragne@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/06/19 13:59:56 by niragne           #+#    #+#             */
/*   Updated: 2019/06/19 14:07:55 by niragne          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_nm.h"

void    nm_print_header(t_nm_browser *browser)
{
    if (browser->type == E_64)
    {
        struct mach_header_64 *ptr;
        ptr = (struct mach_header_64*) browser->header;
        ft_printf("mach magic number identifier: %x\n", ptr->magic);
        ft_printf("cpu specifier: %x\n", ptr->cputype);
        ft_printf("machine specifier: %x\n", ptr->cpusubtype);
        ft_printf("type of file: %d\n", ptr->filetype);
        ft_printf("number of load commands: %d\n", ptr->ncmds);
        ft_printf("the size of all the load commands: %d\n", ptr->sizeofcmds);
        ft_printf("flags: %d\n", ptr->flags);
        ft_printf("reserved: %d\n", ptr->reserved);
    }
}