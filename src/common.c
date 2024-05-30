/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   common.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ybolivar <ybolivar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/16 14:40:19 by aherbin           #+#    #+#             */
/*   Updated: 2024/05/30 13:21:13 by ybolivar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minitalk.h"

int	ft_atoi(char *str)
{
	int	nb;
	int	sign;
	int	i;

	i = 0;
	nb = 0;
	sign = 1;
	while ((str[i] >= 9 && str[i] <= 13) || str[i] == ' ')
		++i;
	if (str[i] == '-')
	{
		sign = -1;
		++i;
	}
	else if (str[i] == '+')
		++i;
	while (str[i] >= '0' && str[i] <= '9')
		nb = nb * 10 + str[i++] - '0';
	return ((int)nb * sign);
}

int	ft_isdigit(int c)
{
	return (c <= '9' && c >= '0');
}

void	ft_putnbr_fd(pid_t nb, int fd)
{
	char		c;

	if (nb == -2147483647)
		write(fd, "-2147483647", 11);
	else if (nb == -2147483648)
		write(fd, "-2147483648", 11);
	else
	{
		if (nb < 0)
		{
			nb *= -1;
			write(fd, "-", 1);
		}
		if (nb > 9)
			ft_putnbr_fd(nb / 10, fd);
		c = '0' + nb % 10;
		write(fd, &c, 1);
	}
}

int	is_pid(char *spid)
{
	int	i;

	i = 0;
	while (spid[i])
	{
		if (!ft_isdigit(spid[i]))
			return (0);
		++i;
	}
	return (1);
}

/**
 * @brief Write a provided error message
 *
 * @param exit_code error message
 */
void	exit_on_error(char *exit_code)
{
	char	*p;

	p = exit_code;
	while (*p)
		++p;
	write(1, exit_code, p - exit_code);
	write(1, "\n", 1);
	exit(0);
}
