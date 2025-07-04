/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   routine.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mm-furi <mm-furi@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/07 17:12:31 by michel            #+#    #+#             */
/*   Updated: 2025/06/24 15:35:09 by mm-furi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "phylosopher.h"

void	*philo_routine(void *arg)
{
	t_philo	*ph;

	ph = (t_philo *)arg;
	while (!ph->data->stop)
	{
		eat_action(ph);
		if (ph->data->stop || (ph->args->meals_required > 0
				&& ph->eats >= ph->args->meals_required))
			break ;
		if (ph->data->stop)
			break ;
		sleep_action(ph);
		if (ph->data->stop)
			break ;
		think_action(ph);
	}
	return (NULL);
}

int	check_death(t_data *data, int i)
{
	long	now;

	pthread_mutex_lock(&data->philos[i].meal_mutex);
	now = get_timestamp();
	if (now - data->philos[i].last_meal > data->args.time_to_die)
		return (1);
	pthread_mutex_unlock(&data->philos[i].meal_mutex);
	return (0);
}

int	check_finished(t_data *data, int i, int *finished)
{
	pthread_mutex_lock(&data->philos[i].meal_mutex);
	if (data->args.meals_required > 0
		&& data->philos[i].eats >= data->args.meals_required)
		(*finished)++;
	pthread_mutex_unlock(&data->philos[i].meal_mutex);
	return (0);
}

static int	monitor_cycle(t_data *data)
{
	int		done;
	int		i;
	t_philo	*ph;

	done = 0;
	i = 0;
	while (i < data->args.nb_philo)
	{
		ph = &data->philos[i];
		if (check_death(data, i))
		{
			safe_log(ph, "died");
			data->stop = 1;
			pthread_mutex_unlock(&ph->meal_mutex);
			break ;
		}
		else
			pthread_mutex_unlock(&ph->meal_mutex);
		check_finished(data, i, &done);
		i++;
	}
	if (data->args.meals_required > 0 && done == data->args.nb_philo)
		data->stop = 1;
	return (data->stop);
}

void	*monitor_routine(void *arg)
{
	t_data	*data;

	data = arg;
	if (data->stop == 1)
		return (NULL);
	while (data->stop != 1)
	{
		monitor_cycle(data);
		if (data->stop == 1)
			break ;
		usleep(1000);
	}
	return (NULL);
}
