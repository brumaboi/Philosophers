/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_input.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <marvin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/30 12:02:16 by marvin            #+#    #+#             */
/*   Updated: 2024/09/30 12:02:16 by marvin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/philo.h"

static int	parse_input(t_data *data, int argc, char **argv)
{
	data->philo_count = ft_atoi(argv[1]);
	data->time_to_die = ft_atoi(argv[2]);
	data->time_to_eat = ft_atoi(argv[3]);
	data->time_to_sleep = ft_atoi(argv[4]);
	if (argc == 6)
		data->meal_count = ft_atoi(argv[5]);
	else
		data->meal_count = -1;
	data->death = 0;
	data->full = 0;
	if (data->philo_count < 1 || data->time_to_die < 1 || data->time_to_eat < 1
		|| data->time_to_sleep < 1 || (argc == 6 && data->meal_count < 1))
		return (printf("Error: Invalid argument\n"), 1);
	data->philos = malloc(sizeof(t_philo) * data->philo_count);
	if (!data->philos)
		return (printf("Error: Malloc failed\n"), 1);
	data->forks = malloc(sizeof(pthread_mutex_t) * data->philo_count);
	if (!data->forks)
		return (printf("Error: Malloc failed\n"), 1);
	if (init_mutex(data) == 1)
	{
		clean_mutex(data, data->philo_count);
		return (printf("Error: Mutex init failed\n"), 1);
	}
	data->start_time = ft_get_time();
	if (init_philos(data) == 1)
		return (printf("Error: Philo init failed\n"), 1);
	return (0);
}

static int	check_input(int argc, char **argv)
{
	int	i;
	int	j;

	if (argc < 5 || argc > 6)
		return (printf("Error: Wrong number of arguments\n"), 1);
	i = 1;
	while (i < argc)
	{
		j = 0;
		while (argv[i][j] != '\0')
		{
			if (argv[i][j] < '0' || argv[i][j] > '9')
				return (printf("Error: Invalid argument\n"), 1);
			j++;
		}
		if (ft_atoi(argv[i]) <= 0)
			return (printf("Error: Invalid argument\n"), 1);
		i++;
	}
	return (0);
}

int	check_parse_input(t_data *data, int argc, char **argv)
{
	if (check_input(argc, argv) == 1)
		return (1);
	if (parse_input(data, argc, argv) == 1)
		return (1);
	return (0);
}
