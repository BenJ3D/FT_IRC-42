/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Commands.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abucia <abucia@student.42lyon.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/07 04:46:30 by abucia            #+#    #+#             */
/*   Updated: 2023/04/07 05:14:37 by abucia           ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "./Server.hpp"

void Server::nick(vector<string> args, int cl) {
	cout << ANSI::cyan << cl << " --> " << args[0] << endl;
	this->_client[cl].set_nick(args[1]);
	string msg("");
	msg += args[0] + " " + args[1] + "\r\n";
	//send(cl, msg.c_str(), strlen(msg.c_str()), 0);
	Rep(*this).R001(cl, args[1]);
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