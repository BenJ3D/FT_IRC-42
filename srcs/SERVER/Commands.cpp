/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Commands.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abucia <abucia@student.42lyon.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/07 04:46:30 by abucia            #+#    #+#             */
/*   Updated: 2023/04/10 17:04:31 by abucia           ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "./Server.hpp"

void Server::nick(vector<string> args, int client_fd) {
	cout << ANSI::cyan << client_fd << " --> " << args[0] << endl;

	if (args.size() < 2)
		return Rep().E431(client_fd, _client[client_fd].get_nick());
	string check("[]\\`_^{|}$abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ");
	string new_nick = args[1];
	if (check.find(new_nick[0]) == string::npos || new_nick.length() > 9)
		return Rep().E432(client_fd, _client[client_fd].get_nick(), new_nick);
	check += "0123456789-";
	for (size_t i = 1; i < new_nick.length(); i++)
		if (check.find(new_nick[i]) == string::npos)
			return Rep().E432(client_fd, _client[client_fd].get_nick(), new_nick);
	for (map<int, Client>::iterator it = _client.begin(); it != _client.end(); it++) {
		if ((*it).first == client_fd)
			continue;
		cout << _client.size() << " " << (*it).second.get_nick() << endl;
		if ((*it).second.get_nick() == new_nick)
			return Rep().E433(client_fd, _client[client_fd].get_nick(), new_nick);
	}
	if (!_client[client_fd].get_is_auth() && _client[client_fd].get_username() != "")
	{
		_client[client_fd].now_auth();
		_client[client_fd].set_nick(new_nick);
		cout << ANSI::red << "DEBUG TEST" << ANSI::reset << endl;
		return Rep().R001(client_fd, new_nick);
	}
	string confirm_msg = "NICK " + new_nick + "\r\n";
	confirm_to_client(client_fd, confirm_msg);
	_client[client_fd].set_nick(new_nick);
}


void Server::user(vector<string> args, int cl) {
	cout << ANSI::cyan << cl << " --> " << args[0] << endl;

	if (args.size() < 5)
		return Rep().E461(cl, _client[cl].get_nick(), args[0]);
	if (_client[cl].get_username() != "")
		return Rep().E462(cl, _client[cl].get_nick());
	string check("[]\\`_^{|}$abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789-");
	for (size_t i = 1; i < args[1].length(); i++)
		if (check.find(args[1][i]) == string::npos)
			return Rep().E468(cl, _client[cl].get_nick());

	string realname = "";
	string username = args[1];

	for (size_t i = 3; i < args.size(); i++)
		realname += args[i] + " ";
	if (args[1].size() > 9 || _client[cl].get_username() == "") {
		if (args[1].size() > 9)
			username = args[1].substr(0, 9);
		username = "~" + username;
		notice(cl, "Could not find your ident, using " + username + " instead.");
	}
	this->_client[cl].set_username(username);
	this->_client[cl].set_realname(realname);
}

void Server::ping(vector<string> args, int cl)
{
	cout << ANSI::cyan << cl << " --> " << args[0] << endl;
	if (args.size() < 2)
		return Rep().E409(cl, _client[cl].get_nick());

	if (args.size() == 2)
		return confirm_to_client(cl, "PONG " + string(SERVER_NAME) + " :" + args[1]);

	if (args[1] != SERVER_NAME)
		return Rep().E402(cl, _client[cl].get_nick(), args[1]);
	confirm_to_client(cl, "PONG " + string(SERVER_NAME) + " :" + args[2]);
}



void	Server::join_channel(vector<string> args, int fd_client) //TODO: gerer le cas de multi canaux (ex: JOIN #test #test2 #test3 passwd)
{
	//nouveau channel
	if (_channel.find(args[0]) == _channel.end())
	{
		_channel[args[1]] = Channel(fd_client, args[0], _client[fd_client]);
		confirm_to_client(fd_client, "JOIN " + args[1]);
		confirm_to_client(fd_client, "MODE " + args[1] + " +o " + _client[fd_client].get_nick());

		Rep().R353(fd_client, _client[fd_client].get_nick(), args[1], _client[fd_client].get_nick(), _channel[args[1]].getMode(), _channel[args[1]].getList().at(fd_client));
		Rep().R366(fd_client, _client[fd_client].get_nick(), args[1]);
	}
	else
	{
		// Channel tmp(fd_client, args[0], _client[fd_client]);
		
		string user_list = "";
		for (map<int, char>::iterator it = _channel[args[1]].getList().begin(); it != _channel[args[1]].getList().end(); it++)
			user_list += " " + _client[(*it).first].get_nick();
		user_list = user_list.substr(1);
		
		confirm_to_client(fd_client, "JOIN :" + args[1]);
		
		Rep().R353(fd_client, _client[fd_client].get_nick(), args[1], user_list, _channel[args[1]].getMode(), _channel[args[1]].getList().at(fd_client));
		Rep().R366(fd_client, _client[fd_client].get_nick(), args[1]);
	}
}

/*
:bducrocq!bducrocq@host JOIN #test
:bducrocq!bducrocq@host MODE #test +o bducrocq
:irc.server.com 353 bducrocq = #test :nickname1 nickname2 nickname3
:irc.server.com 366 bducrocq  #test :End of /NAMES list.
*/

void Server::privmsg(vector<string> args, int cl) {
	cout << ANSI::cyan << cl << " --> " << args[0] << endl;
}
