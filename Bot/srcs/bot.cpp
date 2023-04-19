/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bot.cpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amiguez <amiguez@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/09 06:32:53 by amiguez           #+#    #+#             */
/*   Updated: 2023/04/19 21:15:00 by amiguez          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/Bot.hpp"

Bot::Bot(): _server_fd (-1), _real_name("Bot_bob"), _nick_name("Bob"), _run(true){}

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

	if (connect(_server_fd, (struct sockaddr *)&server_address, sizeof(server_address)) == -1)
		throw (runtime_error("Error: couldn't connect to " + ip + " with port " + port) );

	cout << bold << green <<"Connection established ip : " << ip << end;
	_ip = ip;
}


void Bot::auth(string pasw) throw(exception){
	string s_pass("PASS " + pasw + "\r\n");  //  check PASS
	if (send_serv(s_pass) == -1 )
		throw(runtime_error("Couldn t send password"));
	string s_nick("NICK " + _nick_name + "\r\n");  //  check NICK
	if (send_serv(s_nick) == -1 )
		throw(runtime_error("Couldn t send nick command"));
	string ret_nick = recv_serv();
	if (ret_nick != ":*!@minitel_rose NICK Bob\r\n")
		throw(runtime_error("Couldn't chose my NickName"));
	string s_user("USER " + _nick_name + " 0 * :" + _real_name + "\r\n");  //  Check USER
	if (send_serv(s_user) == -1 )
		throw(runtime_error("Couldn t send the USER command"));
	bool done = true;
	string input("");
	vector<string> inputs;
	while (done){
		input = recv_serv();
		if (input.find(":minitel_rose 003 Bob :") != string::npos)
			done = false;
	} cout <<bold << green << "Authentification Done !" << endl;
}

void Bot::run() {
	string input;
	init_cmds();
	cout << trailing << back_green << input << end;
	while (_run){
		if (!_channel.empty()){
			cout << gray << "in " << _channel << end;
			input = recv_serv();
			try {

			if (input.find("Bob.help") != string::npos)
				help(input);
			if (input.find("Bob.ping") != string::npos)
				ping(input);
			if (input.find("Bob.rps") != string::npos)
				rps(input);
			// if (input.find("Bob.quit") != string::npos)
			// 	quit(input);
			// if (input.find("Bob.part") != string::npos)
			// 	part(input);

			} catch (exception &e){
				cout << red << e.what() << end;
			}
		}
		else
			join();
	}
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
		size_t c = 0;
		for (size_t i = 0; i < line.size(); i++){
			if (chan == line[i]){
				if (send_serv("JOIN " + chan + "\r\n") == -1)
					cout << red << bold << "ERROR :" << r
						 << red << "Couldn't send Join msg, please retry" << end;
				else {
					_channel = line[i];
					return ;
				}
			}
			else c++;
			if (c == line.size())
				cout << red << bold << "ERROR :" << r
					 << red << "Couldn't Join the channel " << gray << "(it doesn t exist anymore :( )" << endl << end;
		}
	}
	cout << bold << red << " === " << r << cyan << "Channel List" << bold << red << " === " << end << end;
	for (size_t i = 0; i < line.size(); i++)
		cout << bold << arg << " -- " << gray << i + 1 << " " << blue << line[i] << r << end;
	cout << gray << "    ==============" << end << white << "  Choose a channel to join (1 ~ n)" << end; 
	string c; 
	if (!std::getline(std::cin, c))
		{_run = false;return;}
	cout << red << c << r <<endl;
	size_t fd = atoi(c.c_str());
	cout << yellow << "fd = " << fd << end;
	if (fd != 0 && fd <= line.size() )
		join(line[fd - 1]);
	else {
		cout << gray << "input out of range" << end;
		join();
	}
}

//--------------------------- Misc ----------------------------------------

int Bot::send_serv(string input){
	return (send(_server_fd, input.c_str(), input.length(), SOCK_STREAM));
}
string Bot::recv_serv(){
	char buff[1024] = {0};
	recv(_server_fd, buff, 1024, 0);
	return (string(buff));
}

