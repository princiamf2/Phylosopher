/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   routine.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mm-furi <mm-furi@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/07 17:12:31 by michel            #+#    #+#             */
/*   Updated: 2025/06/04 15:33:12 by mm-furi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "phylosopher.h"

void	*philo_routine(void *arg)
{
	t_philo *ph;

	ph = (t_philo *)arg;
	if (ph->id % 2 == 0)
		ft_sleep(ph->args->time_to_eat / 10);
	while (!*(ph->stop))
	{
		eat_action(ph);
		if (ph->args->meals_required > 0 && ph->eats >= ph->args->meals_required)
			break;
		sleep_action(ph);
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
	{
		safe_log(&data->philos[i], "died");
		pthread_mutex_unlock(&data->philos[i].meal_mutex);
		return (1);
	}
	pthread_mutex_unlock(&data->philos[i].meal_mutex);
	return (0);
}

int	check_finished(t_data *data, int i, int *finished)
{
	pthread_mutex_lock(&data->philos[i].meal_mutex);
	if (data->args.meals_required > 0 \
	&& data->philos[i].eats >= data->args.meals_required)
		(*finished)++;
	pthread_mutex_unlock(&data->philos[i].meal_mutex);
	return (0);
}

void	*monitor_routine(void *arg)
{
	t_data *data;
	int	 finished;
	int	 i;

	data = (t_data *)arg;
	while (!data->stop)
	{
		finished = 0;
		i = 0;
		while (i < data->args.nb_philo)
		{
			if (check_death(data, i))
				return (NULL);
			check_finished(data, i, &finished);
			i++;
		}
		if (data->args.meals_required > 0 && finished == data->args.nb_philo)
		{
			data->stop = 1;
			return (NULL);
		}
		usleep(1000);
	}
	return (NULL);
}
