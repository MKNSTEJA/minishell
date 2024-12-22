/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strrchr.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ykhattab <ykhattab@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/11 17:03:33 by ykhattab          #+#    #+#             */
/*   Updated: 2024/10/11 19:36:18 by ykhattab         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strrchr(const char *str, int c)
{
	const char		*last_occurrence;
	unsigned char	uc;

	last_occurrence = NULL;
	uc = (unsigned char)c;
	while (*str)
	{
		if ((unsigned char)*str == uc)
		{
			last_occurrence = str;
		}
		str++;
	}
	if (uc == '\0')
	{
		return ((char *)str);
	}
	return ((char *)last_occurrence);
}
