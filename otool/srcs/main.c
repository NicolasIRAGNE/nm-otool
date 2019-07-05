/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: niragne <niragne@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/07/01 14:08:14 by niragne           #+#    #+#             */
/*   Updated: 2019/07/01 16:52:32 by niragne          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_otool.h"

int	process_otool(char *filename)
{
	void *ptr;
	struct stat buf;
	int fd;

	if ((fd = open(filename, O_RDONLY)) < 0)
	{
		ft_dprintf(2, "error opening %s\n", filename);
		return (1);
	}
	if ((fstat(fd, &buf)) < 0)
	{
		ft_printf("error fstat\n");
		return (1);
	}
	if ((ptr = mmap(0, buf.st_size,
		PROT_READ | PROT_WRITE, MAP_PRIVATE, fd, 0)) == MAP_FAILED)
	{
		ft_dprintf(2, "could not map file %s\n", filename);
		return (1);
	}
	dump_mem((unsigned char*)ptr, buf.st_size);
	debug_test();
	return (0);
}


int main(int ac, char **av)
{
	int i;

	i = 1;
	if (ac == 1)
		print_usage(av[0]);
	while (i < ac)
	{
		process_otool(av[i]);
		i++;
	}
	
	return (0);
}
