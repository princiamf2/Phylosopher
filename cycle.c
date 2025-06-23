/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cycle.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mm-furi <mm-furi@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/17 23:15:27 by michel            #+#    #+#             */
/*   Updated: 2025/06/23 13:14:39 by mm-furi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "phylosopher.h"

static void	pick_forks(t_philo *ph)
{
	if (ph->args->nb_philo == 1)
		return ;
	else if (ph->id % 2 == 1)
	{
		pthread_mutex_lock(ph->left_fork);
		pthread_mutex_lock(ph->right_fork);
	}
	else
	{
		pthread_mutex_lock(ph->right_fork);
		pthread_mutex_lock(ph->left_fork);
	}
}

static void	drop_forks(t_philo *ph)
{
	if (ph->args->nb_philo == 1)
		pthread_mutex_unlock(ph->left_fork);
	else
	{
		pthread_mutex_unlock(ph->left_fork);
		pthread_mutex_unlock(ph->right_fork);
	}
}

void	eat_action(t_philo *ph)
{
	pick_forks(ph);
	safe_log(ph, "has taken a fork");
	if (ph->args->nb_philo > 1)
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
	if (ph->args->nb_philo == 1)
	{
		ft_sleep(ph->args->time_to_die);
		safe_log(ph, "died");
		ph->stop = 1;
	}
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
