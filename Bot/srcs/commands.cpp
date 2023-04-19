/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   commands.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amiguez <amiguez@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/11 09:11:21 by amiguez           #+#    #+#             */
/*   Updated: 2023/04/18 16:30:12 by amiguez          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/Bot.hpp"

void Bot::init_cmds(){
	m_cmd["help"] = &Bot::help;
	m_cmd["ping"] = &Bot::ping;
	m_cmd["rps"] = &Bot::rps;
	m_cmd["quit"] = &Bot::quit;
}

void Bot::help(string input){
	(void) input;
	string sending("PRIVMSG "+ _channel +" : -- -- Help list\r\n");
	sending += "PRIVMSG "+ _channel +" : - help -> print this list\r\n";
	sending += "PRIVMSG "+ _channel +" : - rps -> a rock paper scisor\r\n";
	sending += "PRIVMSG "+ _channel +" : - quit -> leave the channel\r\n";
	sending += "PRIVMSG "+ _channel +" : - ping -> return the ping\r\n";
	if (send_serv(sending) == -1)
		throw(runtime_error("Error: couldnt send help message to server"));
}

void Bot::ping(string input){
	(void) input;
	string sending("PING : This_is_a_ping_test_:D\n\r");
	clock_t start = clock();
	if (send_serv(sending) == -1)
		throw(runtime_error("Error: couldnt send ping message to server"));
	sending = recv_serv();
	cout << sending << endl;
	clock_t end = clock();
	if (sending.find("This_is_a_ping_test_:D") == string::npos)
		throw(runtime_error("Error: couldnt receve pong message to server"));
	double time = static_cast<double>(end - start) / CLOCKS_PER_SEC * 1000000;
	ostringstream os_time ;
	os_time << time;
	sending = "PRIVMSG " + _channel + "pong: " + os_time.str();
	send_serv(sending);
}

void Bot::rps(string input){
	string sending("");
	if (input.find("paper\r\n") != string::npos)
		sending = "PRIVMSG " + _channel + " :Ahah I took Cissors!\r\n";
	else if (input.find("rock\r\n") != string::npos)
		sending = "PRIVMSG " + _channel + " :Ahah I took Paper!\r\n";
	else if (input.find("cissors\r\n") != string::npos)
		sending = "PRIVMSG " + _channel + " :Ahah I took Rock ( & STONE )!\r\n";
	else
		sending = "PRIVMSG " + _channel + " :You need to say 'paper' 'cissors' or 'rock' to play :(\r\n";
	if (send_serv(sending) == -1)
		throw(runtime_error("Error: couldnt send rps message to server"));
}
void Bot::quit(string input){
	(void) input;
}
