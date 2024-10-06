/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   routine.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <marvin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/30 12:00:54 by marvin            #+#    #+#             */
/*   Updated: 2024/09/30 12:00:54 by marvin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "../inc/philo.h"

static void lock_forks(t_philo *philo, t_data *data)
{
    if (philo->left_fork < philo->right_fork)
    {
        pthread_mutex_lock(&data->forks[philo->left_fork]);
        print_action(data, philo, "has taken a fork");
        pthread_mutex_lock(&data->forks[philo->right_fork]);
        print_action(data, philo, "has taken a fork");
    }
    else if (philo->left_fork > philo->right_fork)
    {
        pthread_mutex_lock(&data->forks[philo->right_fork]);
        print_action(data, philo, "has taken a fork");
        pthread_mutex_lock(&data->forks[philo->left_fork]);
        print_action(data, philo, "has taken a fork");
    }
    else
    {
        pthread_mutex_lock(&data->forks[philo->left_fork]);
        print_action(data, philo, "has taken a fork");
        if (one_dead(data))
        {
            pthread_mutex_unlock(&data->forks[philo->left_fork]);
            return ;
        }
        ft_usleep(100);
    }
}

static void unlock_forks(t_philo *philo, t_data *data)
{
    if (philo->left_fork < philo->right_fork)
    {
        pthread_mutex_unlock(&data->forks[philo->left_fork]);
        pthread_mutex_unlock(&data->forks[philo->right_fork]);
    }
    else if (philo->left_fork > philo->right_fork)
    {
        pthread_mutex_unlock(&data->forks[philo->right_fork]);
        pthread_mutex_unlock(&data->forks[philo->left_fork]);
    }
    else
    {
        pthread_mutex_unlock(&data->forks[philo->left_fork]);
    }
}

static int give_forks_not_dead(t_philo *philo, t_data *data)
{
    lock_forks(philo, data);
    pthread_mutex_lock(&philo->mutex);
    will_starve(philo, data);
    if(one_dead(data) == 1)
    {
        pthread_mutex_unlock(&philo->mutex);
        unlock_forks(philo, data);
        return (1);
    }
    pthread_mutex_unlock(&philo->mutex);
    return (0);
}

static void ft_eat(t_philo *philo, t_data *data)
{
    if(one_dead(data) == 1)
        return ;
    if (give_forks_not_dead(philo, data) == 1)
        return ;
    print_action(data, philo, "is eating");
    pthread_mutex_lock(&philo->mutex);
    philo->last_meal = ft_get_time();
    pthread_mutex_unlock(&philo->mutex);
    ft_usleep(data->time_to_eat);
    pthread_mutex_lock(&philo->mutex);
    philo->meals++;
    pthread_mutex_unlock(&philo->mutex);
    if (data->meal_count != -1 && philo->meals >= data->meal_count)
    {
        pthread_mutex_lock(&data->full_mutex);
        data->full++;
        pthread_mutex_unlock(&data->full_mutex);
    }
    unlock_forks(philo, data);
}

void *routine(void *arg)
{
    t_philo *philo;
    t_data *data;

    philo = (t_philo *)arg;
    data =  philo->data;
    if (philo->id % 2 == 0)
        ft_usleep(data->time_to_eat / 2);
    while(one_dead(data) == 0)
    {
        print_action(data, philo, "is thinking");
        ft_eat(philo, data);
        ft_sleep(philo, data);
    }
    return (NULL);
}
