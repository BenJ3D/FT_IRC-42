/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   list.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bducrocq <bducrocq@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/13 15:12:15 by bducrocq          #+#    #+#             */
/*   Updated: 2023/04/22 22:23:25 by bducrocq         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"

void	Server::list(vector<string> args, int fd_client)
{
	vector<string>target;
	if (args.size() != 1)
		target = split_cmd(args[1], ',');
	// Rep().R321(fd_client, _client.at(fd_client).get_nick()); // deprecated
	
	map<string,Channel>::iterator	it = _channel.begin();
	if (target.empty())
		for(; it != _channel.end(); ++it)
			Rep().R322(fd_client, _client.at(fd_client).get_nick(), it->second.getNumberClientInChannel(), it->second.getTopic(), it->second.getName());
	else{
		for(; it != _channel.end(); ++it)
			for(size_t i = 0; i < target.size(); i++)
				if ((*it).second.getName().find(target[i]) != string::npos){
					Rep().R322(fd_client, _client.at(fd_client).get_nick(), it->second.getNumberClientInChannel(), it->second.getTopic(), it->second.getName());
					break;
				}
	}
	Rep().R323(fd_client, _client.at(fd_client).get_nick());
}
