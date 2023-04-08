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


void	Server::join_channel(vector<string> args, int fd_client) //TODO: gerer le cas de multi canaux (ex: JOIN #test #test2 #test3 passwd)
{
	Channel tmp2(fd_client, args[0], _client[fd_client]);
	cerr << ANSI::red << "DBG error : " << tmp2.getListStr() << ANSI::reset << endl;

	if (_channel.find(args[0]) == _channel.end())
	{
		Channel tmp(fd_client, args[0], _client[fd_client]);
		_channel.insert(pair<string,Channel>(args[0], tmp));
		stringstream ss;
		ss << ":" << _client[fd_client].get_nick() << "!" << _client[fd_client].get_realnick() << "@127.0.0.1" << " JOIN " << args[1] <<"\r\n";

		if(send(fd_client, ss.str().c_str(), strlen(ss.str().c_str()), MSG_CONFIRM) == -1) //replace with send ft
		{
			cerr << ANSI::red << "Erreur lors de l'envoi des données au client" << endl;
			return;
		}
		ss.clear();
		
		ss << ":" << _client[fd_client].get_nick() << "!" << _client[fd_client].get_realnick() << "@127.0.0.1" << " MODE " << args[1] << " +o " << _client[fd_client].get_nick() << "\r\n";
		if(send(fd_client, ss.str().c_str(), strlen(ss.str().c_str()), MSG_CONFIRM) == -1)
		{
			cerr << ANSI::red << "Erreur lors de l'envoi des données au client" << endl;
			return;
		}
		ss.clear();

	}
	else
	{
		Channel tmp(fd_client, args[0], _client[fd_client]);
		stringstream ss;
		ss << ":" << _client[fd_client].get_nick() << "!" << _client[fd_client].get_realnick() << "@127.0.0.1" << " JOIN " << args[1] <<"\r\n"
		<< ":127.0.0.1 353 " << _client[fd_client].get_nick() << " = " << args[1] << tmp.getListStr() << "\r\n"
		<< ":127.0.0.1 366 " << _client[fd_client].get_nick() << "  " << args[1] << " :End of /NAMES list." << "\r\n";

		cerr << ANSI::red << ss.str().c_str() << ANSI::reset << endl;

		if(send(fd_client, ss.str().c_str(), strlen(ss.str().c_str()), MSG_CONFIRM) == -1) //replace with send ft
		{
			cerr << ANSI::red << "Erreur lors de l'envoi des données au client" << endl;
			return;
		}
		ss.clear();
	}
}
/*
:bducrocq!bducrocq@host JOIN #test
:bducrocq!bducrocq@host MODE #test +o bducrocq
:irc.server.com 353 bducrocq = #test :nickname1 nickname2 nickname3
:irc.server.com 366 bducrocq  #test :End of /NAMES list.
*/