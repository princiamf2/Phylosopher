/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: michel <michel@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/17 15:51:17 by michel            #+#    #+#             */
/*   Updated: 2025/03/17 23:30:06 by michel           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "phylosopher.h"
#include <bits/types/struct_timeval.h>
#include <sys/time.h>
#include <unistd.h>

t_data *allocate_data(int nb_philo)
{
    t_data *data;

    data = malloc(sizeof(t_data));
    if (!data)
        return (NULL);
    data->number_philo = nb_philo;
    data->philo = malloc(sizeof(t_philo) * nb_philo);
    if (!data->philo)
    {
        free(data);
        return (NULL);
    }
    data->mutex = malloc(sizeof(pthread_mutex_t) * nb_philo);
    if (!data->mutex)
    {
        free(data->philo);
        free(data);
        return (NULL);
    }
    return (data);
}

t_data *clean_data(t_data *data, int err)
{
    int i;
    
    i = 0;
    while (i < err -1)
    {
        pthread_mutex_destroy(&data->mutex[i]);
        i++;
    }
    pthread_mutex_destroy(&data->print);
    free(data->mutex);
    free(data->philo);
    free(data);
    return (NULL);
}

long    get_timestamp(void)
{
    struct timeval tv;
    gettimeofday(&tv, NULL);
    return (tv.tv_sec * 1000L + tv.tv_usec / 1000L);
}

void    ft_sleep(int ms)
{
    long    start;
    
    start = get_timestamp();
    while (get_timestamp() - start < ms)
        usleep(100);
}

void release_forks(t_data *data, int idx)
{
    pthread_mutex_unlock(&data->mutex[idx]);
    pthread_mutex_unlock(&data->mutex[(idx + 1) % data->number_philo]);
}
