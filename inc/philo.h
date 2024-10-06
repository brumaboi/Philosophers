/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <marvin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/30 11:57:09 by marvin            #+#    #+#             */
/*   Updated: 2024/09/30 11:57:09 by marvin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_H
# define PHILO_H

# include <unistd.h>
# include <pthread.h>
# include <stdbool.h>
# include <stdio.h>
# include <stdlib.h>
# include <sys/time.h>

////// STRUCTURES //////
typedef struct s_philo
{
    int id;
    int left_fork;
    int right_fork;
    int meals;
    long last_meal;
    pthread_t thread;
    pthread_t dead_checker;
    pthread_mutex_t mutex;
    struct s_data *data;
} t_philo;

typedef struct s_data
{
    int philo_count;
    useconds_t time_to_die;
    useconds_t time_to_eat;
    useconds_t time_to_sleep;
    int meal_count;
    t_philo *philos;
    int death;
    int full;
    long start_time;
    pthread_mutex_t *forks;
    pthread_mutex_t print;
    pthread_mutex_t dead_mutex;
    pthread_mutex_t full_mutex;
} t_data;

////// FUNCTIONS //////



#endif