/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   topic.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amiguez <amiguez@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/13 15:04:18 by bducrocq          #+#    #+#             */
/*   Updated: 2023/04/25 19:01:30 by amiguez          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"

void Server::topic(vector<string> args, int fd_client)
{
	if (args.size() == 3)
	{
		if (_channel.find(args[1]) == _channel.end())
			return Rep().E403(fd_client, _client.at(fd_client).get_nick(), args[1]);
		if (_channel.at(args[1]).getTopic().empty())
			Rep().R331(fd_client, _client.at(fd_client).get_nick(), args[1]);
		else
		{
			Rep().R332(fd_client, _client.at(fd_client).get_nick(),  args[1], ":" + _channel.at(args[1]).getTopic());
			Rep().R333(fd_client, _client.at(fd_client).get_nick(), _channel.at(args[1]).getName(), \
			_channel.at(args[1]).getTopicClientSetter(), time(0));
			// ":" + _client[fd].get_nick() + "!" + _client[fd].get_username() + "@" + string(SERVER_NAME)
		}
	}
	else if (args.size() >= 4)
	{
		vector<string> res = super_split(args[args.size() - 1], 1);
		if (res.size() == 1)
			res.push_back("");
		string msg;
		bool startCopy = false;
		for (string::const_iterator it = res[1].begin(); it != res[1].end(); it++)
		{
			if (startCopy)
				msg += *it;
			if (*it == ':')
				startCopy = true;
		}
		if (msg == "")
			msg = "";
		// else
		// 	msg = res[1].substr(1);

		if(_channel.at(args[1]).getClientMode(fd_client) == '@')
		{
			_channel.at(args[1]).setTopic(msg);
			confirm_to_all_channel_client(fd_client, "TOPIC " + _channel.at(args[1]).getName() + " :" + msg, *this, _channel.at(args[1]));
			_channel.at(args[1]).setTopicClientSetter(_client.at(fd_client).get_nick() + "!" + _client.at(fd_client).get_username() + "@" + string(SERVER_NAME));
		}
		else
			return Rep().E482(fd_client, _client.at(fd_client).get_nick(), args[1]);
	}
	else
		Rep().E409(fd_client, _client.at(fd_client).get_nick());
}

//TOPIC #tt :BLABLA