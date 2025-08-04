/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mm-furi <mm-furi@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/24 19:10:09 by mm-furi           #+#    #+#             */
/*   Updated: 2025/08/01 15:20:30 by mm-furi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "phylosopher_bonus.h"

long	get_timestamp(void)
{
	struct timeval	tv;

	gettimeofday(&tv, NULL);
	return (tv.tv_sec * 1000L + tv.tv_usec / 1000L);
}

void	safe_print(t_data2 *data, int id, const char *msg)
{
	long	now;

	sem_wait(data->print);
	now = get_timestamp() - data->start_time;
	printf("%ld %d %s\n", now, id, msg);
	sem_post(data->print);
}

int	ft_atoi(const char *s)
{
	int		i;
	long	ret;
	int		sign;

	i = 0;
	ret = 0;
	sign = 1;
	while (s[i] == ' ' || (s[i] >= 9 && s[i] <= 13))
		i++;
	if (s[i] == '-' || s[i] == '+')
	{
		if (s[i] == '-')
			sign = -1;
		i++;
	}
	while (s[i] >= '0' && s[i] <= '9')
	{
		ret = ret * 10 + (s[i] - '0');
		i++;
	}
	return ((int)ret * sign);
}

void	ft_putstr_fd(char *str, int fd)
{
	int	i;

	i = 0;
	while (str[i])
	{
		write(fd, &str[i], 1);
		i++;
	}
	write(fd, "\n", 1);
}

void	itoa_min(int n, char *s)
{
	int	tmp;
	int	len;
	int	orig;

	tmp = n;
	len = 0;
	if (n == 0)
		len = 1;
	while (tmp > 0)
	{
		len++;
		tmp /= 10;
	}
	orig = len;
	while (len-- > 0)
	{
		s[len] = (n % 10) + '0';
		n /= 10;
	}
	s[orig] = '\0';
}
