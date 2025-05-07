/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cycle.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: michel <michel@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/17 23:15:27 by michel            #+#    #+#             */
/*   Updated: 2025/05/07 17:09:36 by michel           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "phylosopher.h"

static void	pick_forks(t_philo *ph)
{
	if (ph->id == ph->args->nb_philo)
	{
		pthread_mutex_lock(ph->right_fork);
		pthread_mutex_lock(ph->left_fork);
	}
	else
	{
		pthread_mutex_lock(ph->left_fork);
		pthread_mutex_lock(ph->right_fork);
	}
}

static void	drop_forks(t_philo *ph)
{
	pthread_mutex_unlock(ph->left_fork);
	pthread_mutex_unlock(ph->right_fork);
}

void	eat_action(t_philo *ph)
{
	pick_forks(ph);
	safe_log(ph, "has taken a fork");
	safe_log(ph, "has taken a fork");
	pthread_mutex_lock(&ph->meal_mutex);
	ph->last_meal = get_timestamp();
	pthread_mutex_unlock(&ph->meal_mutex);
	safe_log(ph, "is eating");
	ft_sleep(ph->args->time_to_eat);
	pthread_mutex_lock(&ph->meal_mutex);
	ph->eats++;
	pthread_mutex_unlock(&ph->meal_mutex);
	drop_forks(ph);
}

void	sleep_action(t_philo *ph)
{
	safe_log(ph, "is sleeping");
	ft_sleep(ph->args->time_to_sleep);
}

void	think_action(t_philo *ph)
{
	safe_log(ph, "is thinking");
}