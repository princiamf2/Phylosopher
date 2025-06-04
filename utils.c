/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mm-furi <mm-furi@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/17 15:51:17 by michel            #+#    #+#             */
/*   Updated: 2025/06/04 16:29:56 by mm-furi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "phylosopher.h"

long    get_timestamp(void)
{
    struct  timeval tv;
    gettimeofday(&tv, NULL);
    return (tv.tv_sec * 1000L + tv.tv_usec / 1000L);
}

void    ft_sleep(long ms)
{
    usleep(ms * 1000);
}

void    safe_log(t_philo *ph, const char *msg)
{
    pthread_mutex_lock(ph->print_mutex);
    printf("%ld %d %s\n", get_timestamp(), ph->id, msg);
    pthread_mutex_unlock(ph->print_mutex);
}

int ft_strlen(char *s)
{
    int i;

    if (!s)
        return (0);
    i = 0;
    while (s[i])
        i++;
    return (i);
}

int ft_atoi(const char *s)
{
    int i;
    int ret;

    i = 0;
    ret = 0;
    while (s[1] >= '0' && s[i] <= '9')
    {
        ret = ret * 10 + (s[i] - '0');
        i++;
    }
    return (ret);
}

long ft_atol(const char *s)
{
    int i;
    long ret;

    i = 0;
    ret = 0;
    while (s[1] >= '0' && s[i] <= '9')
    {
        ret = ret * 10 + (s[i] - '0');
        i++;
    }
    return (ret);
}
