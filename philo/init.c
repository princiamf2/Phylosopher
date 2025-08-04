/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mm-furi <mm-furi@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/17 16:17:00 by michel            #+#    #+#             */
/*   Updated: 2025/07/02 17:08:40 by mm-furi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "phylosopher.h"

static void	init_philos(t_data *data)
{
	int		i;
	t_philo	*ph;

	i = 0;
	while (i < data->args.nb_philo)
	{
		ph = &data->philos[i];
		ph->id = i + 1;
		ph->args = &data->args;
		ph->data = data;
		ph->left_fork = &data->forks[i];
		ph->right_fork = &data->forks[(i + 1) % data->args.nb_philo];
		ph->print_mutex = &data->print_mutex;
		ph->last_meal = get_timestamp();
		ph->start_time = data->start_time;
		ph->eats = 0;
		pthread_mutex_init(&ph->meal_mutex, NULL);
		i++;
	}
}

static int	parse_args(t_data *data, int argc, char **argv)
{
	if (argc < 5 || argc > 6)
	{
		printf(
			"Usage: %s nb_philo time_to_die time_to_eat "
			"time_to_sleep [meals_required]",
			argv[0]);
		return (1);
	}
	data->args.nb_philo = ft_atoi(argv[1]);
	data->args.time_to_die = ft_atoi(argv[2]);
	data->args.time_to_eat = ft_atoi(argv[3]);
	data->args.time_to_sleep = ft_atoi(argv[4]);
	data->start_time = get_timestamp();
	data->stop = 0;
	if (argc == 6)
		data->args.meals_required = ft_atoi(argv[5]);
	else
		data->args.meals_required = -1;
	return (0);
}

static int	setup_resources(t_data *data)
{
	int	n;
	int	i;

	n = data->args.nb_philo;
	data->forks = malloc(sizeof(pthread_mutex_t) * n);
	data->threads = malloc(sizeof(pthread_t) * n);
	data->philos = malloc(sizeof(t_philo) * n);
	if (!data->forks || !data->threads || !data->philos)
		return (1);
	i = 0;
	while (i < data->args.nb_philo)
		pthread_mutex_init(&data->forks[i++], NULL);
	pthread_mutex_init(&data->print_mutex, NULL);
	return (0);
}

int	init_data(t_data *data, int argc, char **argv)
{
	if (parse_args(data, argc, argv))
		return (1);
	data->start_time = get_timestamp();
	data->stop = 0;
	if (setup_resources(data))
		return (1);
	init_philos(data);
	return (0);
}

void	cleanup_data(t_data *data)
{
	int	i;

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
