/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sem_helper.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mm-furi <mm-furi@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/24 19:08:58 by mm-furi           #+#    #+#             */
/*   Updated: 2025/08/04 15:28:00 by mm-furi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "phylosopher_bonus.h"

int	open_forks(t_data2 *data)
{
	int		i;
	char	name[16];

	i = 0;
	while (i < data->args.nb_philo)
	{
		build_fork_name(i, name);
		sem_unlink(name);
		data->forks[i] = sem_open(name, O_CREAT | O_EXCL, 0644, 1);
		if (data->forks[i] == SEM_FAILED)
		{
			while (i-- > 0)
			{
				build_fork_name(i, name);
				sem_close(data->forks[i]);
				sem_unlink(name);
			}
			return (error_and_exit("sem_open fork", data));
		}
		i++;
	}
	return (0);
}

void	single_philo_behavior(t_data2 *data, int id)
{
	long	start;

	sem_wait(data->forks[id]);
	safe_print(data, id + 1, "has taken a fork");
	start = get_timestamp();
	while (get_timestamp() - start < data->args.time_to_die)
		usleep(1000);
	sem_wait(data->death_sem);
	safe_print(data, id + 1, "died");
	sem_post(data->death_sem);
	sem_post(data->forks[id]);
	exit(EXIT_FAILURE);
}

void	drop_forks(t_data2 *data, int id)
{
	int	left;
	int	right;

	left = (id + data->args.nb_philo - 1) % data->args.nb_philo;
	right = id;
	sem_post(data->forks[left]);
	sem_post(data->forks[right]);
	sem_post(data->limit);
}

void	take_forks(t_data2 *data, int id)
{
	int	left;
	int	right;

	left = (id + data->args.nb_philo - 1) % data->args.nb_philo;
	right = id;
	sem_wait(data->limit);
	if (id % 2 == 0)
	{
		sem_wait(data->forks[right]);
		sem_wait(data->forks[left]);
	}
	else
	{
		sem_wait(data->forks[left]);
		sem_wait(data->forks[right]);
	}
	safe_print(data, id + 1, "has taken a fork");
	safe_print(data, id + 1, "has taken a fork");
}

void	*death_monitor(void *arg)
{
	t_monitor	*moni;
	t_args2		*args;

	moni = arg;
	args = &moni->data->args;
	while (1)
	{
		if (get_timestamp() - *(moni->last_meal) > args->time_to_die)
		{
			if (*(moni->last_meal) == -1)
			{
				free(moni);
				exit(EXIT_SUCCESS);
			}
			sem_wait(moni->data->death_sem);
			safe_print(moni->data, moni->id + 1, "died");
			sem_post(moni->data->death_sem);
			free(moni);
			exit(EXIT_FAILURE);
		}
		usleep(1000);
	}
	return (NULL);
}
