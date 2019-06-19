/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   debug.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: niragne <niragne@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/06/19 13:59:56 by niragne           #+#    #+#             */
/*   Updated: 2019/06/19 15:37:54 by niragne          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_nm.h"

void    nm_print_header(t_nm_browser *browser)
{
    if (browser->type == E_64)
    {
        struct mach_header_64 *ptr;
        ptr = (struct mach_header_64*) browser->header;
        ft_dprintf(2,"mach magic number identifier: %x\n", ptr->magic);
        ft_dprintf(2,"cpu specifier: %p\n", ptr->cputype);
        ft_dprintf(2,"machine specifier: %p\n", ptr->cpusubtype);
        ft_dprintf(2,"type of file: %d\n", ptr->filetype);
        ft_dprintf(2,"number of load commands: %d\n", ptr->ncmds);
        ft_dprintf(2,"the size of all the load commands: %d\n", ptr->sizeofcmds);
        ft_dprintf(2,"flags: %d\n", ptr->flags);
        ft_dprintf(2,"reserved: %d\n", ptr->reserved);
    }
    else if (browser->type == E_32)
    {
        struct mach_header *ptr;
        ptr = (struct mach_header*) browser->header;
        ft_dprintf(2,"mach magic number identifier: %x\n", ptr->magic);
        ft_dprintf(2,"cpu specifier: %p\n", ptr->cputype);
        ft_dprintf(2,"machine specifier: %p\n", ptr->cpusubtype);
        ft_dprintf(2,"type of file: %d\n", ptr->filetype);
        ft_dprintf(2,"number of load commands: %d\n", ptr->ncmds);
        ft_dprintf(2,"the size of all the load commands: %d\n", ptr->sizeofcmds);
        ft_dprintf(2,"flags: %d\n", ptr->flags);
    }
}