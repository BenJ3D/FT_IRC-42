/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bducrocq <bducrocq@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/22 01:27:56 by bducrocq          #+#    #+#             */
/*   Updated: 2023/03/29 01:26:53 by bducrocq         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "SERVER/Server.hpp"

int	main( int ac, char **av)
{
	(void)ac;
	cout << ANSI::purple << "₩ΞŁϾØΜΞ ŦØ ƑṲϾӃ_ḮŘϾ" << ANSI::white << endl;

	//*******provisoire, pour DEBUG************/
	if (av[1])								/**/
		Server irc(av[1], "127.0.0.1"); 	/**/
	else 									/**/
		Server irc("6667", "127.0.0.1");	/**/
	//*****************************************/
	//*****passage sur branch server***********/

	return 0;
}