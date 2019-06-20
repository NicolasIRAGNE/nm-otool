/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: niragne <niragne@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/06/19 13:02:13 by niragne           #+#    #+#             */
/*   Updated: 2019/06/20 19:45:42 by ldedier          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_nm.h"
/*
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
*/
/*
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
			//fill_symbols(sym->nsyms, sym->symoff, sym->stroff, ptr);
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
*/

void	print_symbol(t_symbol *symbol)
{

}

void	print_symbol_tree(t_tree *tree)
{
	if (tree)
	{
		print_symbol_tree(tree->left);
		print_symbol(tree->content);
		print_symbol_tree(tree->right);
	}
}

void	nm_print(t_nm_browser *browser)
{

}

void	get_header(t_nm_browser *browser)
{
	browser->magic = *(uint32_t*)browser->ptr;
	if (browser->magic == MH_MAGIC_64 || browser->magic == MH_CIGAM_64)
	{
		ft_printf("Executable 64 bits\n");
		browser->type = E_64;
		browser->header_union.header64 = (struct mach_header_64*)browser->ptr;
	}
	else if (browser->magic == MH_MAGIC || browser->magic == MH_CIGAM)
	{
		ft_printf("Executable 32 bits\n");
		browser->type = E_32;
		browser->header_union.header32 = (struct mach_header*)browser->ptr;
	}
	else if (browser->magic == FAT_MAGIC || browser->magic == FAT_CIGAM)
	{
		ft_printf("Executable fat 32 bits\n");
		browser->type = E_FAT32;
		browser->header_union.fat_header = (struct fat_header*)browser->ptr;
	}
	else if (browser->magic == FAT_MAGIC_64 || browser->magic == FAT_CIGAM_64)
	{
		ft_printf("Executable fat 64 bits\n");
		browser->type = E_FAT64;
		browser->header_union.fat_header = (struct fat_header*)browser->ptr;
	}
	else
		browser->type = E_UNKNOWN;
}

t_symbol	*nm_new_symbol64(struct nlist_64 *nlist, char *symbol_name)
{
	t_symbol *symbol;

	if (!(symbol = (t_symbol *)malloc(sizeof(t_symbol))))
		return (NULL);
	symbol->nlist = nlist;
	symbol->name = symbol_name;
	return (symbol);
}

int		process_fill_symbols64(t_nm_browser *browser, int nsyms, int symoff, int stroff)
{
	int				i;
	char			*stringtable;
	struct nlist_64	*array;
	t_symbol		*new_symbol;

	array = (void *)(browser->ptr + symoff);
	stringtable = (void *)(browser->ptr + stroff);
	i = 0;
	while (i < nsyms)
	{
		if (!(new_symbol = nm_new_symbol64(&array[i], stringtable + array[i].n_un.n_strx)))
			return (1);
		if (ft_tree_add_sorted_mul(&browser->symbols, new_symbol,
				browser->sort_func, browser->sort_mult))
			return (1);
		i++;
	}
	return (0);
}

int	fill_browser64(t_nm_browser *browser)
{
	struct mach_header_64 *header;
	struct load_command *lc;
	struct symtab_command *sym;
	uint64_t i;

	header = browser->header_union.header64;
	i = (uint64_t)(browser->ptr + sizeof(*header));
	while (i < (uint64_t)(browser->ptr + header->sizeofcmds))
	{
		lc = (struct load_command*) i;
		if (lc->cmd == LC_SYMTAB)
		{
			sym = (struct symtab_command *)lc;
			if (process_fill_symbols64(browser, sym->nsyms, sym->symoff, sym->stroff))
				return (1);
		}
		i += lc->cmdsize;
	}
	return (0);
}

int nm_perror(char *error_message, t_nm_browser *browser)
{
	ft_printf("%s: %s\n", browser->filename, error_message);
	return (0);
}

int	fill_browser(t_nm_browser *browser)
{
	get_header(browser);
	if (browser->type == E_UNKNOWN)
		return (nm_perror("unknown magic file", browser));
	else if (browser->type == E_64)
		return (fill_browser64(browser));
//	else if (browser->type == E_32)
//		return (fill_browser32());
	return (0);
}

int	init_browser(t_nm_browser *browser, char *filename)
{
	int			fd;

	browser->filename = filename;
	if ((fd = open(filename, O_RDONLY)) < 0)
	{
		ft_dprintf(2, "error opening %s\n", filename);
		return (1);
	}
	if ((fstat(fd, &browser->st)) < 0)
	{
		ft_printf("error fstat\n");
		return (1);
	}
	if ((browser->ptr = mmap(0, browser->st.st_size,
		PROT_READ, MAP_PRIVATE, fd, 0)) == MAP_FAILED)
	{
		ft_printf("ERROR MMAP\n");
		return (1);
	}
	return (0);
}

int		process_nm(char *filename, t_nm_browser *browser)
{
	if (init_browser(browser, filename))
		return (1);
	if (fill_browser(browser))
		return (1);
	nm_print(browser);
	if (munmap(browser->ptr, browser->st.st_size) < 0)
	{
		ft_printf("ERROR MUNMAP\n");
		return (1);
	}
	ft_printf("OK !\n");
	return (0);
}

void	init_browser_general(t_nm_browser *browser)
{
	browser->ret = 0;
	browser->sort_func = cmp_symbol_alpha;
	browser->sort_mult = 1;
	browser->symbols = NULL;
}

int		parse_options(int ac, char **av, t_nm_browser *browser)
{
	(void)ac;
	(void)av;
	(void)browser;
	return (0);
}

int		main(int ac, char **av)
{
	t_nm_browser	browser;
	int				i;
	int				ret;

	init_browser_general(&browser);
	i = 1;
	while (i < ac && (ret = parse_options(ac, av, &browser)))
	{
		if (ret == -1)
			return (EXIT_FAILURE);
		i++;
	}
	if (i == ac)
	{
		if (process_nm(DEFAULT_NM_FILE, &browser))
			return (EXIT_FAILURE);
		return (browser.ret);
	}
	while (i < ac)
	{
		if (process_nm(av[i++], &browser))
			return (EXIT_FAILURE);
	}
	return (browser.ret);
}
