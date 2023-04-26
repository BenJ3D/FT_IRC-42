/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bot.cpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amiguez <amiguez@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/09 06:32:53 by amiguez           #+#    #+#             */
/*   Updated: 2023/04/26 15:23:11 by amiguez          ###   ########.fr       */
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
	cout << trailing << back_green << input << end;
	while (_run){
		if (!_channel.empty()){
			cout << gray << "in " << _channel << end;
			input = recv_serv();
			try {

				if (input.find("Bob.help") != string::npos)
					help();
				if (input.find("Bob.ping") != string::npos)
					ping();
				if (input.find("Bob.rps") != string::npos)
					rps(input);
				if (input.find("Dit bonjour Bob") != string::npos)
					bj_bob();
				if (input.find("Bob.part") != string::npos)
					part();
				if (input.find("Bob.quit") != string::npos)
					quit();
			} catch (exception &e){
				cout << red << e.what() << end;
			}
		}
		else
			join();
	}
	cout << red << "I'm out ! Bye Bye ~ ~" << end;
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

