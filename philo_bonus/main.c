/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mm-furi <mm-furi@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/24 19:10:02 by mm-furi           #+#    #+#             */
/*   Updated: 2025/08/04 15:08:43 by mm-furi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "phylosopher_bonus.h"

int	init_data2(t_data2 *data, pid_t **pids, int ac, char **av)
{
	if (check_argc(ac, av) < 0)
		return (EXIT_FAILURE);
	if (parse_args(data, ac, av) < 0)
		return (EXIT_FAILURE);
	if (setup_resources(data, pids) < 0)
		return (EXIT_FAILURE);
	data->start_time = get_timestamp();
	return (EXIT_SUCCESS);
}

void	spawn_philo(t_data2 *data, pid_t *pids)
{
	int	i;

	i = 0;
	while (i < data->args.nb_philo)
	{
		pids[i] = fork();
		if (pids[i] < 0)
		{
			kill_all(pids, i);
			wait_all(pids, i);
			ft_putstr_fd("fork", 2);
			return ;
		}
		if (pids[i] == 0)
		{
			routine_philo(data, i);
			exit(EXIT_SUCCESS);
		}
		i++;
	}
}

static int	wait_for_children(t_data2 *data)
{
	int	finished;
	int	failure;
	int	result;

	failure = count_success_and_detect_failure(data, &finished);
	if (failure)
		result = 1;
	else if (finished == data->args.nb_philo)
		result = 2;
	else
		result = 0;
	return (result);
}

void	cleanup_resources(t_data2 *data)
{
	int		i;
	char	name[16];

	i = 0;
	while (i < data->args.nb_philo)
	{
		build_fork_name(i, name);
		if (data->forks[i] != SEM_FAILED)
			sem_close(data->forks[i]);
		sem_unlink(name);
		i++;
	}
	if (data->print != SEM_FAILED)
	{
		sem_close(data->print);
		sem_unlink("/print");
	}
	if (data->death_sem != SEM_FAILED)
	{
		sem_close(data->death_sem);
		sem_unlink("/philo_death");
	}
	if (data->limit != SEM_FAILED)
	{
		sem_close(data->limit);
		sem_unlink("/limit");
	}
}

int	main(int ac, char **av)
{
	t_data2	data;
	pid_t	*pids;
	int		exit_status;

	if (init_data2(&data, &pids, ac, av) == EXIT_FAILURE)
		return (1);
	spawn_philo(&data, pids);
	exit_status = wait_for_children(&data);
	if (exit_status != 2)
		kill_all(pids, data.args.nb_philo);
	wait_all(pids, data.args.nb_philo);
	cleanup_resources(&data);
	free(data.pids);
	if (exit_status == 2)
		return (EXIT_SUCCESS);
	else
		return (EXIT_FAILURE);
}
