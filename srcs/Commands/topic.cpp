/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   topic.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bducrocq <bducrocq@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/13 15:04:18 by bducrocq          #+#    #+#             */
/*   Updated: 2023/04/26 23:47:40 by bducrocq         ###   ########.fr       */
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

		if(_client[fd_client].isOperatorInChannel(_channel[args[1]]))
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
