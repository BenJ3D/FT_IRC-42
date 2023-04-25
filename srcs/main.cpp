/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amiguez <amiguez@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/22 01:27:56 by bducrocq          #+#    #+#             */
/*   Updated: 2023/04/25 19:04:28 by amiguez          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"

int	main( int ac, char **av)
{
	cout << ANSI::purple << "₩ΞŁϾØΜΞ ŦØ ℳíиí₸ΞḶ ⱤØṨΞ ḮŘϾ" << ANSI::r << endl;

	if (ac == 3)
		Server irc(av[1], av[2]);
	else 
		cout << ANSI::red << "Usage : ./ircserv <port> <password>" << endl;
	return 0;
}