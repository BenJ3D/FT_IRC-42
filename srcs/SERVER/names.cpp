/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   names.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bducrocq <bducrocq@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/17 02:28:16 by abucia            #+#    #+#             */
/*   Updated: 2023/04/22 22:19:18 by bducrocq         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./Server.hpp"

void	Server::names(vector<string> args, int client_fd)
{
	if (args.size() == 3 && args[2] != SERVER_NAME)
		return Rep().E402(client_fd, _client[client_fd].get_nick(), args[2]);
	if (args.size() == 1)
	{
		if (_channel.size() == 0)
			return Rep().R366(client_fd, _client[client_fd].get_nick(), "*");
		else if (_channel.size() > 10)
			return Rep().E416(client_fd, _client[client_fd].get_nick(), args[1]);
		for (map<string, Channel>::iterator it = _channel.begin(); it != _channel.end(); it++)
		{
			Rep().R353(\
				client_fd, \
				_client[client_fd].get_nick(),it->second.getName(), \
				it->second.list_all_nick(_client), \
				'=', /*FIXME:*/ \
				it->second.getClientMode(client_fd));
			Rep().R366(client_fd, _client[client_fd].get_nick(),it->second.getName());
		}
		return ;
	}
	vector<string> listChan = split_cmd(args[1], ',');
	for (size_t i = 0; i < listChan.size(); i++)
	{
		if (listChan[i][0] != '#' || _channel.find(listChan[i]) == _channel.end())
			Rep().R366(client_fd, _client[client_fd].get_nick(),listChan[i]);
		else
		{
			Rep().R353(\
				client_fd, \
				_client[client_fd].get_nick(),_channel[listChan[i]].getName(), \
				_channel[listChan[i]].list_all_nick(_client), \
				'=', /*FIXME:*/ \
				_channel[listChan[i]].getClientMode(client_fd));
			Rep().R366(client_fd, _client[client_fd].get_nick(),listChan[i]);
		}
	}
}