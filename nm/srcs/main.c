/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: niragne <niragne@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/06/19 13:02:13 by niragne           #+#    #+#             */
/*   Updated: 2019/06/20 14:29:39 by niragne          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_nm.h"

void	print_output(int nsyms, int symoff, int stroff, char *ptr)
{
	int				i;
	char			*stringtable;
	struct nlist_64	*array;

	array = (void *)(ptr + symoff);
	stringtable = (void *)(ptr + stroff);
	i = 0;
	while (i < nsyms)
	{
		if (array[i].n_value)
		{
			ft_printf("%016llx T %s\n", array[i].n_value, stringtable + array[i].n_un.n_strx);
		}
		else
		{
			ft_printf("%18s %s\n", "U", stringtable + array[i].n_un.n_strx);
		}
		i++;
	}
}

void	handle_64(char *ptr, t_nm_browser *browser)
{
	struct mach_header_64 *header;
	struct load_command *lc;
	struct symtab_command *sym;
	uint64_t i;

	header = browser->header_union.header64;
	i = (uint64_t)(ptr + sizeof(*header));
	while (i < (uint64_t)(ptr + header->sizeofcmds))
	{
		lc = (struct load_command*) i;
		if (lc->cmd == LC_SYMTAB)
		{
			sym = (struct symtab_command *)lc;
			print_output(sym->nsyms, sym->symoff, sym->stroff, ptr);
		}
		i += lc->cmdsize;
	}
}

void	get_header(char *ptr, t_nm_browser *browser)
{
	uint32_t magic;

	magic = *(uint32_t*)ptr;
	if (magic == MH_MAGIC_64 || magic == MH_CIGAM_64)
	{
		ft_printf("Executable 64 bits\n");
		browser->type = E_64;
		browser->header_union.header64 = (struct mach_header_64*)ptr;
		handle_64(ptr, browser);
	}
	else if (magic == MH_MAGIC || magic == MH_CIGAM)
	{
		ft_printf("Executable 32 bits\n");
		browser->type = E_32;
		browser->header_union.header32 = (struct mach_header*)ptr;
	}
	else if (magic == FAT_MAGIC || magic == FAT_CIGAM)
	{
		ft_printf("Executable fat 32 bits\n");
		browser->type = E_FAT32;
		browser->header_union.fat_header = (struct fat_header*)ptr;
	}
	else if (magic == FAT_MAGIC_64 || magic == FAT_CIGAM_64)
	{
		ft_printf("Executable fat 64 bits\n");
		browser->type = E_FAT64;
		browser->header_union.fat_header = (struct fat_header*)ptr;
	}
}

int main(int ac, char **av)
{
	char		*ptr;
	int			fd;
	struct stat	buf;
	t_nm_browser browser;

	if (ac != 2)
	{
		ft_printf("error args\n");
		return (1);
	}
	if ((fd = open(av[1], O_RDONLY)) < 0)
	{
		ft_printf("error open\n");
		return (1);
	}
	if ((fstat(fd, &buf)) < 0)
	{
		ft_printf("error fstat\n");
		return (1);
	}
	if ((ptr = mmap(0, buf.st_size, PROT_READ, MAP_PRIVATE, fd, 0)) == MAP_FAILED)
	{
		ft_printf("ERROR MMAP\n");
		return (1);
	}
	get_header(ptr, &browser);
	nm_print_header(&browser);
	if (munmap(ptr, buf.st_size) < 0)
	{
		ft_printf("ERROR MUNMAP\n");
		return (1);
	}
	ft_printf("OK !\n");
	return (0);
}
