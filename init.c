#include "philo.h"

// Inicjalizuje pola struktury t_philo na podstawie argumentow wejsciowych programu
// Argumentami wejsciowymi programu sa w duzej mierze paramatery czasu dla pojedynczego filozofa (argv[2,3,4])
// argv[1] to liczba filozofow
// argv[5] jest to parametr opcjonalny, ktory mowi ile razy kazdy filozof musi zjesc, aby zakonczyc dzialanie programu

void	init_input(t_philo *philo, char *argv[])
{
	philo->time_to_die = ft_atol(argv[2]);
	philo->time_to_eat = ft_atol(argv[3]);
	philo->time_to_sleep = ft_atol(argv[4]);
	philo->num_of_philos = ft_atol(argv[1]);
	if(argv[5])
		philo->num_times_to_eat = ft_atol(argv[5]);
	else
		philo->num_times_to_eat = -1; // -1 oznacza blad, a jest blad poniewaz nie ma 5 argumentu (argv[5])
}

// Inicjalizuje tablice filozofow przypisujac odpiwednie wartosci ich polom
// philos[x] - x oznacza miejsce w tablicy filozofow, czyli konkretnego filozofa
// forks[x] - x oznacza konkretny widelec; kazdy filozof ma inny lewy i prawy widelec, ale "nakladaja" sie one na siebie (patrz obrazek w swoim pliku)

void	init_philos(t_philo *philos, t_program *program, pthread_mutex_t *forks, char *argv[])
{
	int	x;

	x = 0;
	while(x < ft_atol(argv[1])) // petla wykonuje sie dopoki nie przypiszemy wartosci wszystkim filozofom
	{
		philos[x].id = x + 1; // ustawia identyfikator filozofa
		philos[x].eating = 0; // ustawia flage "eating" na 0, co oznacza ze filozof nie je
		philos[x].meals_eaten = 0; // ustawia liczbe zjedzonych posilkow na 0
		init_input(&philos[x], argv); // wywoluje init_input aby zainicjalizowac pola zwiazane z czasem (czyli z danymi ktore wprowadza sie przy starcie porgramu)
		philos[x].start_time = get_current_time(); // ustawia czas startu programu na aktualny
		philos[x].last_meal = get_current_time(); // ustawia czas ostatniego posilku na aktualny
		philos[x].write_lock = &program->write_lock; // ustawia wskaznik na adres mutexu
		philos[x].dead_lock = &program->dead_lock; // ustawia wskaznik na adres mutexu
		philos[x].meal_lock = &program->meal_lock; // ustawia wskaznik na adres mutexu
		philos[x].dead = &program->dead_flag; // ustawia wskaznik dead na adres flagi dead
		philos[x].l_fork = &forks[x]; // ustawia wskaznik l_fork na adres muteksu reprezentujacy lewy widelec
		if(x == 0)
			philos[x].r_fork = &forks[philos[x].num_of_philos - 1]; // jesli lewy widelec jest pierwszym widelcem (x = 0) to prawym musi byc ostatni (czyli liczba filozofow -1)
		else
			philos[x].r_fork = &forks[x - 1]; // w kazdym innym przypadku prawy widelec to jest -1 w stosunku do lewego
		x++;
	}
}

// Sluzy do inicjalizacji tablicy muteksów, które reprezentują widelce w problemie "filozofów przy stole".
// Każdy filozof musi mieć dostęp do dwóch widelców (muteksów), aby móc jeść. Inicjalizacja muteksów
// zapewnia, że będą one gotowe do użycia w programie wielowątkowym.

void	init_forks(pthread_mutex_t *forks, int philo_num) // pthread_mutex_t *forks to wskaznik do tablicy mutexow; philo_num to liczba filozofow a tym samym liczba widelcow ktore nalezy zainicjalizowac
{
	int	x;

	x = 0;
	while(x < philo_num)
	{
		pthread_mutex_init(&forks[x], NULL); // "&forks[x]" wskaznik do biezacego elementu tablicy mutexow z widelcami
		x++;
	}
}

// Inicjalizacja struktury programu
// pthread_mutex_init sluzy do inicjalizacji muteksu w programowaniu z uzyciem watkow POSIX (pthreads)
// pierwszy argument tej funkcji to wskaznik do muteksu, ktory ma zostac zainicjalizowany
// drugi argument to wskaznik do atrybutow muteksu. Tutaj ustawione na NULL, co oznacza ze uzywane sa domyslne atrybuty

void    init_program(t_program *program, t_philo *philos)
{
    program->dead_flag = 0; // ustawiam na 0 czyli ze zaden filozof poki co nie umarl
    program->philos = philos; // "program->philos" to wskaźnik w strukturze "t_program", który teraz będzie wskazywał na tablicę filozofów "philos"; dzięki temu, struktura "program" przechowuje referencję do tablicy filozofów, co pozwala na łatwy dostęp do niej w innych częściach programu. Można teraz uzyskać dostęp do filozofów przez "program->philos"
    pthread_mutex_init(&program->write_lock, NULL); // inicjalizacja mutexow (niezbedna aby je wywolac)
    pthread_mutex_init(&program->dead_lock, NULL);
    pthread_mutex_init(&program->meal_lock, NULL);
}
