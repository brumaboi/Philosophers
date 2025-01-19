/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mutex.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <marvin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/06 12:31:24 by marvin            #+#    #+#             */
/*   Updated: 2024/10/06 12:31:24 by marvin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/philo.h"

int clean_mutex(t_data *data, int i)
{
    int j;

    if (data->cleaned_up)
        return (0);
    if (data->philos)
    {
        j = 0;
        while (j < i)
        {
            pthread_mutex_destroy(&data->philos[j].mutex);
            j++;
        }
        free(data->philos);
        data->philos = NULL;
    }
    if (data->forks)
    {
        j = 0;
        while (j < i)
        {
            pthread_mutex_destroy(&data->forks[j]);
            j++;
        }
        free(data->forks);
        data->forks = NULL;
    }
    pthread_mutex_destroy(&data->print);
    pthread_mutex_destroy(&data->dead_mutex);
    pthread_mutex_destroy(&data->full_mutex);
    data->cleaned_up = 1;
    return (1);
}

int init_mutex(t_data *data)
{
    int i;

    if (pthread_mutex_init(&data->print, NULL) != 0)
        return (1);
    if (pthread_mutex_init(&data->dead_mutex, NULL) != 0)
    {
        pthread_mutex_destroy(&data->print);
        return (1);
    }
    if (pthread_mutex_init(&data->full_mutex, NULL) != 0)
    {
        pthread_mutex_destroy(&data->print);
        pthread_mutex_destroy(&data->dead_mutex);
        return (1);
    }
    i = 0;
    while (i < data->philo_count)
    {
        if (pthread_mutex_init(&data->philos[i].mutex, NULL))
            return (clean_mutex(data, i), 1);
        if (pthread_mutex_init(&data->forks[i], NULL) != 0)
            return (clean_mutex(data, i), 1);
        i++;
    }
    return (0);
}