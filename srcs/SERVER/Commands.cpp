/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Commands.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abucia <abucia@student.42lyon.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/07 04:46:30 by abucia            #+#    #+#             */
/*   Updated: 2023/04/08 09:53:19 by abucia           ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "./Server.hpp"

void Server::nick(vector<string> args, int client_fd) {
	cout << ANSI::cyan << client_fd << " --> " << args[0] << endl;

	if (args.size() < 2)
		return Rep().E431(client_fd, _client[client_fd].get_nick());
	string check("[]\\`_^{|}abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ");
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
	if (_client[client_fd].get_fisrt_connection())
	{
		_client[client_fd].set_first_connection(false);
		_client[client_fd].set_nick(new_nick);
		return Rep().R001(client_fd, new_nick);
	}
	string confirm_msg = "NICK " + new_nick + "\r\n";
	//cout << ANSI::gray << "{send} ==> " << ANSI::cyan <<  _client[client_fd].get_nick() << " " << confirm_msg;
	confirm_to_client(client_fd, confirm_msg);
	_client[client_fd].set_nick(new_nick);
}


void Server::user(vector<string> args, int cl) {
	cout << ANSI::cyan << cl << " --> " << args[0] << endl;
	this->_client[cl].set_realnick(args[1]);
	string msg(":");
	msg += args[0] + " " + args[1] + "\r\n";
	//send(cl, msg.c_str(), strlen(msg.c_str()), 0);
	//Rep(*this).R001(cl, args[1]);
}

void Server::ping(vector<string> args, int cl) {
	cout << ANSI::cyan << cl << " --> " << args[0] << endl;
	string msg("PONG ");
	msg += args[1] + "\r\n";
	send(cl, msg.c_str(), strlen(msg.c_str()), 0);
	// Rep(*this).R001(cl, args[1]);
}