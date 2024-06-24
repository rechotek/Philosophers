/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mrechuli <mrechuli@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/24 18:40:41 by mrechuli          #+#    #+#             */
/*   Updated: 2024/06/24 18:40:43 by mrechuli         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_H
# define PHILO_H
# include <pthread.h>
# include <stdio.h>
# include <stdlib.h>
# include <sys/time.h>
# include <unistd.h>

# define PHILO_MAX 200

typedef struct s_philo
{
    pthread_t		thread;
	int				id;
	int				eating;
	int				meals_eaten;
	size_t			last_meal;
	size_t			time_to_die;
	size_t			time_to_eat;
	size_t			time_to_sleep;
	size_t			start_time;
	int				num_of_philos;
	int				num_times_to_eat;
	int				*dead;
	pthread_mutex_t	*r_fork;
	pthread_mutex_t	*l_fork;
	pthread_mutex_t	*write_lock;
	pthread_mutex_t	*dead_lock;
	pthread_mutex_t	*meal_lock;
}   t_philo;

typedef struct s_program
{
	int				dead_flag;
	pthread_mutex_t	write_lock;
	pthread_mutex_t	dead_lock;
	pthread_mutex_t	meal_lock;
	t_philo			*philos; // przechowuje wszystkich filozofow w tablicy
}   t_program;

// INIT
void    init_program(t_program *program, t_philo *philos);
void	init_forks(pthread_mutex_t *forks, int philo_num);
void	init_philos(t_philo *philos, t_program *program, pthread_mutex_t *forks, char *argv[]);

// THREADS
int		thread_create(t_program *program, pthread_mutex_t *forks);

// ROUTINE
void	eat(t_philo *philo);
void	dream(t_philo *philo);
void    think(t_philo *philo);

// MONITOR
int		dead_loop(t_philo *philo);
void	*monitor(void *pointer);

// UTILS
int		ft_atol(char *str);
void	destroy_all(char *str, t_program *program, pthread_mutex_t *forks);
int		ft_usleep(size_t miliseconds);
size_t	get_current_time(void);
void    print_message(char *str, t_philo *philo, int id);

#endif
