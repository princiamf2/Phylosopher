/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils2.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mm-furi <mm-furi@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/04 14:17:01 by mm-furi           #+#    #+#             */
/*   Updated: 2025/08/04 15:07:53 by mm-furi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "phylosopher_bonus.h"

size_t	ft_strlen(char *s)
{
	size_t	i;

	i = 0;
	while (s[i])
		i++;
	return (i);
}

int	error_and_exit(char *msg, t_data2 *data)
{
	write(2, "Error: ", 7);
	write(2, msg, ft_strlen(msg));
	write(2, "\n", 1);
	cleanup_resources(data);
	exit(EXIT_FAILURE);
	return (1);
}

int	open_semaphores(t_data2 *data)
{
	sem_unlink("/limit");
	data->limit = sem_open("/limit", O_CREAT | O_EXCL, 0644, data->args.nb_philo
			- 1);
	if (data->limit == SEM_FAILED)
		return (error_and_exit("sem_open limit", data));
	sem_unlink("/print");
	data->print = sem_open("/print", O_CREAT | O_EXCL, 0644, 1);
	if (data->print == SEM_FAILED)
		return (error_and_exit("sem_open print", data));
	sem_unlink("/print");
	sem_unlink("/philo_death");
	data->death_sem = sem_open("/philo_death", O_CREAT | O_EXCL, 0644, 1);
	if (data->death_sem == SEM_FAILED)
		return (error_and_exit("sem_open death", data));
	sem_unlink("/philo_death");
	if (open_forks(data) < 0)
		return (-1);
	return (0);
}

int	count_success_and_detect_failure(t_data2 *data, int *finished)
{
	int		status;
	pid_t	pid;
	int		failure;

	*finished = 0;
	failure = 0;
	pid = waitpid(-1, &status, 0);
	(void)data;
	while (pid > 0)
	{
		if (WIFEXITED(status) && WEXITSTATUS(status) == EXIT_SUCCESS)
		{
			(*finished)++;
		}
		else
		{
			failure = 1;
			break ;
		}
		pid = waitpid(-1, &status, 0);
	}
	return (failure);
}

void	build_fork_name(int i, char *name)
{
	int		j;
	char	num[6];

	name[0] = '/';
	name[1] = 'f';
	name[2] = 'o';
	name[3] = 'r';
	name[4] = 'k';
	name[5] = '_';
	name[6] = '\0';
	itoa_min(i, num);
	j = 0;
	while (num[j])
	{
		name[6 + j] = num[j];
		j++;
	}
	name[6 + j] = '\0';
}
