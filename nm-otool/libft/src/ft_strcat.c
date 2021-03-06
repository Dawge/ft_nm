/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strcat.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rostroh <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/11/04 14:51:53 by rostroh           #+#    #+#             */
/*   Updated: 2016/11/08 13:36:01 by rostroh          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strcat(char *s1, const char *s2)
{
	char	*cpy;

	cpy = s1;
	while (*s1 != 0)
		s1++;
	while (*s2 != 0)
	{
		*s1 = *s2;
		s1++;
		s2++;
	}
	*s1 = 0;
	return (cpy);
}
