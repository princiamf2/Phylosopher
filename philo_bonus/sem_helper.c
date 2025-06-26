/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sem_helper.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mm-furi <mm-furi@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/24 19:08:58 by mm-furi           #+#    #+#             */
/*   Updated: 2025/06/26 18:00:52 by mm-furi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "phylosopher_bonus.h"

sem_t	*open_semaphores(t_data2 *data, int n)
{
	sem_unlink("/forks");
	sem_unlink("/print");
	sem_unlink("/philo_death");
	data->forks = sem_open("/forks", O_CREAT | O_EXCL, 0644, n);
	data->print = sem_open("/print", O_CREAT | O_EXCL, 0644, 1);
	data->death_sem = sem_open("/philo_death", O_CREAT | O_EXCL, 0644, 1);
	if (data->forks == SEM_FAILED || data->print == SEM_FAILED
		|| data->death_sem == SEM_FAILED)
	{
		perror("sem_open");
		exit(EXIT_FAILURE);
	}
	return (data->forks);
}

void	close_semaphores(t_data2 *data)
{
	sem_close(data->forks);
	sem_unlink("/philo_forks");
	sem_close(data->print);
	sem_unlink("/philo_writing");
	sem_close(data->death_sem);
	sem_unlink("/philo_death");
}

void	drop_forks(t_data2 *data)
{
	sem_post(data->forks);
	sem_post(data->forks);
}

void	take_forks(t_data2 *data)
{
	sem_wait(data->forks);
	sem_wait(data->forks);
}

void	*death_monitor(void *arg)
{
	t_monitor	*moni;
	t_args2		*args;

	// int	i;
	moni = arg;
	args = &moni->data->args;
	while (1)
	{
		if (get_timestamp() - *(moni->last_meal) > args->time_to_die)
		{
			// i = 0;
			// while (i < args->nb_philo)
			// {
			// 	if (moni->data->pids[i] != getpid())
			// 		kill(moni->data->pids[i], SIGTERM);
			// 	i++;
			// }
			sem_wait(moni->data->death_sem);
			safe_print(moni->data, moni->id, "died");
			free(moni);
			exit(EXIT_FAILURE);
		}
		usleep(1000);
	}
	return (NULL);
}
