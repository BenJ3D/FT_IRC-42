/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   list.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bducrocq <bducrocq@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/13 15:12:15 by bducrocq          #+#    #+#             */
/*   Updated: 2023/04/13 15:13:02 by bducrocq         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"

void	Server::list(vector<string> args, int fd_client)
{
	if (args.size() != 1)
		return Rep().E409(fd_client, _client.at(fd_client).get_nick());
	Rep().R321(fd_client, _client.at(fd_client).get_nick());
	map<string,Channel>::iterator	it = _channel.begin();

	for(; it != _channel.end(); ++it)
	{
		Rep().R322(fd_client, _client.at(fd_client).get_nick(), it->second.getNbClient(), it->second.getTopic(), it->second.getName());
	}
	Rep().R323(fd_client, _client.at(fd_client).get_nick());
}
