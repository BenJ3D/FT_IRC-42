/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   privmsg.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bducrocq <bducrocq@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/06 20:39:56 by abucia            #+#    #+#             */
/*   Updated: 2023/04/26 23:47:04 by bducrocq         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"

void Server::privmsg(vector<string> args, int client_fd) {
	if (args[args.size() - 1][0] == '\n')
		args[args.size() - 1] = args[args.size() - 1].substr(1);

	string msg = " ";
	vector<string> res = super_split(args[args.size() - 1], 2); // <CMD> <TARGET> :<MSG>
	if (res.size() < 3)
	{
		if (res.size() == 2)
			return Rep().E412(client_fd, _client[client_fd].get_nick());
		else
			return Rep().E411(client_fd, _client[client_fd].get_nick(), args[0]);
	}
	vector<string> target_list = split_cmd(res[1], ',');
	if (target_list.size() == 0)
		target_list.push_back(res[1]);

	for (unsigned long int i = 0; i < target_list.size(); i++)
	{
		target_list[i] = trim(target_list[i]);
		if (target_list[i][0] == '#')
		{
			if (_channel.find(target_list[i]) == _channel.end())
			{
				Rep().E403(client_fd, _client[client_fd].get_nick(), target_list[i]);
				continue;
			}

			// verifie si le client est pas dans la blacklist du channel ou si channel a un mdp et que le client n'est pas dans le channel
			if (_channel.at(target_list[i]).isClientInBlackList(client_fd) || \
			(!_channel.at(target_list[i]).getPasswd().empty() && _channel.at(target_list[i]).isClientInChannel(client_fd) == false) || \
			(_channel.at(target_list[i]).isInviteOnly() && _channel.at(target_list[i]).isClientInChannel(client_fd) == false))
			{
				Rep().E404(client_fd, _client[client_fd].get_nick(), target_list[i]);
				continue;
			}

			for (map<int, pair<char, vector<string> > >::const_iterator it = _channel[target_list[i]].getList().begin(); it != _channel[target_list[i]].getList().end(); it++)
			{
				if (it->first != client_fd)
				{
					string ret = ":" + _client[client_fd].get_nick() + "!" + _client[client_fd].get_username() + "@" + string(SERVER_NAME) + " PRIVMSG " + target_list[i] + " " + res[2] + "\r\n";
					if (send_to_user(ret, it->first) == -1)
						send_error((*it).first);
				}
			}
		}
		else
		{
			int dst_fd = Client().find_user_by_nick(target_list[i], _client);
			if (dst_fd == -1)
			{
				Rep().E401(client_fd, _client[client_fd].get_nick(), target_list[i]);
				continue;
			}
			string ret = ":" + _client[client_fd].get_nick() + "!" + _client[client_fd].get_username() + "@" + string(SERVER_NAME) + " PRIVMSG " + target_list[i] + " " + res[2] + "\r\n";
			if (send_to_user(ret, dst_fd) == -1)
				send_error(dst_fd);
			if (_client[dst_fd].get_mode_a())
				Rep().R301(client_fd, _client[client_fd].get_nick(), target_list[i], _client[dst_fd].get_away_message());
			//cout << ANSI::gray << "{send} => " << ANSI::purple << ret << endl;
		}
	}
}
