/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   topic.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bducrocq <bducrocq@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/13 15:04:18 by bducrocq          #+#    #+#             */
/*   Updated: 2023/04/13 15:05:45 by bducrocq         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "./Server.hpp"

void Server::topic(vector<string> args, int fd_client)
{
	if (args.size() == 2)
	{
		if (_channel.at(args[1]).getTopic().empty())
			Rep().R331(fd_client, _client.at(fd_client).get_nick(), args[1]);
		else
		{
			Rep().R332(fd_client, _client.at(fd_client).get_nick(),  args[1], ":" + _channel.at(args[1]).getTopic());
			Rep().R333(fd_client, _client.at(fd_client).get_nick(), _channel.at(args[1]).getName(), \
			_client.at(fd_client).get_nick() + "!" + _client.at(fd_client).get_username() + "@" + string(SERVER_NAME), time(0));
			// ":" + _client[fd].get_nick() + "!" + _client[fd].get_username() + "@" + string(SERVER_NAME)
		}
	}
	else if (args.size() == 3)
	{
		_channel.at(args[1]).setTopic(args[2].substr(1));
		confirm_to_client(fd_client, "TOPIC " + _channel.at(args[1]).getName() + " :" + args[2].substr(1), *this);
		//TODO:: faire un ft confirm to all client in channel
		confirm_to_all_channel_client(fd_client, "TOPIC " + _channel.at(args[1]).getName() + " :" + args[2].substr(1), *this, _channel.at(args[1]));
	}
	else
		Rep().E409(fd_client, _client.at(fd_client).get_nick());
}

//TOPIC #tt :BLABLA
