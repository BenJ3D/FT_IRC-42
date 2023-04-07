/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Commands.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abucia <abucia@student.42lyon.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/07 04:46:30 by abucia            #+#    #+#             */
/*   Updated: 2023/04/07 05:23:29 by abucia           ###   ########lyon.fr   */
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
		if ((*it).second.get_nick() == new_nick)
		{
			string error_msg = "ERROR :Nickname is already in use.";
			send(client_fd, error_msg.c_str(), strlen(error_msg.c_str()), 0);
			return;
		}
	}

	// Attribution du nouveau pseudo au client
	_client[client_fd].set_nick(new_nick);
	// Envoi d'un message de confirmation
	string confirm_msg = "Your nickname has been changed to " + new_nick;
	send(client_fd, confirm_msg.c_str(), strlen(confirm_msg.c_str()), 0);
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