/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bducrocq <bducrocq@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/22 01:27:56 by bducrocq          #+#    #+#             */
/*   Updated: 2023/04/25 16:06:22 by bducrocq         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "SERVER/Server.hpp"

int	main( int ac, char **av)
{
	(void)ac;
	cout << ANSI::purple << "₩ΞŁϾØΜΞ ŦØ ℳíиí₸ΞḶ ⱤØṨΞ ḮŘϾ" << ANSI::white << endl;

	if (ac == 3)
		Server irc(av[1], av[2]);
	return 0;
}