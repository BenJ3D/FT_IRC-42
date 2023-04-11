/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amiguez <amiguez@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/09 06:32:57 by amiguez           #+#    #+#             */
/*   Updated: 2023/04/11 06:03:26 by amiguez          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/Bot.hpp"

int main(int ac, char **av){

	Bot Bob;

	char **po = av;
	try{
		switch (ac)
		{
		case 1:{
			throw(runtime_error(USAGE));
			break;
			}
		case 2:{
			Bob.init();
			break;
			}
		case 3:{
			Bob.init(string (av[2]));
			break;
			}
		case 4:{
			Bob.init(string (av[2]), string (po[3]));
			break;
			}
		default :{
			throw(runtime_error(USAGE));
			}
		}
		Bob.auth(av[1]);
	}
	catch(exception &e){cout << ANSI::red << e.what() << endl; return (1);}

	while (1);
}