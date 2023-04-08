/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amiguez <amiguez@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/22 01:27:56 by bducrocq          #+#    #+#             */
/*   Updated: 2023/04/08 06:38:37 by amiguez          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "SERVER/Server.hpp"

int	main( int ac, char **av)
{
	(void)ac;
	cout << ANSI::purple << "₩ΞŁϾØΜΞ ŦØ ƑṲϾӃ_ḮŘϾ" << ANSI::white << endl;

	//*******provisoire, pour DEBUG****************/
	if (ac == 3)								/**/
		Server irc(av[1], av[2]);				/**/
	else if (ac == 2)							/**/
		Server irc(av[1], "password");			/**/
	else										/**/
		Server irc("6667", "password");			/**/
	//*********************************************/
	//*****passage sur branch server***************/

	return 0;
}