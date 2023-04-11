/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bot.cpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amiguez <amiguez@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/09 06:32:53 by amiguez           #+#    #+#             */
/*   Updated: 2023/04/11 06:28:01 by amiguez          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/Bot.hpp"

Bot::Bot(): _server_fd (-1), _real_name("Bot_bob"), _nick_name("bob"), _auth(false){}

Bot::~Bot(){
	if (_server_fd != -1)
		close(_server_fd);
}

void Bot::init(string ip, string port) throw(exception){

	_server_fd = socket(AF_INET, SOCK_STREAM, 0);
	if(_server_fd == -1)
		throw(std::runtime_error("Error: couldn't create socket"));

	sockaddr_in server_address;
	server_address.sin_family = AF_INET;
	server_address.sin_addr.s_addr = inet_addr(ip.c_str());
	server_address.sin_port = htons(atoi(port.c_str()));

	if (connect(_server_fd, (struct sockaddr *)&server_address, sizeof(server_address)) == -1 ){
		throw (runtime_error("Error: couldn't connect to " + ip + " with port " + port) );
	}
	cout << bold << green <<"Connection established ip : " << ip << end;
}


void Bot::auth(string pasw) throw(exception){
	string s_pass("PASS " + pasw + "\n\r");
	cout << ANSI::back_yellow << _server_fd << end;
	if (send(_server_fd, s_pass.c_str(), s_pass.length(), SOCK_STREAM) == -1 )
		throw(runtime_error("Couldn t send password"));
	string s_nick("NICK bob\n\r");
	string s_user(s_nick + "USER bob 0 * :Bob\n\r");
	if (send(_server_fd, s_user.c_str(), s_user.length(), SOCK_STREAM) == -1 )
		throw(runtime_error("Couldn t send password"));
}


//--------------------------- Getter

bool Bot::is_auth(){return (_auth);}