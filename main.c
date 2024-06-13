#include "philo.h"

int main(int argc, char *argv[])
{
	t_program		program;
	t_philo			philos[PHILO_MAX];
	pthread_mutez_t	forks[PHILO_MAX];

	if(argc != 5 && argc != 6) // jesli liczba argumentow nie jest rowna ani 5 ani 6...
		return(write(2, "Wrong argument count\n", 22), 1); // ... to wypisuje komunikat bledu (2 odpowiada standardowemu wyjsciu bledu)
}