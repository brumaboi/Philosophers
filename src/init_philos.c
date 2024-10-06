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

# include "../inc/philo.h"

static void check_philos(t_data *data)
{
    int i;

    while(1)
    {
        i = 0; 
        while (i < data->philo_count)
        {
            if(will_starve(&data->philos[i], data) == 1)
                return ;
            i++;
        }
        pthread_mutex_lock(&data->full_mutex);
        if (data->full == data->philo_count)
        {
            set_death(data);
            pthread_mutex_unlock(&data->full_mutex);
            return ;
        }
        pthread_mutex_unlock(&data->full_mutex);
        ft_usleep(100);
    }
}

static void create_philos(t_data *data)
{
    int i;

    i = 0;
    while (i < data->philo_count)
    {
        if (pthread_create(&data->philos[i].thread, NULL, routine, &data->philos[i]))
        {
            set_death(data);
            clean_mutex(data, i);
            break ;
        }
        i++;
    }
    check_philos(data);
    i = 0;
    while (i < data->philo_count)
    {
        pthread_join(data->philos[i].thread, NULL);
        i++;
    }
    clean_mutex(data, data->philo_count);
}

int init_philos(t_data *data)
{
    int i;

    i = 0;
    while (i < data->philo_count)
    {
        data->philos[i].id = i;
        data->philos[i].left_fork = i;
        data->philos[i].right_fork = (i + 1) % data->philo_count;
        data->philos[i].meals = 0;
        data->philos[i].last_meal = data->start_time;
        i++;
    }
    create_philos(data);
    return (0);
}


