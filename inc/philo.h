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
	int				id;
	int				left_fork;
	int				right_fork;
	int				meals;
	long			last_meal;
	pthread_t		thread;
	pthread_t		dead_checker;
	pthread_mutex_t	mutex;
	struct s_data	*data;
}	t_philo;

typedef struct s_data
{
	int				philo_count;
	useconds_t		time_to_die;
	useconds_t		time_to_eat;
	useconds_t		time_to_sleep;
	int				meal_count;
	t_philo			*philos;
	int				death;
	int				full;
	long			start_time;
	pthread_mutex_t	*forks;
	pthread_mutex_t	print;
	pthread_mutex_t	dead_mutex;
	pthread_mutex_t	full_mutex;
	int				cleaned_up;
	pthread_t		monitor_thread;
}	t_data;

////// FUNCTIONS //////
//init_philos.c
int			init_philos(t_data *data);
//mutex.c
int			clean_mutex(t_data *data, int i);
int			init_mutex(t_data *data);
//parse_input.c
int			check_parse_input(t_data *data, int argc, char **argv);
//routine.c
void		*routine(void *arg);
//time.c
long long	ft_get_time(void);
void		ft_usleep(useconds_t usec, t_data *data);
void		ft_sleep(t_philo *philo, t_data *data);
//utils.c
int			ft_atoi(const char *str);
void		print_action(t_data *data, t_philo *philo, const char *str);
int			one_dead(t_data *data);
void		set_death(t_data *data);
int			will_starve(t_philo *philo, t_data *data);

#endif