/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   quit.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amiguez <amiguez@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/16 22:38:14 by bducrocq          #+#    #+#             */
/*   Updated: 2023/04/19 20:57:49 by amiguez          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"

void	Server::quit(vector<string> args, int fd_client) {
	this->_delete_client = true;
	vector<string> res = super_split(args[args.size() - 1], 1);
	if (res.size() == 1)
		res.push_back("Client Quit");
	int len = res.size();
	cout << "size = " << len << endl;
	cout << "res[0] = " << res[0] << " res1 = " << res[1] << endl;
	string msg = res[1];
	if (msg == "")
		msg = "Client Quit";
	else
		msg = res[1].substr(1);
	for (map<string, Channel>::iterator it = _channel.begin(); it != _channel.end(); it++)
		if (it->second.getClientMode(fd_client) != '0')
			for (map<int, pair<char, vector<string> > >::iterator it2 = it->second.getList().begin(); it2 != it->second.getList().end(); it2++)
				if (it2->first != fd_client)
					it->second.ClientLeave(it2->first, *this, msg, true);
	if (_client.find(fd_client) != _client.end())
		_client.erase(fd_client);
	close(fd_client);
}
