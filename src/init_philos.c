/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   actions.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <marvin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/30 12:01:08 by marvin            #+#    #+#             */
/*   Updated: 2024/09/30 12:01:08 by marvin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/philo.h"

static void	*monitor_routine(void *arg)
{
	t_data	*data = (t_data *)arg;
	int		i;

	while (1)
	{
		i = 0;
		while (i < data->philo_count)
		{
			if (will_starve(&data->philos[i], data) == 1)
				return (NULL);
			i++;
		}
		pthread_mutex_lock(&data->full_mutex);
		if (data->full == data->philo_count)
		{
			set_death(data);
			pthread_mutex_unlock(&data->full_mutex);
			return (NULL);
		}
		pthread_mutex_unlock(&data->full_mutex);
		ft_usleep(50, data);
	}
	return (NULL);
}

static void	create_philos(t_data *data)
{
	int	i;
	int	j;

	i = 0;
	while (i < data->philo_count)
	{
		if (pthread_create(&data->philos[i].thread,
				NULL, routine, &data->philos[i]))
		{
			set_death(data);
			clean_mutex(data, i);
			break ;
		}
		i++;
	}
	ft_usleep(1, data);
	if (pthread_create(&data->monitor_thread, NULL, monitor_routine, data))
	{
		set_death(data);
		clean_mutex(data, i);
		return ;
	}
	j = 0;
	while (j < i)
	{
		pthread_join(data->philos[j].thread, NULL);
		j++;
	}
	pthread_join(data->monitor_thread, NULL);
	clean_mutex(data, data->philo_count);
}

int	init_philos(t_data *data)
{
	int	i;

	i = 0;
	while (i < data->philo_count)
	{
		data->philos[i].id = i;
		data->philos[i].left_fork = i;
		data->philos[i].right_fork = (i + 1) % data->philo_count;
		data->philos[i].meals = 0;
		data->philos[i].last_meal = data->start_time;
		data->philos[i].data = data;
		i++;
	}
	create_philos(data);
	return (0);
}
