/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Commands.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bducrocq <bducrocq@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/07 04:46:30 by abucia            #+#    #+#             */
/*   Updated: 2023/04/14 02:46:26 by bducrocq         ###   ########lyon.fr   */
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
		Rep().R001(client_fd, new_nick);
		Rep().R002(client_fd, new_nick, string(SERVER_NAME), string(SERVER_VERSION));
		return Rep().R003(client_fd, new_nick ,string(SERVER_DATE));
	}
	string confirm_msg = "NICK " + new_nick + "\r\n";
	confirm_to_client(client_fd, confirm_msg, _client);
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
		notice(cl, "*** Could not find your ident, using " + username + " instead.");
	}
	if (!_client[cl].get_is_auth() && _client[cl].get_nick() != "*")
	{
		_client[cl].now_auth();
		this->_client[cl].set_username(username);
		this->_client[cl].set_realname(realname);
		Rep().R001(cl, _client[cl].get_nick());
		Rep().R002(cl, _client[cl].get_nick(), string(SERVER_NAME), string(SERVER_VERSION));
		return Rep().R003(cl, _client[cl].get_nick(), string(SERVER_DATE));
	}
	this->_client[cl].set_username(username);
	this->_client[cl].set_realname(realname);
}

void Server::privmsg(vector<string> args, int client_fd) {
	cout << ANSI::cyan << client_fd << " --> " << args[0] << endl;
	if (args.size() < 3)
		return Rep().E411(client_fd, _client[client_fd].get_nick(), args[0]);
	
	if (args[1][0] == '#')
	{
		if (_channel.find(args[1]) == _channel.end())
			return Rep().E403(client_fd, _client[client_fd].get_nick(), args[1]);
		for (map<int, pair<char, vector<string> > >::iterator it = _channel[args[1]].getList().begin(); it != _channel[args[1]].getList().end(); it++)
			if (it->first != client_fd)
				confirm_to_client(it->first, "PRIVMSG " + args[1] + " :" + args[2], _client);
	}
	else
	{
		int dst_fd = Client().find_user_by_nick(args[1], _client);
		if (dst_fd == -1)
			return Rep().E401(client_fd, _client[client_fd].get_nick(), args[1]);
		confirm_to_client(dst_fd, "PRIVMSG " + args[1] + " :" + args[2], _client);
		confirm_to_client(client_fd, "PRIVMSG " + args[1] + " :" + args[2], _client);
	}
}
