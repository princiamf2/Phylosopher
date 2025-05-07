/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: michel <michel@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/17 16:17:00 by michel            #+#    #+#             */
/*   Updated: 2025/05/07 18:39:28 by michel           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "phylosopher.h"

static void	init_philos(t_data *data)
{
	int			i;
	t_philo		*ph;

	i = 0;
	while (i < data->args.nb_philo)
	{
		ph = &data->philos[i];
		ph->id          = i + 1;
		ph->args        = &data->args;
		ph->stop        = &data->stop;
		ph->left_fork   = &data->forks[i];
		ph->right_fork  = &data->forks[(i + 1) % data->args.nb_philo];
		ph->print_mutex = &data->print_mutex;
		ph->last_meal   = get_timestamp();
		ph->eats        = 0;
		pthread_mutex_init(&ph->meal_mutex, NULL);
		i++;
	}
}

int	init_data(t_data *data, int argc, char **argv)
{
	int i;

	if (argc < 5 || argc > 6)
	{
		fprintf(stderr, "Usage: %s nb_philo time_to_die "
			"time_to_eat time_to_sleep [meals_required]\n", argv[0]);
		return (1);
	}
	data->args.nb_philo = atoi(argv[1]);
	data->args.time_to_die = atol(argv[2]);
	data->args.time_to_eat = atol(argv[3]);
	data->args.time_to_sleep = atol(argv[4]);
	data->args.meals_required = (argc == 6) ? atoi(argv[5]) : 0;
	data->stop = 0;
	i = 0;
	data->forks = malloc(sizeof(pthread_mutex_t) * data->args.nb_philo);
	data->threads = malloc(sizeof(pthread_t) * data->args.nb_philo);
	data->philos = malloc(sizeof(t_philo) * data->args.nb_philo);
	if (!data->forks || !data->threads || !data->philos)
		return (1);
	while (i < data->args.nb_philo)
		pthread_mutex_init(&data->forks[i++], NULL);
	pthread_mutex_init(&data->print_mutex, NULL);
	init_philos(data);
	return (0);
}

void	cleanup_data(t_data *data)
{
	int i;

	i = 0;
	while (i < data->args.nb_philo)
		pthread_mutex_destroy(&data->forks[i++]);
	i = 0;
	while (i < data->args.nb_philo)
		pthread_mutex_destroy(&data->philos[i++].meal_mutex);
	pthread_mutex_destroy(&data->print_mutex);
	free(data->forks);
	free(data->threads);
	free(data->philos);
}