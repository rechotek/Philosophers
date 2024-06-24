/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mrechuli <mrechuli@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/24 18:41:00 by mrechuli          #+#    #+#             */
/*   Updated: 2024/06/24 18:41:06 by mrechuli         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	ft_atol(char *str)
{
	unsigned long long	num;
	int					sign;
	int					x;

	num = 0;
	sign = 1;
	x = 0;
	while(str[x] && (str[x] == ' ' || str[x] == '\r' || str[x] == '\t' || str[x] == '\v' || str[x] == '\f' || str[x] == '\n'))
		x++;
	if(str[x] == '-' || str[x] == '+')
	{
		if(str[x] == '-')
			sign = -1;
		x++;
	}
	while(str[x] >= '0' && str[x] <= '9')
	{
		num = (num * 10) + (str[x] - '0');
		x++;
	}
	return(sign * num);
}

int		ft_strlen(char *str)
{
	int x;

	if(!str)
		return(0);
	x = 0;
	while(str[x])
		x++;
	return(x);
}

// wypisuje komunikat bledu i niszcze mutexy

void	destroy_all(char *str, t_program *program, pthread_mutex_t *forks)
{
	int	x;

	x = 0;
	if(str) // jesli "str" czyli pierwszy argument jest niepusty...
	{
		write(2, str, ft_strlen(str)); // to wypisujemy go (ten komunikat bledu)
		write(2, "\n", 1);
	}
	pthread_mutex_destroy(&program->write_lock); // niszcze wszystkie mutexy
	pthread_mutex_destroy(&program->dead_lock);
	pthread_mutex_destroy(&program->meal_lock);
	while(x < program->philos[0].num_of_philos) // petla trwa dopoki "x" jest mniejsze od ilosci filozofow
	{
		pthread_mutex_destroy(&forks[x]); // niszcze mutexy widelcow, ktorych uzywaja filozofowie
		x++;
	}
}

// Ta funkcja zatrzymuje dzialanie danego procesu na okreslona ilosc czasu
// Wykorzystuje ta funkcje, a nie bezposrednio "usleep" poniewaz taki sposob
// zapewnia dokladniejsze opoznienie czasowe, ktore jest krytyczne przy zarzadaniu watkami

int	ft_usleep(size_t miliseconds)
{
	size_t	start;

	start = get_current_time(); // podaje czas w milisekundach
	while((get_current_time() - start) < miliseconds) // dopoki czas ktory uplynal od rozpoczecia (startu) do teraz jest mniejszy od "milisecond" (czyli czasu ktory zadeklarowalismy do czekania)...
		usleep(500); // to czekamy 500 mikrosekund = 0,5 milisekundy; to znaczy ze funkcja czeka krotki czas zanim ponownie sprawdzi warunek
	return(0);	
}

// Sluzy do uzyskania biezacego czasu w milisekundach. Wykorzystuje funkcje "gettimeofday"
// ze standardowej biblioteki C, ktora zwraca czas w strukturze "timeval". Ten czas liczony
// jest od epoki UNIX (1 stycznia 1970). Czyli dostaniemy czas jaki uplynal od tej daty, a nie aktualna godzine.
// Na koncu czas jest przeliczany na milisekundy.
// Struktura czasu:
// 1 sekunda
// = 1000 milisekund
// = 1000000 mikrosekund

size_t	get_current_time(void)
{
	struct timeval	time; // deklaracja struktury o nazwie time

	gettimeofday(&time, NULL); // standardowa funkcja. Pobiera biezacy czas i zapisuje go w strukturze "time" (pierwszy argument); drugi argument jest opcjonalnym wskaznikiem do strefy czasowej (nie uzywam go tutaj)
	if(gettimeofday(&time, NULL) == -1) // jesli funkcja zwroci -1 to znaczy ze to blad, wiec...
		write(2, "gettimeofday() error\n", 22); // ... wypisuje blad
	return(time.tv_sec * 1000 + time.tv_usec / 1000); // wartosci czasu przechowywane w strukturze time sa konwertowane na milisekundy; wiec time.tv_sec, czyli sekundy sa *1000, a time.tv_usec sa /1000
}
