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
	return (0);
}

int		main(int ac, char **av)
{
	t_nm_browser	browser;
	int				i;
	int				ret;

	init_browser_general(&browser);
	if ((ret = parse_options(&i, ac, av, &browser)))
		return (ret == 1 ? 1 : 0);
	if (browser.ret)
		return (browser.ret);
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
