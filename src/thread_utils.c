/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   thread_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nal-haki <nal-haki@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/03 15:10:15 by nal-haki          #+#    #+#             */
/*   Updated: 2024/12/20 13:43:40 by nal-haki         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	threads_join(t_data *data)
{
	int	i;

	i = -1;
	while (++i < data->num_of_philos)
		if (pthread_join(data->philos[i].thread, NULL) != 0)
			return (write(2, "pthread_join error\n", 19), 1);
	if (pthread_join(data->monitor, NULL) != 0)
		return (write(2, "pthread_join error\n", 19), 1);
	return (0);
}

/*
 * Checks if num_required_meals var is satisfied
 */
int	meals_check(t_data *data)
{
	int	i;

	i = -1;
	while (++i < data->num_of_philos)
	{
		pthread_mutex_lock(&data->eaten_enough_lock);
		if (!(data->philos[i].has_eaten_enough))
			return (pthread_mutex_unlock(&data->eaten_enough_lock), 0);
		pthread_mutex_unlock(&data->eaten_enough_lock);
	}
	usleep(10);
	return (1);
}

int	is_dead(t_data *data)
{
	size_t	dead_time;
	int		i;

	i = -1;
	if (timestamp(data) < data->time_to_die)
		dead_time = 0;
	else
		dead_time = timestamp(data) - data->time_to_die;
	while (++i < data->num_of_philos)
	{
		pthread_mutex_lock(&data->last_meal_lock);
		if (data->philos[i].time_of_last_meal <= dead_time && dead_time != 0)
		{
			print_status(&(data->philos[i]), "died 💀", 1);
			pthread_mutex_lock(&data->write_lock);
			data->finished = 1;
			pthread_mutex_unlock(&data->last_meal_lock);
			return (pthread_mutex_unlock(&data->write_lock), 1);
		}
		pthread_mutex_unlock(&data->last_meal_lock);
	}
	return (0);
}

void	destroy_locks(t_data *data)
{
	int	i;

	i = -1;
	while (++i < data->num_of_philos)
		pthread_mutex_destroy(&(data->forks_lock[i]));
	pthread_mutex_destroy(&(data->write_lock));
}

void	meals_eaten_util(t_philo *philo)
{
	philo->meals_eaten += 1;
	pthread_mutex_lock(&philo->data->eaten_enough_lock);
	if (!philo->has_eaten_enough
		&& (philo->meals_eaten == philo->data->num_required_meals))
		philo->has_eaten_enough = 1;
	pthread_mutex_unlock(&philo->data->eaten_enough_lock);
}
