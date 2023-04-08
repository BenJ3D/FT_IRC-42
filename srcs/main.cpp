/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amiguez <amiguez@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/22 01:27:56 by bducrocq          #+#    #+#             */
/*   Updated: 2023/04/08 03:51:40 by amiguez          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"

int	main( int ac, char **av)
{

	std::cout << ANSI::purple << "₩ΞŁϾØΜΞ ŦØ ƑṲϾӃ_ḮŘϾ" << std::endl;

	//*******provisoire, pour DEBUG************/
	if (av[1])								/**/
		Server irc(av[1], "127.0.0.1"); 	/**/
	else 									/**/
		Server irc("6667", "127.0.0.1");	/**/
	//*****************************************/
	//*****passage sur branch server***********/

	return 0;
}