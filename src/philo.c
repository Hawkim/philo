/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nal-haki <nal-haki@student.42beirut.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/20 14:09:58 by nal-haki          #+#    #+#             */
/*   Updated: 2025/01/24 20:46:00 by nal-haki         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

/*
 * av[1] = number_of_philosophers (min 1 / max 200)
 * av[2] = die_time (min 60ms)
 * av[3] = eat_time (min 60ms)
 * av[4] = sleep_time (min 60ms)
 * av[5] = number_of_times_each_philosopher_must_eat [optional]
 */

int	eat(t_philo *philo)
{
	if (philo->id % 2 == 1)
		pthread_mutex_lock(&(philo->table->forks_lock[philo->leftfork]));
	else
		pthread_mutex_lock(&(philo->table->forks_lock[philo->rightfork]));
	print_status(philo, "took a fork 🍴", 0);
	if (philo->table->philo_number == 1)
		return (pthread_mutex_unlock(
				&(philo->table->forks_lock[philo->leftfork])), 1);
	if (philo->id % 2 == 1)
		pthread_mutex_lock(&(philo->table->forks_lock[philo->rightfork]));
	else
		pthread_mutex_lock(&(philo->table->forks_lock[philo->leftfork]));
	// print_status(philo, "took a fork 🍴", 0);
	pthread_mutex_lock(&philo->table->last_meal_lock);
	philo->last_meal_time = timestamp(philo->table);
	pthread_mutex_unlock(&philo->table->last_meal_lock);
	print_status(philo, "is eating 🍇", 0);
	ft_usleep(philo->table->eat_time);
	meals_count_util(philo);
	pthread_mutex_unlock(&(philo->table->forks_lock[philo->rightfork]));
	pthread_mutex_unlock(&(philo->table->forks_lock[philo->leftfork]));
	print_status(philo, "is sleeping 🛌", 0);
	ft_usleep(philo->table->sleep_time);
	print_status(philo, "is thinking 🤔", 0);
	return (0);
}

void	*monitor(void *arg)
{
	t_table	*table;

	table = (t_table *)arg;
	while (1)
	{
		if (is_dead(table))
			return (NULL);
		if (table->is_meal_required && meals_check(table))
		{
			pthread_mutex_lock(&table->write_lock);
			table->has_finished = 1;
			return (pthread_mutex_unlock(&table->write_lock), NULL);
		}
		usleep(100);
	}
	return (NULL);
}

void	*routine(void *arg)
{
	t_philo	*philo;

	philo = (t_philo *)arg;
	if (philo->id % 2 == 0)
		ft_usleep(philo->table->eat_time);
	while (1)
	{
		if (eat(philo))
		{
			pthread_mutex_lock(&philo->table->write_lock);
			philo->table->has_finished = 1;
			return (pthread_mutex_unlock(&philo->table->write_lock), NULL);
		}
		pthread_mutex_lock(&philo->table->write_lock);
		if (philo->table->has_finished == 1)
			return (pthread_mutex_unlock(&philo->table->write_lock), NULL);
		pthread_mutex_unlock(&philo->table->write_lock);
		if (philo->table->philo_number % 2 == 1)
			ft_usleep((philo->table->eat_time * 2)
				- (philo->table->sleep_time));
	}
	return (NULL);
}

void	run_philos(t_table *table)
{
	int	i;

	pthread_create(&table->monitor, NULL, monitor, (void *)table);
	i = -1;
	while (++i < table->philo_number)
	{
		pthread_mutex_lock(&table->last_meal_lock);
		table->philos[i].last_meal_time = timestamp(table);
		pthread_create(&table->philos[i].thread, NULL, routine,
			(void *)&table->philos[i]);
		pthread_mutex_unlock(&table->last_meal_lock);
	}
}

int	main(int ac, char **av)
{
	t_table	table;

	if (init_table(ac, av, &table) != 0)
		return (1);
	table.start_time = get_current_time();
	run_philos(&table);
	threads_join(&table);
	destroy_locks(&table);
	return (0); 
}
