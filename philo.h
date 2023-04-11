/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: omansour <omansour@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/26 20:04:42 by omansour          #+#    #+#             */
/*   Updated: 2023/04/10 22:38:24 by omansour         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_H
# define PHILO_H

# include <stdio.h>
# include <string.h>
# include <stdlib.h>
# include <unistd.h>
# include <sys/time.h>
# include <limits.h>
# include <pthread.h>

typedef struct s_varz
{
	int						nb_philo;
	int						nb_meal;
	unsigned int			time_d;
	unsigned int			time_e;
	unsigned int			time_s;
	int						nbr;
	unsigned long int		start_time;
	unsigned long int		last_meal_time;
	pthread_mutex_t			*for_print;
	pthread_mutex_t			*for_last_meal;
	pthread_mutex_t			*forksmutex;
}	t_varz;

#endif