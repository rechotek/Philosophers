#include "philo.c"

void    think(t_philo *philo)
{
    print_message("Is thinking", philo, philo->id);
}

void	dream(t_philo *philo) // funkcja nazywa sie "dream", a nie sleep, poniewaz w C jest funkcja ktora nazywa sie sleep
{
	print_message("Is sleeping", philo, philo->id);
	ft_usleep(philo->time_to_sleep);
}