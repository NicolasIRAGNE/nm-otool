/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: niragne <niragne@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/06/19 13:02:13 by niragne           #+#    #+#             */
/*   Updated: 2019/06/20 19:45:58 by niragne          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_nm.h"

void	debug_list(t_list *segments)
{
	struct segment_command_64 *seg;

	while (segments)
	{
		seg = (struct segment_command_64 *)segments->content;
		ft_printf("%s\n", seg->segname);
		segments = segments->next;
	}
}

void	*banane(t_list *segments, uint8_t n_sect)
{
	struct segment_command_64 *seg;
	uint8_t i;

	i = 0;

	while (segments && i < n_sect)
	{
		seg = (struct segment_command_64*)segments->content;
		if (i == n_sect - 1)
		{
			return (seg);
		}
		segments = segments->next;
		i++;
	}
	return (NULL);
}

char	get_letter(char *str, uint8_t type)
{
	char ret;
	if (str == NULL)
		ret = 'U';

	if (ft_strcmp(str, SEG_TEXT) == 0)
	{
		ret = 'D';
	}
	else if (ft_strcmp(str, SEG_LINKEDIT) == 0)
	{
		ret = 'T';
	}
	else if (ft_strcmp(str, SEG_DATA) == 0)
	{
		ret = 'B';
	}
	else if (ft_strcmp(str, SEG_IMPORT) == 0)
	{
		ret = 'I';
	}
	else
	{
		ret = 'U';
	}
	if (!(type & N_EXT))
		ret = ft_tolower(ret);
	return (ret);
}

void	print_output(int nsyms, int symoff, int stroff, char *ptr, t_list *segments)
{
	int				i;
	char			*stringtable;
	struct segment_command_64			*seg;
	struct nlist_64	*array;

	array = (void *)(ptr + symoff);
	stringtable = (void *)(ptr + stroff);
	i = 0;
	debug_list(segments);
	while (i < nsyms)
	{
		seg = (struct segment_command_64*)(banane(segments, array[i].n_sect));
		char letter;
		if (seg)
			letter = get_letter(seg->segname, array[i].n_type);
		else
			letter = 'U';
		ft_printf("%016llx %c %s\n", array[i].n_value, letter, stringtable + array[i].n_un.n_strx);
		(void)segments;
		i++;
	}
}

void	print_output32(int nsyms, int symoff, int stroff, char *ptr)
{
	int				i;
	char			*stringtable;
	struct nlist	*array;

	array = (void *)(ptr + symoff);
	stringtable = (void *)(ptr + stroff);
	i = 0;
	while (i < nsyms)
	{
		if (array[i].n_value)
		{
			ft_printf("%08llx T %s\n", array[i].n_value, stringtable + array[i].n_un.n_strx);
		}
		else
		{
			ft_printf("%10s %s\n", "U", stringtable + array[i].n_un.n_strx);
		}
		i++;
	}
}


void	handle_64(char *ptr, t_nm_browser *browser)
{
	struct mach_header_64 *header;
	struct load_command *lc;
	struct symtab_command *sym;
	struct segment_command_64 *seg;
	t_list *segments = NULL;
	uint64_t i;

	header = browser->header_union.header64;

	i = (uint64_t)(ptr + sizeof(*header));
	while (i < (uint64_t)(ptr + header->sizeofcmds))
	{
		lc = (struct load_command*) i;
		if (lc->cmd == LC_SEGMENT_64)
		{
			seg = (struct segment_command_64 *)lc;
			ft_add_to_list_ptr(&segments, seg, sizeof(seg));
		}
		if (lc->cmd == LC_SYMTAB)
		{
			sym = (struct symtab_command *)lc;
			print_output(sym->nsyms, sym->symoff, sym->stroff, ptr, segments);
		}
		i += lc->cmdsize;
	}
}

void	handle_32(char *ptr, t_nm_browser *browser)
{
	struct mach_header *header;
	struct load_command *lc;
	struct symtab_command *sym;
	uint64_t i;

	header = browser->header_union.header32;
	i = (uint64_t)(ptr + sizeof(*header));
	while (i < (uint64_t)(ptr + header->sizeofcmds))
	{
		lc = (struct load_command*) i;
		if (lc->cmd == LC_SYMTAB)
		{
			sym = (struct symtab_command *)lc;
			print_output32(sym->nsyms, sym->symoff, sym->stroff, ptr);
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
		handle_32(ptr, browser);
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
	else
		browser->type = E_UNKNOWN;
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
