#include "philo.h"

// sprawdzam czy wprowadzone dane sa liczbami, jesli nie to zwracam blad

int	check_arg_isnumber(char *arg)
{
	int x;

	x = 0;
	while(arg[x])
	{
		if(arg[x] < '0' || arg[x] > '9')
			return(1); // 1 to kod bledu
		x++;
	}
	return(0); // to znaczy ze nie ma bledu
}

// sprawdza czy wprowadzone dane sa prawidlowe
// ogranicza sie to wlasciwie tylko do sprawdzenia czy to liczba i czy nie jest ujemna
// dodatkowo w pierwszym argumencie sprawdzam czy nie przekracza wartosci PHILO_MAX
// dodatkowo w piatym argumencie sprawdzam czy on wogole istnieje (bo to jest opcjonalny argument)

int	check_valid_args(char *argv[])
{
	if(ft_atol(argv[1]) > PHILO_MAX || ft_atol(argv[1]) <= 0 || check_arg_content(argv[1]) == 1) // sprawdza czy ilosc filozofow jesy OK
		return(write(2, "Invalid philosophers number\n", 29), 1);
	if(ft_atol(argv[2]) <= 0 || check_arg_isnumber(argv[2]) == 1)
		return(write(2, "Invalid time to die\n", 21), 1);
	if(ft_atol(argv[3]) <= 0 || check_arg_isnumber(argv[3]) == 1)
		return(write(2, "Invalid time to eat\n", 21), 1);
	if(ft_atol(argv[4]) <= 0 || check_arg_isnumber(argv[4]) == 1)
		return(write(2, "Invalid time to sleep\n", 23), 1);
	if(argv[5] && (ft_atol(argv[5]) < 0 || check_arg_isnumber(argv[5]) == 1))
		return(write(2, "Invalid numbers of times each philosopher must eat\n", 51), 1);
	return(0);
}

// struktura wprowadza danych:
// ./a.out 5 800 200 200 7
// 5 - liczba filozofow
// 800 - czas po ktorym filozof umrze jesli nie zje
// 200 - czas jaki potrzebuje filozof do zjedzenia
// 200 - czas jaki potrzebuje filozof do snu
// 7 - ilosc posilkow, jakie wszyscy filozofowie musza zjesc przed zakonczeniem programu (* argument opcjonalny)

int main(int argc, char *argv[])
{
	t_program		program;
	t_philo			philos[PHILO_MAX];
	pthread_mutez_t	forks[PHILO_MAX];

	if(argc != 5 && argc != 6) // jesli liczba argumentow nie jest rowna ani 5 ani 6...
		return(write(2, "Wrong argument count\n", 22), 1); // ... to wypisuje komunikat bledu (2 odpowiada standardowemu wyjsciu bledu) i koncze program z wartoscia 1 ( return(1) )
	if(check_valid_args(argv) == 1) // jesli funkcja zwraca 1 oznacza to ze podane argumenty wejsciowe sa nieprawidlowe.
		return(1); // wiec zwracam 1, bo jest to standardowy kod bledu
}