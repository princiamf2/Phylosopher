/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mm-furi <mm-furi@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/24 19:10:02 by mm-furi           #+#    #+#             */
/*   Updated: 2025/07/02 15:55:11 by mm-furi          ###   ########.fr       */
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
			perror("fork");
		if (pids[i] == 0)
			routine_philo(data, i + 1);
		i++;
	}
}

static int	wait_for_children(t_data2 *data)
{
	int		status;
	int		finished;
	pid_t	pid;

	finished = 0;
	pid = waitpid(-1, &status, 0);
	while (pid > 0)
	{
		if (WIFEXITED(status) == 0)
			return (0);
		if (WEXITSTATUS(status) == EXIT_FAILURE)
			return (1);
		if (WEXITSTATUS(status) == EXIT_SUCCESS)
		{
			finished++;
			if (finished == data->args.nb_philo)
				return (2);
		}
		else
			return (0);
		pid = waitpid(-1, &status, 0);
	}
	return (0);
}

void	cleanup_and_wait(t_data2 *data, pid_t *pids)
{
	int	result;

	result = wait_for_children(data);
	(void)result;
	kill_all(pids, data->args.nb_philo);
	wait_all(pids, data->args.nb_philo);
	close_semaphores(data);
	free(pids);
}

int	main(int ac, char **av)
{
	t_data2	data;
	pid_t	*pids;

	if (init_data2(&data, &pids, ac, av) == EXIT_FAILURE)
		return (1);
	spawn_philo(&data, pids);
	cleanup_and_wait(&data, pids);
	return (EXIT_SUCCESS);
}
