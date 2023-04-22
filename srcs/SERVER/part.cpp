/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   part.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amiguez <amiguez@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/16 22:37:26 by bducrocq          #+#    #+#             */
/*   Updated: 2023/04/22 19:04:11 by amiguez          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"

/*
	ERR_NEEDMOREPARAMS	461
	ERR_NOSUCHCHANNEL	403
	ERR_NOTONCHANNEL	442
*/
void	Server::part(vector<string> args, int fd_client) {
	if (args.size() < 3)
		return Rep().E461(fd_client, _client[fd_client].get_nick(), "PART");
	vector<string> res = super_split(args[args.size() - 1], 2);
	// cout << "res[0] = " << res[0] << " res1 = " << res[1] << endl; // Segfault if there is res[2] for if no args
	vector<string> listChan = split_cmd(res[1], ',');
	if (listChan.size() == 0)
		listChan.push_back(res[1]);
	for (size_t i = 0; i < listChan.size(); i++) {
		if (listChan[i][0] != '#' || \
		_channel.find(listChan[i]) == _channel.end())
		{
			Rep().E403(fd_client, _client[fd_client].get_nick(), listChan[i]);
			continue ;
		}
		else if (_channel[listChan[i]].getClientMode(fd_client) == '0')
		{
			Rep().E442(fd_client, _client[fd_client].get_nick(), listChan[i]);
			continue ;
		}
		// string msg("PART " + listChan[i] + "");
		// confirm_to_all_channel_client(fd_client, msg, *this, _channel[listChan[i]]);
		if (res.size() == 2)
			_channel[listChan[i]].ClientLeave(fd_client, *this, "", false);
		else if (res.size() > 2)
			_channel[listChan[i]].ClientLeave(fd_client, *this, res[2], false);
		if (_channel[listChan[i]].getList().empty()){
			cout << ANSI::blue << "chan erase" << endl;
			_channel.erase(listChan[i]);
		}
	}
}