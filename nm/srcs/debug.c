/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   debug.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: niragne <niragne@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/06/19 13:59:56 by niragne           #+#    #+#             */
/*   Updated: 2019/06/20 15:42:47 by niragne          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_nm.h"

void	debug_mach_header(struct mach_header *ptr)
{
        ft_dprintf(2,"mach magic number identifier: %x\n", ptr->magic);
        ft_dprintf(2,"cpu specifier: %p\n", ptr->cputype);
        ft_dprintf(2,"machine specifier: %p\n", ptr->cpusubtype);
        ft_dprintf(2,"type of file: %d\n", ptr->filetype);
        ft_dprintf(2,"number of load commands: %d\n", ptr->ncmds);
        ft_dprintf(2,"the size of all the load commands: %d\n", ptr->sizeofcmds);
        ft_dprintf(2,"flags: %d\n", ptr->flags);
}

void    nm_print_header(t_header_parser *parser)
{
    if (parser->type == E_64)
    {
        struct mach_header_64 *ptr;
        ptr = (struct mach_header_64*)parser->header_union.header64;
        ft_dprintf(2,"mach magic number identifier: %x\n", ptr->magic);
        ft_dprintf(2,"cpu specifier: %p\n", ptr->cputype);
        ft_dprintf(2,"machine specifier: %p\n", ptr->cpusubtype);
        ft_dprintf(2,"type of file: %d\n", ptr->filetype);
        ft_dprintf(2,"number of load commands: %d\n", ptr->ncmds);
        ft_dprintf(2,"the size of all the load commands: %d\n", ptr->sizeofcmds);
        ft_dprintf(2,"flags: %d\n", ptr->flags);
        ft_dprintf(2,"reserved: %d\n", ptr->reserved);
    }
    else if (parser->type == E_32)
     	debug_mach_header(parser->header_union.header32);
    else if (parser->type == E_FAT32 || parser->type == E_FAT64)
    {
        struct fat_header *ptr;
        ptr = parser->header_union.fat_header;
		ft_dprintf(2,"mach magic number identifier: %x\n", ptr->magic);
        ft_dprintf(2,"number of arch : %u\n", ptr->nfat_arch);
    }
    else if (parser->type == E_UNKNOWN)
    {
        ft_dprintf(2, "Unknown file type\n");
    }
}
