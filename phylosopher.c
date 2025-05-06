/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   phylosopher.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: michel <michel@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/17 13:50:27 by michel            #+#    #+#             */
/*   Updated: 2025/03/21 21:19:42 by michel           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "phylosopher.h"


void    *routine_threads(void *arg)
{
    t_philo_arg *p_arg;
    t_data      *data;
    int         idx;

    p_arg = (t_philo_arg *)arg;
    data = p_arg->data;
    idx = p_arg->index;
    /* Petit délai pour les philosophes pairs */
    if (idx % 2 == 0)
        ft_sleep(10);  // 10ms de délai

    while (1)
    {
        take_forks(data, idx);
        eat(data, idx);
        release_forks(data, idx);
        sleep_and_think(data, idx);
    }
    return (NULL);
}

int main(int argc, char **argv)
{
    int         nb_philo;
    int         times[4];
    int         i;
    t_data      *data;
    pthread_t   *threads;
    pthread_t   monitor;
    t_philo_arg *args;

    (void)argc;
    (void)argv;
    nb_philo = 5;
    times[0] = 800;  // time_for_die
    times[1] = 200;  // time_for_eat
    times[2] = 200;  // time_for_sleep
    times[3] = 100;  // time_for_think

    data = init_data(nb_philo, times);
    if (!data)
        return (EXIT_FAILURE);
    threads = malloc(sizeof(pthread_t) * nb_philo);
    if (!threads)
        return (EXIT_FAILURE);
    args = malloc(sizeof(t_philo_arg) * nb_philo);
    if (!args)
    {
        free(threads);
        return (EXIT_FAILURE);
    }
    i = 0;
    while (i < nb_philo)
    {
        args[i].index = i;
        args[i].data = data;
        if (pthread_create(&threads[i], NULL, routine_threads, &args[i]) != 0)
        {
            perror("Erreur lors de la création du thread");
            return (EXIT_FAILURE);
        }
        i++;
    }
    if (pthread_create(&monitor, NULL, monitor_routine, data) != 0)
    {
        perror("Erreur lors de la création du thread de surveillance");
        return (EXIT_FAILURE);
    }
    i = 0;
    while (i < nb_philo)
    {
        pthread_join(threads[i], NULL);
        i++;
    }
    free(threads);
    free(args);
    clean_data(data, nb_philo);
    return (0);
}
