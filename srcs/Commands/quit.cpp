/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   quit.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bducrocq <bducrocq@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/16 22:38:14 by bducrocq          #+#    #+#             */
/*   Updated: 2023/04/26 22:00:03 by bducrocq         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"

void	Server::quit(vector<string> args, int fd_client) {
	this->_delete_client = true;
	vector<string> res = super_split(args[args.size() - 1], 1);
	if (res.size() == 1)
		res.push_back(":Client Quit");
	string msg = res[1].substr(1);
	msg = ":" + _client[fd_client].get_nick() + "!" + _client[fd_client].get_username() + "@" + string(SERVER_NAME) + " QUIT :" + msg + "\r\n";

	for (map<int, Client>::iterator it = _client.begin(); it != _client.end(); it++){
		if (send_to_user(msg, it->first) == -1 )
			send_error(it->first);
	}

	map<string, Channel> tmp = _channel;
	for (map<string, Channel>::iterator it = tmp.begin(); it != tmp.end();it++){ //for all chan
		if (it->second.getList().empty())
			_channel.erase(it->first);
	}
	if (_client.find(fd_client) != _client.end()){
		close(fd_client);
		_client.erase(fd_client);
	}
}
