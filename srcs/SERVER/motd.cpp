/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   motd.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amiguez <amiguez@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/17 17:41:10 by amiguez           #+#    #+#             */
/*   Updated: 2023/04/17 21:06:41 by amiguez          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"

/**
 *		RPL_MOTDSTART	R375
 *		RPL_MOTD		R372
 *		RPL_ENDOFMOTD	R376
 *		ERR_NOMOTD		E422
 */
void Server::motd(vector<string> args, int cl){
	(void) args;
	std::ifstream motd_file(_motd.c_str());
	if (!motd_file.is_open())
		return Rep().E422(cl, _client[cl].get_nick());
	string line;
	Rep().R375(cl, _client[cl].get_nick(), SERVER_NAME);
	while(getline(motd_file, line)){
		Rep().R372(cl, _client[cl].get_nick(), line);
	}
	Rep().R376(cl, _client[cl].get_nick());
}
void Server::motd_auth(int cl){
	std::ifstream motd_file(_motd.c_str());
	if (!motd_file.is_open())
		return Rep().E422(cl, _client[cl].get_nick());
	string line;
	Rep().R375(cl, _client[cl].get_nick(), SERVER_NAME);
	while(getline(motd_file, line)){
		Rep().R372(cl, _client[cl].get_nick(), line);
	}
	Rep().R376(cl, _client[cl].get_nick());
}
