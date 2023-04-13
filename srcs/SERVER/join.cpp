/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   join.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bducrocq <bducrocq@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/13 15:02:34 by bducrocq          #+#    #+#             */
/*   Updated: 2023/04/13 15:47:04 by bducrocq         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "./Server.hpp"

void	Server::join(vector<string> args, int fd_client) //TODO: gerer le cas de multi canaux (ex: JOIN #test,#test2,#test3 passwd)
{
	if (args.size() < 2)
		return Rep().E461(fd_client, _client[fd_client].get_nick(), args[0]);

	if (_channel.find(args[1]) == _channel.end())
	{
		_channel[args[1]] = Channel(fd_client, args[1]);
		//_channel[args[1]].addClient(fd_client, '@');
		confirm_to_client(fd_client, "JOIN " + args[1], _client);
		confirm_to_client(fd_client, "MODE " + args[1] + " +o " + _client[fd_client].get_nick(), _client);

		Rep().R353(fd_client, _client[fd_client].get_nick(), args[1], _client[fd_client].get_nick(), _channel[args[1]].getMode(), _channel[args[1]].getList().at(fd_client).first);
		Rep().R366(fd_client, _client[fd_client].get_nick(), args[1]);
	}
	else
	{
		for (vector<int>::iterator it = _channel[args[1]].getBlackList().begin(); it != _channel[args[1]].getBlackList().end(); it++)
			if ((*it) == fd_client)
				return Rep().E474(fd_client, _client[fd_client].get_nick(), args[1]);

		_channel[args[1]].addClient(fd_client, ' ');
		confirm_to_client(fd_client, "JOIN " + args[1], _client);
		string user_list = _channel[args[1]].ListNick(_client, fd_client);
		Rep().R353(fd_client, _client[fd_client].get_nick(), args[1], user_list, _channel[args[1]].getMode(), _channel[args[1]].getList().at(fd_client).first);
		Rep().R366(fd_client, _client[fd_client].get_nick(), args[1]);
		cerr << ANSI::red << "DEBUG TEST USER LIST = "  << user_list << ANSI::reset << endl;
	}
}