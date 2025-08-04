/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cycle.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mm-furi <mm-furi@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/17 23:15:27 by michel            #+#    #+#             */
/*   Updated: 2025/06/24 15:38:59 by mm-furi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "phylosopher.h"

static bool	pick_forks(t_philo *ph)
{
	pthread_mutex_lock(ph->left_fork);
	if (ph->data->stop)
	{
		pthread_mutex_unlock(ph->left_fork);
		return (false);
	}
	pthread_mutex_lock(ph->right_fork);
	if (ph->data->stop)
	{
		pthread_mutex_unlock(ph->right_fork);
		pthread_mutex_unlock(ph->left_fork);
		return (false);
	}
	return (true);
}

static void	drop_forks(t_philo *ph)
{
	pthread_mutex_unlock(ph->left_fork);
	if (ph->args->nb_philo > 1)
		pthread_mutex_unlock(ph->right_fork);
}

void	eat_action(t_philo *ph)
{
	if (ph->args->nb_philo == 1)
	{
		one_philo(ph);
		return ;
	}
	if (ph->data->stop)
		return ;
	if (!pick_forks(ph))
		return ;
	safe_log(ph, "has taken a fork");
	safe_log(ph, "has taken a fork");
	pthread_mutex_lock(&ph->meal_mutex);
	ph->last_meal = get_timestamp();
	pthread_mutex_unlock(&ph->meal_mutex);
	if (ph->data->stop)
	{
		drop_forks(ph);
		return ;
	}
	safe_log(ph, "is eating");
	stoppable_sleep(ph, ph->args->time_to_eat);
	pthread_mutex_lock(&ph->meal_mutex);
	ph->eats++;
	pthread_mutex_unlock(&ph->meal_mutex);
	drop_forks(ph);
}

void	sleep_action(t_philo *ph)
{
	if (ph->data->stop == 1)
		return ;
	safe_log(ph, "is sleeping");
	stoppable_sleep(ph, ph->args->time_to_sleep);
}

void	think_action(t_philo *ph)
{
	if (ph->data->stop)
		return ;
	safe_log(ph, "is thinking");
}
