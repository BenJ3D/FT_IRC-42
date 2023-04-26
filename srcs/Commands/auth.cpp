/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   auth.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abucia <abucia@student.42lyon.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/16 22:37:26 by bducrocq          #+#    #+#             */
/*   Updated: 2023/04/23 03:31:39 by abucia           ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"

void	Server::pass(vector<string> args, int cl_fd) {
	if (_client[cl_fd].get_pass_confirm())
		return Rep().E462(cl_fd, _client[cl_fd].get_nick());
	if (args.size() == 1)
		return Rep().E461(cl_fd, _client[cl_fd].get_nick(), "PASS");
	if (args[1] == this->_pass_word)
		_client[cl_fd].now_pass();
}

void Server::nick(vector<string> args, int client_fd) {
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
		if ((*it).second.get_nick() == new_nick)
			return Rep().E433(client_fd, _client[client_fd].get_nick(), new_nick);
	}
	if (!_client[client_fd].get_is_auth() && _client[client_fd].get_username() != "")
		_client[client_fd].now_auth(*this);
	string msg = ":" + get_client()[client_fd].get_nick() + "!" + get_client()[client_fd].get_username() + "@" + string(SERVER_NAME) + " NICK " + new_nick + "\r\n";
	if (!_client[client_fd].get_is_auth()){
		if (send_to_user(msg, client_fd) == -1)
			send_error(client_fd);
		_client[client_fd].set_nick(new_nick);
		return ;
	}
	for (map<int, Client>::iterator it = _client.begin(); it != _client.end(); it++)
		if (send_to_user(msg, it->first) == -1)
			send_error(it->first);
	_client[client_fd].set_nick(new_nick);
}

void Server::user(vector<string> args, int cl) {
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
		_client[cl].now_auth(*this);
	this->_client[cl].set_username(username);
	this->_client[cl].set_realname(realname);
}
