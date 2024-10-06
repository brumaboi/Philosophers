/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <marvin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/30 12:00:11 by marvin            #+#    #+#             */
/*   Updated: 2024/09/30 12:00:11 by marvin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "../inc/philo.h"

int main(int argc, char **argv)
{
    t_data data;

    data.philos = NULL;
    data.forks = NULL;
    if (check_parse_input(&data, argc, argv) == 1)
    {
        if (data.philos)
            free(data.philos);
        if(data.forks)
            free(data.forks);
        return (1);
    }
    else 
    {
        clean_mutex(&data, data.philo_count);
        return (0);
    }
}
