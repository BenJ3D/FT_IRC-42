/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Commands.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bducrocq <bducrocq@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/07 04:46:30 by abucia            #+#    #+#             */
/*   Updated: 2023/04/11 04:52:53 by bducrocq         ###   ########lyon.fr   */
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
		notice(cl, "Could not find your ident, using " + username + " instead.");
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

void Server::ping(vector<string> args, int cl)
{
	cout << ANSI::cyan << cl << " --> " << args[0] << endl;
	if (args.size() < 2)
		return Rep().E409(cl, _client[cl].get_nick());

	if (args.size() == 2)
		return confirm_to_client(cl, "PONG " + string(SERVER_NAME) + " :" + args[1], _client);

	if (args[1] != SERVER_NAME)
		return Rep().E402(cl, _client[cl].get_nick(), args[1]);
	confirm_to_client(cl, "PONG " + string(SERVER_NAME) + " :" + args[2], _client);
}



void	Server::join_channel(vector<string> args, int fd_client) //TODO: gerer le cas de multi canaux (ex: JOIN #test #test2 #test3 passwd)
{
	
	if (args.size() < 2)
		return Rep().E461(fd_client, _client[fd_client].get_nick(), args[0]);

	if (_channel.find(args[1]) == _channel.end())
	{
		_channel[args[1]] = Channel(fd_client, args[1]);
		//_channel[args[1]].addClient(fd_client, '@');
		confirm_to_client(fd_client, "JOIN " + args[1], _client);
		confirm_to_client(fd_client, "MODE " + args[1] + " +o " + _client[fd_client].get_nick(), _client);

		Rep().R353(fd_client, _client[fd_client].get_nick(), args[1], _client[fd_client].get_nick(), _channel[args[1]].getMode(), _channel[args[1]].getList().at(fd_client).first);
		Rep().R366(fd_client, _client[fd_client].get_nick(), args[1]);
	}
	else
	{
		for (vector<int>::iterator it = _channel[args[1]].getBlackList().begin(); it != _channel[args[1]].getBlackList().end(); it++)
			if ((*it) == fd_client)
				return Rep().E474(fd_client, _client[fd_client].get_nick(), args[1]);
		
		_channel[args[1]].addClient(fd_client, ' ');
		confirm_to_client(fd_client, "JOIN " + args[1], _client);
		string user_list = _channel[args[1]].ListNick(_client, fd_client);
		Rep().R353(fd_client, _client[fd_client].get_nick(), args[1], user_list, _channel[args[1]].getMode(), _channel[args[1]].getList().at(fd_client).first);
		Rep().R366(fd_client, _client[fd_client].get_nick(), args[1]);
		cerr << ANSI::red << "DEBUG TEST USER LIST = "  << user_list << ANSI::reset << endl;

	}
}

void Server::mode(vector<string> args, int fd_client) {
	cout << ANSI::cyan << fd_client << " --> " << args[0] << endl;

	if (args.size() < 3)
		return Rep().E461(fd_client, _client[fd_client].get_nick(), args[0]);
	
	if (args[1][0] != '#' || _channel.find(args[1]) == _channel.end())
		return Rep().E403(fd_client, _client[fd_client].get_nick(), args[1]);
	
	string check = "ov";
	if (string("+-").find(args[2][0]) == string::npos && args[2].length() < 2)
		return Rep().E472(fd_client, _client[fd_client].get_nick(), args[2][0]);
	size_t i = 1;
	for (; i < args[2].length(); i++)
		if (check.find(args[2][i]) == string::npos)
			return Rep().E472(fd_client, _client[fd_client].get_nick(), args[2][i]);
	if (i != args.size() - 2)
		return Rep().E461(fd_client, _client[fd_client].get_nick(), args[0]); // OU 401 ?

	for (size_t i = 1; i < args[2].length(); i++)
	{
		if (args[2][i] == 'o')
		{
			if (args[2][0] == '+')
				_channel[args[1]].addClient(fd_client, '@');
			else if (args[2][0] == '-')
				_channel[args[1]].addClient(fd_client, ' ');
		}
		else if (args[2][i] == 'v')
		{
			if (args[2][0] == '+')
				_channel[args[1]].addClient(fd_client, '+');
			else if (args[2][0] == '-')
				_channel[args[1]].addClient(fd_client, ' ');
		}
	}
}
/*
:bducrocq!bducrocq@host JOIN #test
:bducrocq!bducrocq@host MODE #test +o bducrocq
:irc.server.com 353 bducrocq = #test :nickname1 nickname2 nickname3
:irc.server.com 366 bducrocq  #test :End of /NAMES list.
*/

void	Server::privmsg(vector<string> args, int cl) {
	cout << ANSI::cyan << cl << " --> " << args[0] << endl;
	if (args.size() < 3)

		return Rep().E411(cl, _client[cl].get_nick(), args[0]);
}

void	Server::list(vector<string> args, int fd_client)
{
	if (args.size() != 1)
		return Rep().E409(fd_client, _client.at(fd_client).get_nick());
	Rep().R321(fd_client, _client.at(fd_client).get_nick());
	map<string,Channel>::iterator	it = _channel.begin();

	for(; it != _channel.end(); ++it)
	{
		Rep().R322(fd_client, _client.at(fd_client).get_nick(), it->second.getNbClient(), it->second.getTopic(), it->second.getName());
	}
	Rep().R323(fd_client, _client.at(fd_client).get_nick());
}

void Server::topic(vector<string> args, int fd_client)
{
	if (args.size() == 2)
	{
		if (_channel.at(args[1]).getTopic().empty())
			Rep().R331(fd_client, _client.at(fd_client).get_nick(), args[1]);
		else
		{
			cerr << ANSI::green << ANSI::bold << "args[1] = " + args[1] << " getTopic = : " + _channel.at(args[1]).getTopic() << ANSI::reset << endl;
			Rep().R332(fd_client, _client.at(fd_client).get_nick(),  args[1], ":" + _channel.at(args[1]).getTopic());
			Rep().R333(fd_client, _client.at(fd_client).get_nick(), _channel.at(args[1]).getName(), \
			_client.at(fd_client).get_nick() + "!" + _client.at(fd_client).get_username() + "@" + string(SERVER_NAME), time(0));
			// ":" + _client[fd].get_nick() + "!" + _client[fd].get_username() + "@" + string(SERVER_NAME)
		}
	}
	else if (args.size() == 3)
	{
		_channel.at(args[1]).setTopic(args[2].substr(1));
		cerr << ANSI::green << ANSI::bold << "Args 1 = " + args[1] << " 2 = " + args[2] << ANSI::reset << endl;
		confirm_to_client(fd_client, "TOPIC " + _channel.at(args[1]).getName() + " :" + args[2], _client);
	}
	else
		Rep().E409(fd_client, _client.at(fd_client).get_nick());
}

//TOPIC #tt :BLABLA
