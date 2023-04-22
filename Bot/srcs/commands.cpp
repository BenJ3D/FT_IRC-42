/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   commands.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amiguez <amiguez@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/11 09:11:21 by amiguez           #+#    #+#             */
/*   Updated: 2023/04/22 17:38:33 by amiguez          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/Bot.hpp"

void Bot::help(){
	string sending("PRIVMSG "+ _channel +" : -- -- Help list\r\n");
	sending += "PRIVMSG "+ _channel +" : - help -> print this list\r\n";
	sending += "PRIVMSG "+ _channel +" : - rps -> a rock paper scisor\r\n";
	sending += "PRIVMSG "+ _channel +" : - part -> leave the channel\r\n";
	sending += "PRIVMSG "+ _channel +" : - quit -> leave the server\r\n";
	sending += "PRIVMSG "+ _channel +" : - ping -> return the ping\r\n";
	if (send_serv(sending) == -1)
		throw(runtime_error("Error: couldnt send help message to server"));
}

void Bot::ping(){
	cout << purple << "in ping" << end;
	string sending("PING This_is_a_ping_test_:D\n\r");
	clock_t start = clock();
	if (send_serv(sending) == -1)
		throw(runtime_error("Error: couldnt send ping message to server"));
	sending = recv_serv();
	cout << sending << end;
	clock_t end = clock();
	if (sending.find("This_is_a_ping_test_:D") == string::npos)
		throw(runtime_error("Error: couldnt receve pong message to server"));
	double time = static_cast<double>(end - start) / CLOCKS_PER_SEC * 1000000;
	cout << purple << "Receved pong !" << time << endl;
	ostringstream os_time ;
	os_time << time;
	sending = "PRIVMSG " + _channel + " :pong : " + os_time.str() + "\r\n";
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

void Bot::bj_bob(){
	send_serv("PRIVMSG " + _channel +" :Bonjour Bob\n\r");
}

void Bot::join(string chan){
	send_serv("LIST\r\n");
	string input_serv ("");
	input_serv = recv_serv();
	vector<string> line;
	while (input_serv.find(":minitel_rose 323 Bob :End of LIST\r\n") == string::npos)
		input_serv += recv_serv();
	line = split_cmd(input_serv, '\n');
	for (size_t i = 0; i < line.size(); i++)
		line[i].erase(line[i].end()-1);
	vector<string>::iterator it = line.begin();
	for (; it != line.end();){
		if ((*it).find("minitel_rose 322 Bob") == string::npos)
			line.erase(it);
		else
			it++;
	}
	for (size_t i = 0; i < line.size(); i++){
		line[i].erase(0, 22);
		line[i].erase(line[i].find(" "), line[i].size()-1);
	}
	if (!chan.empty()){
		cout << "Chan = " << back_black << chan << end;
		for (size_t i = 0; i < line.size();){
			cout << "temp =" << back_green << line[i] << end;
			if (chan == line[i]){
				if (send_serv("JOIN " + chan + "\r\n") == -1)
					cout << red << bold << "ERROR :" << r
						 << red << "Couldn't send Join msg, please retry" << end;
				else {
					_channel = line[i];
					return ;
				}
			}
			else i++;
			if (i == line.size())
				cout << red << bold << "ERROR :" << r
					 << red << "Couldn't Join the channel " << gray << "it doesn t exist anymore :(" << endl << end;
		}
	}
	cout << bold << red << " === " << r << cyan << "Channel List" << bold << red << " === " << end << end;
	for (size_t i = 0; i < line.size(); i++)
		cout << bold << arg << " -- " << gray << i + 1 << " " << blue << line[i] << r << end;
	cout << gray << "    ==============" << end << white << " - Choose a channel to join (1 ~ n) -" << end; 

	string c; 
	if (!std::getline(std::cin, c))
		{_run = false;return;}
	size_t fd = atoi(c.c_str());
	if (fd > 0 && fd <= line.size() )
		join(line[fd - 1]);
	else {
		cout << gray << "input out of range" << end;
		join();
	}
}

void Bot::part(){
	cout << "part" << end;
	cout << _channel << end ;
	send_serv("PART " + _channel + " :BobBot leave :D\r\n");
	_channel.clear();
}

void Bot::quit(){
	// send_serv("PART " + _channel + "\r\n");
	send_serv("QUIT :I'm out baby ~ ~!\r\n");
	_run = false;
}
