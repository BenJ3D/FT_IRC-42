/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   join.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bducrocq <bducrocq@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/13 15:02:34 by bducrocq          #+#    #+#             */
/*   Updated: 2023/04/22 02:24:06 by bducrocq         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "./Server.hpp"

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
	if (name.size() >= 1 && name[0] == '#')
	{
		cerr << ANSI::blue << "checkNameChannelIsGood: true" << ANSI::reset << endl;
		return true;
	}
	cerr << ANSI::blue << "checkNameChannelIsGood: false" << ANSI::reset << endl;
	return false;
}

void Server::join(vector<string> args, int fd_client) // TODO: check le premier char # &
{
	string clientNick = _client[fd_client].get_nick();
	if (args.size() < 2)
		return Rep().E461(fd_client, clientNick, args[0]); // Pas assez de parametres

	if (args[1].empty())
		return;
	vector<string> chan;
	vector<string> pass;
	chan = split_sep(args[1], ',');
	if (args.size() < 3)
		args.push_back("");
	pass = split_sep(args[2], ',');

	cerr << ANSI::red << "pass.size = " << pass.size() << endl << ANSI::reset;

	for (size_t i = pass.size(); i < chan.size(); ++i) // remplir de pass vide pour simplifier la suite :}
		pass.push_back("");
	vector<string>::const_iterator it_chan = chan.begin();
	vector<string>::const_iterator it_passwd = pass.begin();
	for (; it_chan != chan.end(); ++it_chan, ++it_passwd)
	{
		if (checkNameChannelIsGood(*it_chan))
			if (_channel.find(*it_chan) == _channel.end())
			{
				_channel[*it_chan] = Channel(fd_client, *it_chan);
				//_channel[*it_chan].addClient(fd_client, '@');
				confirm_to_client(fd_client, "JOIN " + *it_chan, _client);
				confirm_to_client(fd_client, "MODE " + *it_chan + " +o " + clientNick, _client);

				Rep().R353(fd_client, clientNick, *it_chan, clientNick, _channel[*it_chan].getVisibilityMode(), _channel[*it_chan].getList().at(fd_client).first); //FIXME: FAUX
				Rep().R366(fd_client, clientNick, *it_chan);
			}
			else
			{
				for (vector<int>::iterator it = _channel[*it_chan].getBlackList().begin(); it != _channel[*it_chan].getBlackList().end(); it++)
					if ((*it) == fd_client)
						return Rep().E474(fd_client, clientNick, *it_chan);
				//TODO: verif si le channel est en mode invite only et si le client est dans _inviteList

				if (_channel.at(*it_chan).isClientInInviteList(fd_client) == false && _channel.at(*it_chan).isInviteOnly() == true)
				{
					cerr << ANSI::red << "DEBUG TEST INVITE ONLY" << ANSI::reset << endl;
					return Rep().E473(fd_client, clientNick, *it_chan);
				}

				if (!_channel.at(*it_chan).getPasswd().empty())					//y a til un passwd de set
				{
					if (_channel.at(*it_chan).getPasswd().compare(*it_passwd) != 0) // si oui, est ce le bon passwd en param (si pas de pass channel et un pass en param, ce dernier est ignorer)
					{
						Rep().E475(fd_client, clientNick, *it_chan);
						continue;
					}
				}
				_channel[*it_chan].addClient(fd_client, ' ');
				confirm_to_client(fd_client, "JOIN " + *it_chan, _client);
				string user_list = _channel[*it_chan].ListNick(_client, fd_client);
				Rep().R353(fd_client, clientNick, *it_chan, user_list, _channel[*it_chan].getVisibilityMode(), _channel[*it_chan].getList().at(fd_client).first); //FIXME: FAUX
				Rep().R366(fd_client, clientNick, *it_chan);
				cerr << ANSI::red << "DEBUG TEST USER LIST = " << user_list << ANSI::reset << endl;
					
				// }
			}
		else
			Rep().E476(fd_client, _client.at(fd_client).get_nick());
		// if (it_passwd != pass.end())
		// 	++it_passwd;
	}
}