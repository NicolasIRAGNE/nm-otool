/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   macho_perror.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: niragne <niragne@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/08/13 18:03:53 by niragne           #+#    #+#             */
/*   Updated: 2019/08/13 18:38:21 by niragne          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "mach_o.h"

int		macho_perror(int error, t_browser *browser)
{
	if (error == ERROR_CORRUPTED)
		ft_dprintf(2, "%s: %s: truncated or malformed object (load commands"
					" extend past the end of the file)\n", browser->progname,
					browser->filename);
	return (error);
}
