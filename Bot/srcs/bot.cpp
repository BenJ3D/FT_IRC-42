/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bot.cpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amiguez <amiguez@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/09 06:32:53 by amiguez           #+#    #+#             */
/*   Updated: 2023/04/10 08:42:03 by amiguez          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Bot.hpp"

Bot::Bot(): _server_fd (-1){}

Bot::~Bot(){
	if (_server_fd != -1)
		close(_server_fd);
}

void Bot::init(string ip, string port) throw(exception){

	int _server_fd = socket(AF_INET, SOCK_STREAM, 0);
	if(_server_fd == -1)
		throw(std::runtime_error("Error: couldn't create socket"));
	
	// struct sockaddr_in address_serv;
	sockaddr_in server_address;
	server_address.sin_family = AF_INET;
	server_address.sin_addr.s_addr = inet_addr(ip.c_str());
	server_address.sin_port = htons(atoi(port.c_str()));

	if (connect(_server_fd, (struct sockaddr *)&server_address, sizeof(server_address)) == -1 ){
		throw (runtime_error("Error: couldn't connect to " + ip + " with port " + port) );
	}
	cout << ANSI::bold << ANSI::green <<"Connection established with the server !" << ANSI::r << endl;
	while (1);
}
