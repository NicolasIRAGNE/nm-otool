/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: niragne <niragne@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/06/19 13:02:13 by niragne           #+#    #+#             */
/*   Updated: 2019/06/19 14:04:48 by niragne          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_nm.h"

void	get_header(char *ptr, t_nm_browser *browser)
{
	uint32_t magic;
	magic = *(uint32_t*)ptr;
	(void)browser;
	if (magic == MH_MAGIC_64)
	{
		ft_printf("Executable 64 bits\n");
		browser->type = E_64;
		browser->header = (struct mach_header_64*)ptr;
	}
	else if (magic == MH_MAGIC)
	{
		ft_printf("Executable 32 bits\n");
		browser->type = E_32;
	}
	else if (magic == FAT_MAGIC)
	{
		ft_printf("Executable fat 32 bits\n");
		browser->type = E_FAT32;
	}
	else if (magic == FAT_MAGIC_64)
	{
		ft_printf("Executable fat 64 bits\n");
		browser->type = E_FAT64;
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
