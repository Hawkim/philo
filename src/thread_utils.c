/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   thread_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nal-haki <nal-haki@student.42beirut.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/03 15:10:15 by nal-haki          #+#    #+#             */
/*   Updated: 2025/01/23 22:29:47 by nal-haki         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	threads_join(t_table *table)
{
	int	i;

	i = -1;
	while (++i < table->philo_number)
		if (pthread_join(table->philos[i].thread, NULL) != 0)
			return (write(2, "pthread_join error\n", 19), 1);
	if (pthread_join(table->monitor, NULL) != 0)
		return (write(2, "pthread_join error\n", 19), 1);
	return (0);
}

/*
 * Checks if required_meal_number var is satisfied
 */
int	meals_check(t_table *table)
{
	int	i;

	i = -1;
	while (++i < table->philo_number)
	{
		pthread_mutex_lock(&table->eaten_enough_lock);
		if (!(table->philos[i].meals_satisfied))
			return (pthread_mutex_unlock(&table->eaten_enough_lock), 0);
		pthread_mutex_unlock(&table->eaten_enough_lock);
	}
	usleep(10);
	return (1);
}

int	is_dead(t_table *table)
{
	size_t	dead_time;
	int		i;

	i = -1;
	if (timestamp(table) < table->die_time)
		dead_time = 0;
	else
		dead_time = timestamp(table) - table->die_time;
	while (++i < table->philo_number)
	{
		pthread_mutex_lock(&table->last_meal_lock);
		if (table->philos[i].last_meal_time <= dead_time && dead_time != 0)
		{
			print_status(&(table->philos[i]), "died 💀", 1);
			pthread_mutex_lock(&table->write_lock);
			table->has_finished = 1;
			pthread_mutex_unlock(&table->last_meal_lock);
			return (pthread_mutex_unlock(&table->write_lock), 1);
		}
		pthread_mutex_unlock(&table->last_meal_lock);
	}
	return (0);
}

void	destroy_locks(t_table *table)
{
	int	i;

	i = -1;
	while (++i < table->philo_number)
		pthread_mutex_destroy(&(table->forks_lock[i]));
	pthread_mutex_destroy(&(table->write_lock));
}

void	meals_count_util(t_philo *philo)
{
	philo->meals_count += 1;
	pthread_mutex_lock(&philo->table->eaten_enough_lock);
	if (!philo->meals_satisfied
		&& (philo->meals_count == philo->table->required_meal_number))
		philo->meals_satisfied = 1;
	pthread_mutex_unlock(&philo->table->eaten_enough_lock);
}
