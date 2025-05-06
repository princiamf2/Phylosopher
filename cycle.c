/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cycle.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: michel <michel@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/17 23:15:27 by michel            #+#    #+#             */
/*   Updated: 2025/03/21 21:51:59 by michel           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "phylosopher.h"

void take_forks(t_data *data, int idx)
{
    if (idx == 0)
    {
        /* Philosophe 1 prend dans l'ordre inverse :
           d'abord la fourchette droite, ensuite la fourchette gauche */
        pthread_mutex_lock(&data->mutex[(idx + 1) % data->number_philo]);
        pthread_mutex_lock(&data->print);
        printf("%ld %ld has taken a fork\n", get_timestamp(), data->philo[idx].id);
        pthread_mutex_unlock(&data->print);

        pthread_mutex_lock(&data->mutex[idx]);
        pthread_mutex_lock(&data->print);
        printf("%ld %ld has taken a fork\n", get_timestamp(), data->philo[idx].id);
        pthread_mutex_unlock(&data->print);
    }
    else if (idx % 2 == 0)
    {
        /* Philosophe pair (autre que 1) : prend d'abord la fourchette gauche */
        pthread_mutex_lock(&data->mutex[idx]);
        pthread_mutex_lock(&data->print);
        printf("%ld %ld has taken a fork\n", get_timestamp(), data->philo[idx].id);
        pthread_mutex_unlock(&data->print);

        pthread_mutex_lock(&data->mutex[(idx + 1) % data->number_philo]);
        pthread_mutex_lock(&data->print);
        printf("%ld %ld has taken a fork\n", get_timestamp(), data->philo[idx].id);
        pthread_mutex_unlock(&data->print);
    }
    else
    {
        /* Philosophe impair : prend d'abord la fourchette droite */
        pthread_mutex_lock(&data->mutex[(idx + 1) % data->number_philo]);
        pthread_mutex_lock(&data->print);
        printf("%ld %ld has taken a fork\n", get_timestamp(), data->philo[idx].id);
        pthread_mutex_unlock(&data->print);

        pthread_mutex_lock(&data->mutex[idx]);
        pthread_mutex_lock(&data->print);
        printf("%ld %ld has taken a fork\n", get_timestamp(), data->philo[idx].id);
        pthread_mutex_unlock(&data->print);
    }
}

void    eat(t_data *data, int idx)
{
    pthread_mutex_lock(&data->print);
    printf("%ld %ld is eating\n", get_timestamp(), data->philo[idx].id);
    pthread_mutex_unlock(&data->print);
    data->philo[idx].last_meal_time = get_timestamp();
    ft_sleep(data->philo[idx].time_for_eat);
    data->philo[idx].eat_count++;
}

void sleep_and_think(t_data *data, int idx)
{
    pthread_mutex_lock(&data->print);
    printf("%ld %ld is sleeping\n", get_timestamp(), data->philo[idx].id);
    pthread_mutex_unlock(&data->print);
    ft_sleep(data->philo[idx].time_for_sleep);
    pthread_mutex_lock(&data->print);
    printf("%ld %ld is thinking\n", get_timestamp(), data->philo[idx].id);
    pthread_mutex_unlock(&data->print);
}

void *monitor_routine(void *arg)
{
    t_data *data;
    int i;
    int all_full;

    data = (t_data *)arg;
    while (1)
    {
        i = 0;
        all_full = 1;
        while (i < data->number_philo)
        {
            if ((get_timestamp() - data->philo[i].last_meal_time) > data->philo[i].time_for_die)
            {
                pthread_mutex_lock(&data->print);
                printf("%ld %ld died\n", get_timestamp(), data->philo[i].id);
                pthread_mutex_unlock(&data->print);
                exit(0);
            }
            if (data->philo[i].eat_count < 5)
                all_full = 0;
            i++;
        }
        if (all_full)
        {
            pthread_mutex_lock(&data->print);
            printf("%ld All philosophers have eaten at least 5 times\n", get_timestamp());
            pthread_mutex_unlock(&data->print);
            return (NULL);
        }
        usleep(1000);  // vérifie toutes les 1ms
    }
    return (NULL);
}
