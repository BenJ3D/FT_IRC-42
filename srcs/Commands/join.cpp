/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   join.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bducrocq <bducrocq@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/13 15:02:34 by bducrocq          #+#    #+#             */
/*   Updated: 2023/04/27 00:53:28 by bducrocq         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"

/**
 * @brief verifie si le nom de channel commence par # 
 * et si il n'y a pas d'espaseen premier char apres #
 * 
 * @param name nom du channel
 * @return true 
 * @return false 
 */
bool	checkNameChannelIsGood( string name )
{
	if (name.size() >= 1 && name.size() < 50 && name[0] == '#')
		return true;
	return false;
}

void Server::join(vector<string> args, int fd_client)
{
	string clientNick = _client[fd_client].get_nick();
	if (args.size() < 2)
		return Rep().E461(fd_client, clientNick, args[0]); // Pas assez de parametres

	if (args[1].empty())
		return;
	for(size_t i = 0; i < args[1].size(); i++)
		args[1][i] = tolower(args[1][i]);

	vector<string> chan;
	vector<string> pass;
	chan = split_sep(args[1], ',');
	if (args.size() < 3)
		args.push_back("");
	pass = split_sep(args[2], ',');

	for (size_t i = pass.size(); i < chan.size(); ++i) // remplir de pass vide pour simplifier la suite
		pass.push_back("");
	vector<string>::const_iterator it_chan = chan.begin();
	vector<string>::const_iterator it_passwd = pass.begin();
	for (; it_chan != chan.end(); ++it_chan, ++it_passwd)
	{
		if (checkNameChannelIsGood(*it_chan))
		{
			if (_channel.find(*it_chan) == _channel.end())
			{
				cerr << ANSI::yellow << "this =" << this << endl;
				_channel[*it_chan] = Channel(fd_client, *it_chan);
				confirm_to_client(fd_client, "JOIN " + *it_chan, *this);
				confirm_to_client(fd_client, "MODE " + *it_chan + " +o " + clientNick, *this);

				Rep().R353(fd_client, clientNick, *it_chan, clientNick, _channel[*it_chan].getVisibilityMode(), _channel[*it_chan].getList().at(fd_client).first);
				Rep().R366(fd_client, clientNick, *it_chan);
			}
			else
			{
				if (_channel.at(*it_chan).isClientInChannel(fd_client) == true)
					continue ;
				bool tmp = false;
				for (size_t j = 0; j < _channel[*it_chan].getBlackList().size(); j++){
					if ( _channel[*it_chan].getBlackList()[j] == fd_client){
						Rep().E474(fd_client, clientNick, *it_chan);
						tmp = true;
						break;
					}
				}
				if (tmp)
					continue;
				if (!_channel.at(*it_chan).getPasswd().empty())	// y a til un passwd de set
				{
					if (_channel.at(*it_chan).getPasswd().compare(*it_passwd) != 0) // si oui, est ce le bon passwd en param
					{
						if (_channel.at(*it_chan).isClientInInviteList(fd_client) == false)
						{
							Rep().E475(fd_client, clientNick, *it_chan);
							continue;
						}
						if (_channel.at(*it_chan).isInviteOnly() == false)
						{
							_channel.at(*it_chan).removeInviteList(fd_client);
						}
					}
				}
				
				if (_channel.at(*it_chan).isInviteOnly() == true )
				{
					if (_channel.at(*it_chan).isClientInInviteList(fd_client) == false)
						return Rep().E473(fd_client, clientNick, *it_chan);
					else 
						_channel.at(*it_chan).removeInviteList(fd_client);
				}
				
				if (_channel.at(*it_chan).isClientInBlackList(fd_client) == true)
					return Rep().E474(fd_client, clientNick, *it_chan);

				_channel[*it_chan].addClient(fd_client, ' ');
				string user_list = _channel[*it_chan].ListNick(*this, fd_client);
				Rep().R353(fd_client, clientNick, *it_chan, user_list, _channel[*it_chan].getVisibilityMode(), _channel[*it_chan].getList().at(fd_client).first); // FIXME: FAUX
				Rep().R366(fd_client, clientNick, *it_chan);
			}
		}
		else
			Rep().E476(fd_client, _client.at(fd_client).get_nick());
	}
}