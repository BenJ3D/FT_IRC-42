/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   commands.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bducrocq <bducrocq@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/07 04:46:30 by abucia            #+#    #+#             */
/*   Updated: 2023/04/17 01:17:25 by bducrocq         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"


/*
:bducrocq!bducrocq@host JOIN #test
:bducrocq!bducrocq@host MODE #test +o bducrocq
:irc.server.com 353 bducrocq = #test :nickname1 nickname2 nickname3
:irc.server.com 366 bducrocq  #test :End of /NAMES list.
*/
vector<string> Server::super_split(string cmd, int nb_arg)
{
	vector<string> res;
	string tmp;
	unsigned long int i = 0;
	if (cmd[0] == '\n')
		cmd = cmd.substr(1);
	while (i < cmd.length() && nb_arg != 0)
	{
		if (cmd[i] == ' ' && nb_arg > 0)
		{
			res.push_back(tmp);
			tmp = "";
			nb_arg--;
			while (cmd[i] == ' ')
				i++;
		}
		else
			tmp += cmd[i++];
	}
	if (i == cmd.length())
	{
		if (res.size() == 0)
			res.push_back(tmp);
		return res;
	}
	res.push_back(cmd.substr(i));
	return res;
}

void Server::cmd_notice(vector<string> args, int client_fd) {
	if (args[args.size() - 1][0] == '\n')
		args[args.size() - 1] = args[args.size() - 1].substr(1);

	string msg = " ";
	vector<string> res = super_split(args[args.size() - 1], 2);
	if (res.size() < 3)
		return;

	vector<string> target_list = split_cmd(res[1], ',');
	if (target_list.size() == 0)
		target_list.push_back(res[1]);

	for (unsigned long int i = 0; i < target_list.size(); i++)
	{
		target_list[i] = trim(target_list[i]);
		if (target_list[i][0] == '#')
		{
			if (_channel.find(target_list[i]) == _channel.end())
				continue;
			for (map<int, pair<char, vector<string> > >::const_iterator it = _channel[target_list[i]].getList().begin(); it != _channel[target_list[i]].getList().end(); it++)
			{
				if (it->first != client_fd && !_client[it->first].get_mode_s())
				{
					string ret = ":" + _client[client_fd].get_nick() + "!" + _client[client_fd].get_username() + "@" + string(SERVER_NAME) + " NOTICE " + target_list[i] + " " + res[2] + "\r\n";
					if (send_to_user(ret, it->first) == -1)
						send_error((*it).first);
				}
			}
		}
		else
		{
			int dst_fd = Client().find_user_by_nick(target_list[i], _client);
			if (dst_fd == -1 || !_client[dst_fd].get_mode_s())
				continue;
			string ret = ":" + _client[client_fd].get_nick() + "!" + _client[client_fd].get_username() + "@" + string(SERVER_NAME) + " NOTICE " + target_list[i] + " " + res[2] + "\r\n";
			if (send_to_user(ret, dst_fd) == -1)
				send_error(dst_fd);
		}
	}
}
