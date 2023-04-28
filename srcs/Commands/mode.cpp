/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mode.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bducrocq <bducrocq@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/14 02:46:29 by bducrocq          #+#    #+#             */
/*   Updated: 2023/04/28 16:58:29 by bducrocq         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"



/*********************************************************/
/*********************************************************/
/*********************************************************/
/*********************************************************/
/******************* MODE CLIENT *************************/
/*********************************************************/
/*********************************************************/
/*********************************************************/
/*********************************************************/
void Server::mode_client(vector<string> args, int fd_client)
{
	if (args[2][0] != '+' && args[2][0] != '-')
	{
		Rep().E501(fd_client, _client[fd_client].get_nick());
		return ;
	}
	bool mod = false; //determine si mode '+' ou '-'
	if(args[2][0] == '+')
		mod = true;
	set<char> addMode; // liste des modes a ajouter
	set<char> delMode; // liste des modes a supprimer
	for(string::iterator it = args[2].begin(); it != args[2].end(); it++)
	{
		switch (*it)
		{
		case '+':
			mod = true;
			addMode.insert('+');
			break;
		case '-':
			mod = false;
			delMode.insert('-');
			break;
		case 'a': 											// away
			if (mod) {
				_client[fd_client].set_mode_a();
				addMode.insert('a');
			}
			else {
				_client[fd_client].unset_mode_a();
				delMode.insert('a');
			}
			break;
		case 'i': 											// invisible
			if (mod) {
				_client[fd_client].set_mode_i();
				addMode.insert('i');
			}
			else {
				_client[fd_client].unset_mode_i();
				delMode.insert('i');
			}
			break;
		case 'o': 											// operator
			if (mod)
				; //deprecated in RFC-2812
			else {
				_client[fd_client].unset_mode_o();
				delMode.insert('o');
			}
			break;
		case 's': 											// server notices
			if (mod) {
				_client[fd_client].set_mode_s();
				addMode.insert('s');
			}
			else {
				_client[fd_client].unset_mode_s();
				delMode.insert('s');
			}
			break;
		case 'w': 											// wallops
			if (mod) {
				_client[fd_client].set_mode_w();
				addMode.insert('w');
			}
			else {
				_client[fd_client].unset_mode_w();
				delMode.insert('w');
			}
			break;
		default: 											// error
			Rep().E472(fd_client, _client[fd_client].get_nick(), *it);
			break;
		}
	}
	string appendFullCmdMod;
	if (addMode.size() > 1) // si on a des modes a ajouter
		for(set<char>::iterator it = addMode.begin(); it != addMode.end(); it++)
			appendFullCmdMod += *it;
	if (delMode.size() > 1) //	si on a des modes a supprimer
		for(set<char>::iterator it = delMode.begin(); it != delMode.end(); it++)
			appendFullCmdMod += *it;

	if (!appendFullCmdMod.empty())	// si on a des modes a ajouter ou supprimer, envoie tout les modes + et - valide au client
		Rep().R221(fd_client, _client[fd_client].get_nick(), appendFullCmdMod);
	addMode.clear();
	delMode.clear();
}



/**
 * @brief Count the number of modes in a string exclude + and -
 * 
 * @param str 
 * @return int 
 */
int countNbrMode(std::string str)
{
	int count = 0;
	for (std::string::iterator it = str.begin(); it != str.end(); ++it)
	{
		char c = *it;
		if (c != '+' && c != '-')
			count++;
	}
	return count;
}

string returnArgsAppendWithoutSeparator(string str, char separator)
{
	string ret;
	for (string::iterator it = str.begin(); it != str.end(); ++it)
	{
		char c = *it;
		if (c != separator)
			ret += c;
	}
	return ret;
}


string		Server::recupAllModChan(string channelName, bool displayKey)
{
	string chanMode;

	if (_channel.at(channelName).isInviteOnly() || _channel.at(channelName).getPasswd().empty() == false)
		chanMode += '+';
	if (_channel.at(channelName).getPasswd().empty() == false)
		chanMode += 'k';
	if (_channel.at(channelName).isInviteOnly())
		chanMode += 'i';

	if (_channel.at(channelName).getPasswd().empty() == false)
	{
		if (displayKey)
			chanMode += " :" + _channel.at(channelName).getPasswd();
		else
			chanMode += " :<key>";
	}
	if (chanMode.empty())
		chanMode = "";
	return chanMode;
}




