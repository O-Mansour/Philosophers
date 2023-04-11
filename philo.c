/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: omansour <omansour@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/26 20:04:25 by omansour          #+#    #+#             */
/*   Updated: 2023/04/10 22:46:38 by omansour         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	ft_atoi(char *str)
{
	int					i;
	unsigned long		res;

	if (!str)
		return (-13);
	if (!str[0])
		return (-13);
	i = 0;
	res = 0;
	while (str[i] == 32 || (str[i] >= 9 && str[i] <= 13))
		i++;
	while (str[i] >= '0' && str[i] <= '9')
	{
		res *= 10;
		res += str[i] - 48;
		i++;
		if (res > INT_MAX)
			return (-13);
	}
	return (res);
}

int	check_args(int ac, char *av[])
{
	if (ac != 5 && ac != 6)
		return (1);
	if (ft_atoi(av[1]) <= 0)
		return (1);
	int i = 0;
	int j;
	while (av[++i])
	{
		j = -1;
		while (av[i][++j])
		{
			if (av[i][j] < '0' || av[i][j] > '9')
				return (1);
		}
		if (ft_atoi(av[i]) == -13 || ft_atoi(av[i]) == 0)
			return (1);
	}
	return (0);
}


int	varz_init(t_varz *varz, char *av[])
{
	int i = 0;
	pthread_mutex_t *mutex = malloc(sizeof(pthread_mutex_t) * ft_atoi(av[1]));
	pthread_mutex_t *for_print = malloc(sizeof(pthread_mutex_t));
	pthread_mutex_t *for_last_meal = malloc(sizeof(pthread_mutex_t));
	if (!mutex || !for_print || !for_last_meal)
		return (1);
	pthread_mutex_init(for_print, NULL);
	pthread_mutex_init(for_last_meal, NULL);
	while (i < ft_atoi(av[1]))
	{
		varz[i].nb_philo = ft_atoi(av[1]);
		varz[i].time_d = ft_atoi(av[2]);
		varz[i].time_e = ft_atoi(av[3]);
		varz[i].time_s = ft_atoi(av[4]);
		varz[i].nb_meal = ft_atoi(av[5]);
		varz[i].forksmutex = mutex;
		varz[i].for_print = for_print;
		varz[i].for_last_meal = for_last_meal;
		i++;
	}
	i = 0;
	while (i < varz->nb_philo)
	{
		pthread_mutex_init(&(varz + i)->forksmutex[i], NULL);
		i++;
	}
	return (0);
}


unsigned long int get_curr_time(void)
{
	struct timeval tv;
    gettimeofday(&tv, NULL);
	return ((tv.tv_sec * 1000) + (tv.tv_usec / 1000));
}


void	print_action(unsigned long int tv, int nbr, char *str, pthread_mutex_t *for_print)
{
	pthread_mutex_lock(for_print);
	printf("%lu ms %d %s\n", tv, nbr, str);
	pthread_mutex_unlock(for_print);
}

void	*routine(void *vars)
{
	t_varz	*varz = (t_varz *) vars;
	int	last_sleep_time = -1;
	while (++last_sleep_time < 5)
	{
		pthread_mutex_lock(&varz->forksmutex[varz->nbr - 1]);
		print_action(get_curr_time() - varz->start_time, varz->nbr, "has taken a fork", varz->for_print);
		pthread_mutex_lock(&varz->forksmutex[varz->nbr % varz->nb_philo]);
		print_action(get_curr_time() - varz->start_time, varz->nbr, "has taken a fork", varz->for_print);
		print_action(get_curr_time() - varz->start_time, varz->nbr, "is eating", varz->for_print);
		pthread_mutex_lock(varz->for_last_meal);
		varz[varz->nbr - 1].last_meal_time = get_curr_time();
		if (varz[varz->nbr - 1].nb_meal != -13)
			varz[varz->nbr - 1].nb_meal--;
		pthread_mutex_unlock(varz->for_last_meal);
		while (get_curr_time() - varz[varz->nbr - 1].last_meal_time < varz->time_e)
			usleep(130);
		pthread_mutex_unlock(&varz->forksmutex[varz->nbr - 1]);
		pthread_mutex_unlock(&varz->forksmutex[varz->nbr % varz->nb_philo]);
		if (varz[varz->nbr - 1].nb_meal == 0)
			return (NULL);
		last_sleep_time = get_curr_time();
		print_action(get_curr_time() - varz->start_time, varz->nbr, "is sleeping", varz->for_print);
		while (get_curr_time() - last_sleep_time < varz->time_s)
			usleep(130);
		print_action(get_curr_time() - varz->start_time, varz->nbr, "is thinking", varz->for_print);
	}
	return (NULL);
}

int	zero_meal(t_varz *varz)
{
	int i = -1;
	while (++i < varz->nb_philo)
	{
		if (varz[i].nb_meal != 0)
			return 0;
	}
	return 1;
}

void	death_clock(t_varz *varz)
{
	int	i;
	while(1)
	{
		i = -1;
		while (++i < varz->nb_philo)
		{
			pthread_mutex_lock(varz->for_last_meal);
			if (get_curr_time() - varz[i].last_meal_time > varz->time_d)
			{
				printf("%ld ms %d died\n", get_curr_time() - varz->start_time, i + 1);
				return ;
			}
			if (varz[i].nb_meal == 0)
			{
				if (zero_meal(varz))
					return ;
			}
			pthread_mutex_unlock(varz->for_last_meal);
		}
	}
}

int main(int ac, char *av[])
{
	if (check_args(ac, av))
	{
		printf("ERROR.\nIncorrect arguments.\n");
		return (1);
	}
	t_varz *varz = malloc(sizeof(t_varz) * ft_atoi(av[1]));
	if (!varz)
		return (1);
	int i = varz_init(varz, av);
	if (i)
		return (1);
	pthread_t tid[varz->nb_philo];
	while (i < varz->nb_philo)
	{
		varz->nbr = i + 1;
		varz[i].start_time = get_curr_time();
		varz[i].last_meal_time = varz->start_time;
		if (pthread_create(&tid[i], NULL, &routine, varz) != 0) {
			printf("Failed to create thread\n");
			return (1);
        }
		pthread_detach(tid[i]);
		usleep(1000);
		i++;
	}
	death_clock(varz);
	return 0;
}
