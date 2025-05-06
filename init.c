/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: michel <michel@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/17 16:17:00 by michel            #+#    #+#             */
/*   Updated: 2025/03/17 23:50:48 by michel           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "phylosopher.h"

int init_print_mutex(t_data *data)
{
    if (pthread_mutex_init(&data->print, NULL) != 0)
        return (1);
    return (0);
}

int init_mutex(t_data *data)
{
    int i;
    
    i = 0;
    while (i < data->number_philo)
    {
        if (pthread_mutex_init(&data->mutex[i], NULL) != 0)
            return (i + 1);
        i++;
    }
    return (0);
}

void init_philo(t_data *data, int *times)
{
    int i;
    
    i = 0;
    while (i < data->number_philo)
    {
        data->philo[i].id = i + 1;
        data->philo[i].time_for_die = times[0];
        data->philo[i].time_for_eat = times[1];
        data->philo[i].time_for_sleep = times[2];
        data->philo[i].time_for_think = times[3];
        data->philo[i].eat_count = 0;
        data->philo[i].last_meal_time = get_timestamp();
        i++;
    }
}

t_data *init_data(int nb_philo, int *times)
{
    t_data *data;
    int err;

    data = allocate_data(nb_philo);
    if (!data)
        return (NULL);
    if (init_print_mutex(data))
    {
        free(data->mutex);
        free(data->philo);
        free(data);
        return (NULL);
    }
    err = init_mutex(data);
    if (err)
        return (clean_data(data, err));
    init_philo(data, times);
    return (data);
}