/*********************************************************/
/*********************************************************/
/*********************************************************/
/*********************************************************/
/******************* MODE CHANNEL ************************/
/*********************************************************/
/*********************************************************/
/*********************************************************/
/*********************************************************/
void Server::mode_channel(vector<string> args, int fd_client)
{
	if (args.size() < 3) // rep 324
	{
		string mode;
		if (_channel.at(args[1]).isClientInChannel(fd_client) == true)
			mode = recupAllModChan(args[1], true);
		else
			mode = recupAllModChan(args[1], false);
		Rep().R324(fd_client, _client[fd_client].get_nick(), args[1], mode, "");
		return ;
	}
	
	if (args[2][0] != '+' && args[2][0] != '-')
	{
		Rep().E501(fd_client, _client[fd_client].get_nick());
		return ;
	}
	vector<string>	modeParams; //split les arguments des modes
	if (args.size() > 3)
		modeParams = split_sep(args[3], ',');
	for (long unsigned int i = 0; i < (args[2].size()); ++i)
		modeParams.push_back("");
	map<char, string> modeParamsMap;
	for (long unsigned int i = 0; i < args[2].size(); i++)
	{
		if (args[2][i] != '+' && args[2][i] != '-')
		{
			if (modeParams[i - 1].empty()) 
				modeParamsMap[args[2][i]] = "";
			else
				modeParamsMap[args[2][i]] = modeParams[i - 1];
		}
	}
	bool mod = false;
	if(args[2][0] == '+')
		mod = true;
	set<char> addMode;
	set<char> delMode;
	for(string::iterator it = args[2].begin(); it != args[2].end(); it++)
	{
		switch (*it)
		{
		case '+':
			mod = true;
			addMode.insert('+');
			break;
		case '-':
			mod = false;
			delMode.insert('-');
			break;
		case 'o': // operator channel
			if (mod)
			{
				if(args.size() < 4)
				{
					Rep().E461(fd_client, _client[fd_client].get_nick(), "MODE");
					continue ;
				}
				if (modeParamsMap[*it].empty())
				{
					Rep().E461(fd_client, _client[fd_client].get_nick(), "MODE");
					continue ;
				}
				if (!_client[fd_client].isOperatorInChannel(_channel[args[1]]))
				{
					Rep().E482(fd_client, _client[fd_client].get_nick(), args[1]);
					continue ;
				}
				else
				{
					_channel[args[1]].addOperator(fd_client, *this, args[3]);
					addMode.insert('o');
				}
			}
			else
			{
				if(args.size() < 4)
				{
					Rep().E461(fd_client, _client[fd_client].get_nick(), "MODE");
					continue ;
				}
				if (modeParamsMap[*it].empty())
				{
					Rep().E461(fd_client, _client[fd_client].get_nick(), "MODE");
					continue ;
				}
				if (!_client[fd_client].isOperatorInChannel(_channel[args[1]]))
				{
					Rep().E482(fd_client, _client[fd_client].get_nick(), args[1]);
					continue ;
				}
				else
				{
					_channel[args[1]].removeOperator(fd_client, *this, args[3]);
					delMode.insert('o');
				}
			}
			break;
		case 'k': // key
			if (mod)
			{
				if(args.size() < 4)
				{
					Rep().E461(fd_client, _client[fd_client].get_nick(), "MODE");
					continue ;
				}
				if (modeParamsMap[*it].empty())
				{
					Rep().E461(fd_client, _client[fd_client].get_nick(), "MODE");
					continue ;
				}
				if (!_client[fd_client].isOperatorInChannel(_channel[args[1]]))
				{
					Rep().E482(fd_client, _client[fd_client].get_nick(), args[1]);
					continue ;
				}
				else
				{
					_channel[args[1]].setPasswd(modeParamsMap[*it]);
					addMode.insert('k');
				}
			}
			else
			{
				if(args.size() < 4)
				{
					Rep().E461(fd_client, _client[fd_client].get_nick(), "MODE");
					continue ;
				}
				if (modeParamsMap[*it].empty())
				{
					Rep().E461(fd_client, _client[fd_client].get_nick(), "MODE");
					continue ;
				}
				if (!_client[fd_client].isOperatorInChannel(_channel[args[1]]))
				{
					Rep().E482(fd_client, _client[fd_client].get_nick(), args[1]);
					continue ;
				}
				else
				{
					_channel[args[1]].setPasswd("");
					delMode.insert('k');
				}
			}
			break; 
		case 'l': // limit number of users
			if (mod)
			{
				if(args.size() < 4)
				{
					Rep().E461(fd_client, _client[fd_client].get_nick(), "MODE");
					continue ;
				}
				if (modeParamsMap[*it].empty())
				{
					Rep().E461(fd_client, _client[fd_client].get_nick(), "MODE");
					continue ;
				}
				if (!_client[fd_client].isOperatorInChannel(_channel[args[1]]))
				{
					Rep().E482(fd_client, _client[fd_client].get_nick(), args[1]);
					continue ;
				}
				else
				{
					if (modeParamsMap[*it].find_first_not_of("0123456789") != string::npos)
					{
						Rep().E461(fd_client, _client[fd_client].get_nick(), "MODE");
						continue ;
					}
					_channel[args[1]].setLimit(atoi(modeParamsMap[*it].c_str()));
					delMode.insert('l');
				}
			}
			else
			{
				if(args.size() < 4)
				{
					Rep().E461(fd_client, _client[fd_client].get_nick(), "MODE");
					continue ;
				}
				if (modeParamsMap[*it].empty())
				{
					Rep().E461(fd_client, _client[fd_client].get_nick(), "MODE");
					continue ;
				}
				if (!_client[fd_client].isOperatorInChannel(_channel[args[1]]))
				{
					Rep().E482(fd_client, _client[fd_client].get_nick(), args[1]);
					continue ;
				}
				else
				{
					_channel[args[1]].setLimit(0);
					delMode.insert('l');
				}
			}
			break;
		case 'i': // invite only
			if (mod)
			{
				
				if (!_client[fd_client].isOperatorInChannel(_channel[args[1]]))
				{
					Rep().E482(fd_client, _client[fd_client].get_nick(), args[1]);
					continue ;
				}
				else
				{
					_channel[args[1]].setInviteOnly(true);
					addMode.insert('i');
				}
			}
			else
			{
				if (!_client[fd_client].isOperatorInChannel(_channel[args[1]]))
				{
					Rep().E482(fd_client, _client[fd_client].get_nick(), args[1]);
					continue ;
				}
				else
				{
					_channel[args[1]].setInviteOnly(false);
					delMode.insert('i');
				}
			}
			break;
		case 'p': // private // = for public | * for private | @ for secret
			if (mod)
			{
				if(args.size() < 4)
				{
					Rep().E461(fd_client, _client[fd_client].get_nick(), "MODE");
					continue ;
				}
				if (!_client[fd_client].isOperatorInChannel(_channel[args[1]]))
				{
					Rep().E482(fd_client, _client[fd_client].get_nick(), args[1]);
					continue ;
				}
				else
				{
					_channel[args[1]].setVisibilityMode('*');
					addMode.insert('p');
				}
			}
			else
			{
				if(args.size() < 4)
				{
					Rep().E461(fd_client, _client[fd_client].get_nick(), "MODE");
					continue ;
				}
				if (!_client[fd_client].isOperatorInChannel(_channel[args[1]]))
				{
					Rep().E482(fd_client, _client[fd_client].get_nick(), args[1]);
					continue ;
				}
				else
				{
					_channel[args[1]].setVisibilityMode('=');
					delMode.insert('p');
				}
			}
			break;
		case 's': // secret
			if (mod)
			{
				if(args.size() < 4)
				{
					Rep().E461(fd_client, _client[fd_client].get_nick(), "MODE");
					continue ;
				}
				if (!_client[fd_client].isOperatorInChannel(_channel[args[1]]))
				{
					Rep().E482(fd_client, _client[fd_client].get_nick(), args[1]);
					continue ;
				}
				else
				{
					_channel[args[1]].setVisibilityMode('@');
					addMode.insert('s');
				}
			}
			else
			{
				if(args.size() < 4)
				{
					Rep().E461(fd_client, _client[fd_client].get_nick(), "MODE");
					continue ;
				}
				if (!_client[fd_client].isOperatorInChannel(_channel[args[1]]))
				{
					Rep().E482(fd_client, _client[fd_client].get_nick(), args[1]);
					continue ;
				}
				else
				{
					_channel[args[1]].setVisibilityMode('=');
					delMode.insert('s');
				}
			}
			break;
		case 'b': //banList mask.
			if (mod)
			{
				if(args.size() < 4)
				{
					Rep().E461(fd_client, _client[fd_client].get_nick(), "MODE");
					continue ;
				}
				if (findClientFdWithNick(args[3]) == 0) //client not exist
				{
					Rep().E401(fd_client, _client[fd_client].get_nick(), args[3]);
					continue ;
				}
				if (!_client[fd_client].isOperatorInChannel(_channel[args[1]]))
				{
					Rep().E482(fd_client, _client[fd_client].get_nick(), args[1]);
					continue ;
				}
				else
				{
					_channel.at(args[1]).addBlackList(_client[findClientFdWithNick(args[3])].get_id(), fd_client, *this);
					addMode.insert('b');
				}
				
			}
			else
			{
				if(args.size() < 4)
				{
					Rep().E461(fd_client, _client[fd_client].get_nick(), "MODE");
					continue ;
				}
				if (findClientFdWithNick(args[3]) == 0) //client not exist
				{
					Rep().E401(fd_client, _client[fd_client].get_nick(), args[3]);
					continue ;
				}
				if (!_client[fd_client].isOperatorInChannel(_channel[args[1]]))
				{
					Rep().E482(fd_client, _client[fd_client].get_nick(), args[1]);
					continue ;
				}
				else
				{
					_channel.at(args[1]).removeBlackList(findClientFdWithNick(args[3]));
					delMode.insert('b');
				}
			}
			break;
		default: // error
			Rep().E472(fd_client, _client[fd_client].get_nick(), *it);
			break;
		}
	}
		string appendFullCmdMod;//
	if (addMode.size() > 1) // si on a des modes a ajouter
		for(set<char>::iterator it = addMode.begin(); it != addMode.end(); it++)
			appendFullCmdMod += *it;
	if (delMode.size() > 1) //	si on a des modes a supprimer
		for(set<char>::iterator it = delMode.begin(); it != delMode.end(); it++)
			appendFullCmdMod += *it;
	string appendMode;
	for(map<char, string>::iterator it = modeParamsMap.begin(); it != modeParamsMap.end(); it++)
	{
		if (it->second.size() > 0)
			appendMode += it->first;
	}

	string appendArgs;
	for(map<char, string>::iterator it = modeParamsMap.begin(); it != modeParamsMap.end(); it++)
	{
		if (it->second.size() > 0)
			appendArgs += it->second;
	}


	if (!appendFullCmdMod.empty())	// si on a des modes a ajouter ou supprimer, envoi tout les modes + et - valide au client
	{
		if (!appendArgs.empty())
			confirm_to_client(fd_client, "MODE " + args[1] + " " + appendFullCmdMod + " :" + appendArgs, *this);
		else
			confirm_to_client(fd_client, "MODE " + args[1] + " :" + appendFullCmdMod, *this);
	}
	addMode.clear();
	delMode.clear();
}

void Server::mode(vector<string> args, int fd_client) 
{
	if (args.size() == 2)
	{
		if (args[1][0] == '#')
		{
			if (!isExistChannelName(args[1]))
				return Rep().E403(fd_client, _client[fd_client].get_nick(), args[1]);
		}
	}

	if (args.size() < 2)
		return Rep().E461(fd_client, _client[fd_client].get_nick(), args[0]);
	if (args[1][0] == '#')
	{
		if (!isExistChannelName(args[1]))
			return Rep().E403(fd_client, _client[fd_client].get_nick(), args[1]);
		mode_channel(args, fd_client);
	}
	else
	{
		if(_client[fd_client].get_nick().compare(args[1]) != 0)
			return Rep().E502(fd_client, _client[fd_client].get_nick());
		mode_client(args, fd_client);
	}
}
