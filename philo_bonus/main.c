/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mm-furi <mm-furi@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/24 19:10:02 by mm-furi           #+#    #+#             */
/*   Updated: 2025/06/26 18:04:46 by mm-furi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "phylosopher_bonus.h"

int	init_data2(t_data2 *data, pid_t **pids, int ac, char **av)
{
	int	argc_ok;

	if (ac < 5 || ac > 6)
	{
		fprintf(stderr,
				"Usage: %s nb_philo time_to_die time_to_eat "
				"time_to_sleep [meals_required]",
				av[0]);
		return (EXIT_FAILURE);
	}
	argc_ok = (ac >= 5 && ac <= 6);
	if (!argc_ok)
		return (EXIT_FAILURE);
	data->args.nb_philo = ft_atoi(av[1]);
	data->args.time_to_die = ft_atoi(av[2]);
	data->args.time_to_eat = ft_atoi(av[3]);
	data->args.time_to_sleep = ft_atoi(av[4]);
	if (ac == 6)
		data->args.meals_required = ft_atoi(av[5]);
	else
		data->args.meals_required = -1;
	*pids = malloc(sizeof(pid_t) * data->args.nb_philo);
	data->pids = *pids;
	if (!*pids)
		return (EXIT_FAILURE);
	open_semaphores(data, data->args.nb_philo);
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

void	cleanup_and_wait(t_data2 *data, pid_t *pids)
{
	int		i;
	int		status;
	int		finished;
	pid_t	pid;

	finished = 0;
	while ((pid = waitpid(-1, &status, 0)) > 0)
	{
		if (WEXITSTATUS(status))
		{
			if (WEXITSTATUS(status) == EXIT_FAILURE)
				break;
			else if (WEXITSTATUS(status) == EXIT_SUCCESS)
			{
				finished++;
				if (finished == data->args.nb_philo)
					break;
			}
		}
		else
			break ;
	}
	i = 0;
	while (i < data->args.nb_philo)
	{
		kill(pids[i], SIGKILL);
		i++;
	}
	i = 0;
	while (i < data->args.nb_philo)
	{
		waitpid(pids[i], NULL, 0);
		i++;
	}
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
