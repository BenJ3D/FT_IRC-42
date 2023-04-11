/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amiguez <amiguez@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/09 06:32:57 by amiguez           #+#    #+#             */
/*   Updated: 2023/04/10 08:46:01 by amiguez          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Bot.hpp"

int main(int ac, char **av){

	Bot Bob;

	char **po = av;
	try{
		switch (ac)
		{
		case 1:{
			Bob.init();
			break;
			}
		case 2:{
			Bob.init(string (av[1]));
			break;
			}
		case 3:{
			Bob.init(string (av[1]), string (po[2]));
			break;
			}
		default :{
			throw(runtime_error(USAGE));
			}
		}
	}
	catch(exception &e){cout << ANSI::red << e.what() << ANSI::r << endl;}

}