/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Commands.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abucia <abucia@student.42lyon.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/07 04:46:30 by abucia            #+#    #+#             */
/*   Updated: 2023/04/08 04:48:41 by abucia           ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "./Server.hpp"

// void Server::nick(vector<string> args, int cl) {
// 	cout << ANSI::cyan << cl << " --> " << args[0] << endl;
// 	this->_client[cl].set_nick(args[1]);
// 	string msg("");
// 	msg += args[0] + " " + args[1] + "\r\n";
// 	send(cl, "", 0, 0);
// 	//Rep(*this).R001(cl, args[1]);
// }

void Server::nick(vector<string> args, int client_fd) {
	cout << ANSI::cyan << client_fd << " --> " << args[0] << endl;
	cout << endl;
	// Vérification des arguments de la commande
	if (args.size() < 2)
	{
		cerr << "ERROR: Not enough arguments for NICK command." << endl;
		return;
	}

	// Récupération du nouveau pseudo
	string new_nick = args[1];

	// Vérification que le pseudo n'est pas déjà pris
	for (map<int, Client>::iterator it = _client.begin(); it != _client.end(); it++) {
		cout.flush();
		//cout << "hello mine turtle" << endl;
		cout << _client.size() << " " << (*it).second.get_nick() << endl;
		if ((*it).second.get_nick() == new_nick)
		{
			string error_msg = "ERROR :Nickname is already in use.";
			send(client_fd, error_msg.c_str(), strlen(error_msg.c_str()), 0);
			return;
		}
	}

	if (_client[client_fd].get_fisrt_connection() == true)
	{
		_client[client_fd].set_first_connection(false);
		_client[client_fd].set_nick(new_nick);
		Rep().R001(client_fd, new_nick);
		return;
	}

	// Envoi d'un message de confirmation
	string confirm_msg = ":" + _client[client_fd].get_nick() + "!" + _client[client_fd].get_realnick() + "@127.0.0.1 NICK " + new_nick + "\r\n";
	cout << ANSI::gray << "{send} => " << ANSI::cyan << confirm_msg << endl;
	send(client_fd, confirm_msg.c_str(), strlen(confirm_msg.c_str()), 0);

	// Attribution du nouveau pseudo au client
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