/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: michel <michel@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/17 15:51:17 by michel            #+#    #+#             */
/*   Updated: 2025/05/07 17:02:51 by michel           ###   ########.fr       */
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