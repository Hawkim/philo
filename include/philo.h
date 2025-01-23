/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nal-haki <nal-haki@student.42beirut.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/20 13:42:31 by nal-haki          #+#    #+#             */
/*   Updated: 2025/01/23 09:05:39 by nal-haki         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_H
# define PHILO_H

# include <limits.h>
# include <pthread.h>
# include <stdint.h>
# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include <sys/time.h>
# include <time.h>
# include <unistd.h>

# define MAX_PHILOS 200

typedef struct s_philo
{
	int				id;
	int				meals_count;
	int				meals_satisfied;
	pthread_t		thread;
	int				left_fork;
	int				right_fork;
	size_t			last_meal_time;
	struct s_data	*data;
}					t_philo;

typedef struct s_data
{
	struct s_philo	philos[MAX_PHILOS];
	int				philo_number;
	size_t			die_time;
	size_t			eat_time;
	size_t			sleep_time;
	int				is_meal_required;
	int				required_meal_number;
	size_t			start_time;
	pthread_t		monitor;
	pthread_mutex_t	write_lock;
	pthread_mutex_t	forks_lock[MAX_PHILOS];
	pthread_mutex_t	last_meal_lock;
	pthread_mutex_t	eaten_enough_lock;
	int				has_finished;
}					t_data;

// * parse_args.c *
int					parse_args(int ac, char **av, t_data *data);

// * utils.c *
size_t				get_current_time(void);
int					ft_usleep(size_t milliseconds);
size_t				timestamp(t_data *data);
int					print_status(t_philo *philo, char *str, int force_print);

// * init.c *
int					init_data(int ac, char **av, t_data *data);

// * thread_utils.c *
int					threads_join(t_data *data);
int					meals_check(t_data *data);
int					is_dead(t_data *data);
void				destroy_locks(t_data *data);
void				meals_count_util(t_philo *philo);

#endif