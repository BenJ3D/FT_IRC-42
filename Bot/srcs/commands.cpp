/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   commands.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amiguez <amiguez@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/11 09:11:21 by amiguez           #+#    #+#             */
/*   Updated: 2023/04/11 14:45:17 by amiguez          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/Bot.hpp"

void Bot::init_cmds(){
	m_cmd["help"] = &Bot::help;
	m_cmd["ping"] = &Bot::ping;
	m_cmd["rps"] = &Bot::rps;
	m_cmd["quit"] = &Bot::quit;
}

void Bot::help(string input){}

void Bot::ping(string input){
	(void) input;
	string sending("PING :This is a ping test :D\n\r");
	clock_t start = clock();
	if (send_serv(sending) == -1)
		throw(runtime_error("Error: couldnt send ping message to server"));
	sending = recv_serv();
	clock_t end = clock();
		throw(runtime_error("Error: couldnt receve pong message to server"));
	if (sending.find("PONG :This is a ping test :D\n\r") == string::npos)
		throw(runtime_error("Error: couldnt receve pong message to server"));
	double time = static_cast<double>(end - start) / CLOCKS_PER_SEC * 1000000;
	ostringstream os_time ;
	os_time << time;
	sending = "PRIVMSG " + _channel + "pong: " + os_time.str();
	send_serv(sending);
}

void Bot::rps(string input){}
void Bot::quit(string input){}
