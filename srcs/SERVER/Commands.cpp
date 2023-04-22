/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Commands.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amiguez <amiguez@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/07 04:46:30 by abucia            #+#    #+#             */
/*   Updated: 2023/04/17 00:48:26 by amiguez          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"

void Server::nick(vector<string> args, int client_fd) {
	cout << ANSI::cyan << client_fd << " --> " << args[0] << endl;

	if (args.size() < 2)
		return Rep().E431(client_fd, _client[client_fd].get_nick());
	string check("[]\\`_^{|}$abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ");
	string new_nick = args[1];
	if (check.find(new_nick[0]) == string::npos || new_nick.length() > 9)
		return Rep().E432(client_fd, _client[client_fd].get_nick(), new_nick);
	check += "0123456789-";
	for (size_t i = 1; i < new_nick.length(); i++)
		if (check.find(new_nick[i]) == string::npos)
			return Rep().E432(client_fd, _client[client_fd].get_nick(), new_nick);
	for (map<int, Client>::iterator it = _client.begin(); it != _client.end(); it++) {
		if ((*it).first == client_fd)
			continue;
		cout << _client.size() << " " << (*it).second.get_nick() << endl;
		if ((*it).second.get_nick() == new_nick)
			return Rep().E433(client_fd, _client[client_fd].get_nick(), new_nick);
	}
	if (!_client[client_fd].get_is_auth() && _client[client_fd].get_username() != "")
	{
		_client[client_fd].now_auth();
		_client[client_fd].set_nick(new_nick);
		cout << ANSI::red << "DEBUG TEST" << ANSI::reset << endl;
		Rep().R001(client_fd, new_nick);
		Rep().R002(client_fd, new_nick, string(SERVER_NAME), string(SERVER_VERSION));
		return Rep().R003(client_fd, new_nick ,string(SERVER_DATE));
	}
	string confirm_msg = "NICK " + new_nick;
	confirm_to_client(client_fd, confirm_msg, *this);
	_client[client_fd].set_nick(new_nick);
}

void Server::user(vector<string> args, int cl) {
	cout << ANSI::cyan << cl << " --> " << args[0] << endl;

	if (args.size() < 5)
		return Rep().E461(cl, _client[cl].get_nick(), args[0]);
	if (_client[cl].get_username() != "")
		return Rep().E462(cl, _client[cl].get_nick());
	string check("[]\\`_^{|}$abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789-");
	for (size_t i = 1; i < args[1].length(); i++)
		if (check.find(args[1][i]) == string::npos)
			return Rep().E468(cl, _client[cl].get_nick());

	string realname = "";
	string username = args[1];

	for (size_t i = 3; i < args.size(); i++)
		realname += args[i] + " ";
	if (args[1].size() > 9 || _client[cl].get_username() == "") {
		if (args[1].size() > 9)
			username = args[1].substr(0, 9);
		username = "~" + username;
		notice(cl, "*** Could not find your ident, using " + username + " instead.");
	}
	if (!_client[cl].get_is_auth() && _client[cl].get_nick() != "*")
	{
		_client[cl].now_auth();
		this->_client[cl].set_username(username);
		this->_client[cl].set_realname(realname);
		Rep().R001(cl, _client[cl].get_nick());
		Rep().R002(cl, _client[cl].get_nick(), string(SERVER_NAME), string(SERVER_VERSION));
		Rep().R003(cl, _client[cl].get_nick(), string(SERVER_DATE));
		return motd_auth(cl);
	}
	this->_client[cl].set_username(username);
	this->_client[cl].set_realname(realname);
}

/*
:bducrocq!bducrocq@host JOIN #test
:bducrocq!bducrocq@host MODE #test +o bducrocq
:irc.server.com 353 bducrocq = #test :nickname1 nickname2 nickname3
:irc.server.com 366 bducrocq  #test :End of /NAMES list.
*/

vector<string> Server::super_split(string cmd, int nb_arg)
{
	vector<string> res;
	string tmp;
	unsigned long int i = 0;
	if (cmd[0] == '\n')
		cmd = cmd.substr(1);
	while (i < cmd.length() && nb_arg != 0)
	{
		if (cmd[i] == ' ' && nb_arg > 0)
		{
			res.push_back(tmp);
			tmp = "";
			nb_arg--;
			while (cmd[i] == ' ')
				i++;
		}
		else
			tmp += cmd[i++];
	}
	if (i == cmd.length())
	{
		if (res.size() == 0)
			res.push_back(tmp);
		return res;
	}
	res.push_back(cmd.substr(i));
	return res;
}

void Server::cmd_notice(vector<string> args, int client_fd) {
	if (args[args.size() - 1][0] == '\n')
		args[args.size() - 1] = args[args.size() - 1].substr(1);
	cout << ANSI::cyan << client_fd << " --> " << args[0] << endl;
	for (size_t i = 0; i < args.size(); i++)
		cerr << ANSI::red << "ARGS[" << i << "] = " << args[i] << ANSI::reset << endl;

	string msg = " ";
	vector<string> res = super_split(args[args.size() - 1], 2); // <CMD> <TARGET> :<MSG>
	cerr << res.size() << endl;
	if (res.size() < 3)
		return;

	vector<string> target_list = split_cmd(res[1], ',');
	if (target_list.size() == 0)
		target_list.push_back(res[1]);

	for (unsigned long int i = 0; i < target_list.size(); i++)
	{
		target_list[i] = trim(target_list[i]);
		if (target_list[i][0] == '#')
		{
			if (_channel.find(target_list[i]) == _channel.end())
				continue;
			for (map<int, pair<char, vector<string> > >::const_iterator it = _channel[target_list[i]].getList().begin(); it != _channel[target_list[i]].getList().end(); it++)
			{
				cout << ANSI::red << "DEBUG TEST PRIVMSG = " << target_list[i] << ANSI::reset << endl;
				if (it->first != client_fd && !_client[it->first].get_mode_s())
				{
					string ret = ":" + _client[client_fd].get_nick() + "!" + _client[client_fd].get_username() + "@" + string(SERVER_NAME) + " NOTICE " + target_list[i] + " " + res[2] + "\r\n";
					if (send((*it).first, ret.c_str(), ret.length(), 0) == -1)
						cerr << ANSI::red << "Erreur lors de l'envoi des données au client " << it->first << endl;
					cout << ANSI::gray << "{send} => " << ANSI::purple << ret << endl;
				}
			}
		}
		else
		{
			int dst_fd = Client().find_user_by_nick(target_list[i], _client);
			if (dst_fd == -1 || !_client[dst_fd].get_mode_s())
				continue;
			string ret = ":" + _client[client_fd].get_nick() + "!" + _client[client_fd].get_username() + "@" + string(SERVER_NAME) + " NOTICE " + target_list[i] + " " + res[2] + "\r\n";
			if (send(dst_fd, ret.c_str(), ret.length(), 0) == -1)
				cerr << ANSI::red << "Erreur lors de l'envoi des données au client " << dst_fd << endl;
			cout << ANSI::gray << "{send} => " << ANSI::purple << ret << endl;
		}
	}
}

void Server::privmsg(vector<string> args, int client_fd) {
	if (args[args.size() - 1][0] == '\n')
		args[args.size() - 1] = args[args.size() - 1].substr(1);
	cout << ANSI::cyan << client_fd << " --> " << args[0] << endl;
	for (size_t i = 0; i < args.size(); i++)
		cerr << ANSI::red << "ARGS[" << i << "] = " << args[i] << ANSI::reset << endl;

	string msg = " ";
	vector<string> res = super_split(args[args.size() - 1], 2); // <CMD> <TARGET> :<MSG>
	cerr << res.size() << "AAAAAAAAAAAAAAAAAAA" << endl;
	if (res.size() < 3)
	{
		if (res.size() == 2)
			return Rep().E412(client_fd, _client[client_fd].get_nick());
		else
			return Rep().E411(client_fd, _client[client_fd].get_nick(), args[0]);
	}
	vector<string> target_list = split_cmd(res[1], ',');
	cout << "debug" << endl;
	if (target_list.size() == 0)
		target_list.push_back(res[1]);

	for (unsigned long int i = 0; i < target_list.size(); i++)
	{
		target_list[i] = trim(target_list[i]);
		if (target_list[i][0] == '#')
		{
			if (_channel.find(target_list[i]) == _channel.end())
			{
				Rep().E403(client_fd, _client[client_fd].get_nick(), target_list[i]);
				continue;
			}
			for (map<int, pair<char, vector<string> > >::const_iterator it = _channel[target_list[i]].getList().begin(); it != _channel[target_list[i]].getList().end(); it++)
			{
				cout << ANSI::red << "DEBUG TEST PRIVMSG = " << target_list[i] << ANSI::reset << endl;
				if (it->first != client_fd)
				{
					if (_client[it->first].get_mode_a())
						Rep().R301(client_fd, _client[client_fd].get_nick(), target_list[i], _client[it->first].get_away_message());
					string ret = ":" + _client[client_fd].get_nick() + "!" + _client[client_fd].get_username() + "@" + string(SERVER_NAME) + " PRIVMSG " + target_list[i] + " " + res[2] + "\r\n";
					if (send((*it).first, ret.c_str(), ret.length(), 0) == -1)
						cerr << ANSI::red << "Erreur lors de l'envoi des données au client " << it->first << endl;
					cout << ANSI::gray << "{send} => " << ANSI::purple << ret << endl;
				}
			}
		}
		else
		{
			int dst_fd = Client().find_user_by_nick(target_list[i], _client);
			if (dst_fd == -1)
			{
				Rep().E401(client_fd, _client[client_fd].get_nick(), target_list[i]);
				continue;
			}
			string ret = ":" + _client[client_fd].get_nick() + "!" + _client[client_fd].get_username() + "@" + string(SERVER_NAME) + " PRIVMSG " + target_list[i] + " " + res[2] + "\r\n";
			if (send(dst_fd, ret.c_str(), ret.length(), 0) == -1)
				cerr << ANSI::red << "Erreur lors de l'envoi des données au client " << dst_fd << endl;
			if (_client[dst_fd].get_mode_a())
				Rep().R301(client_fd, _client[client_fd].get_nick(), target_list[i], _client[dst_fd].get_away_message());
			cout << ANSI::gray << "{send} => " << ANSI::purple << ret << endl;
		}
	}
}

// send to everyone 
//:blue!~blue@freenode-2on.i6k.qof1pp.IP KICK #4422 target :msg
/*
	check channel mask ['&' or '#']
	check if channel existe
	check if user in channel
	check if op
	check if target on channel
*/
void Server::kick(vector<string> args, int cl){
	if (args.size() < 4)
		return Rep().E461(cl, _client[cl].get_nick(), args[0]);

	vector<string> chan = split_cmd(args[1], ',');
	vector<string> temp_usr = split_cmd(args[2], ',');
	vector<int> usrs_fd;
	for(size_t i = 0; i < temp_usr.size(); i++)
		usrs_fd.push_back(Client().find_user_by_nick(temp_usr[i], _client));

	for (size_t i = 0; i < chan.size(); i++){
		if (chan[i][0] != '&' && chan[i][0] != '#'){
			Rep().E476(cl, chan[i]);
			continue;
		} if (_channel.find(chan[i]) == _channel.end()){
			Rep().E403(cl, _client[cl].get_nick(), chan[i]);
			continue;
		}

		Channel& chan_temp = _channel[chan[i]];
		map< int, pair<char, vector<string> > > cl_in_chan = chan_temp.getList();
		if (cl_in_chan.find(cl) == cl_in_chan.end()){
			Rep().E442(cl, _client[cl].get_nick(), chan[i]);
			continue;
		}

		if (!_client[cl].isOperatorInChannel(_channel[chan[i]])){
			Rep().E482(cl, _client[cl].get_nick(), chan[i]);
			continue;
		}

		for(size_t j = 0; j < usrs_fd.size(); j++){
			map<int, pair<char, vector<string> > >::iterator target = cl_in_chan.find(usrs_fd[j]);
			if (target == cl_in_chan.end()){
				Rep().E441(cl, _client[cl].get_nick(), chan[i], temp_usr[j]);
				continue;
			}
			string ret = ":" + _client[cl].get_nick() + "!" + _client[cl].get_username() + "@" + string(SERVER_NAME) + " KICK " + chan[i] + " " + _client[(*target).first].get_nick();
			if (args.size() > 4){
				vector<string> msg = split_to_point(args.back());
				if (!msg.empty())
					ret += " :" + msg.back();
			}
			ret += "\r\n";
			cout << ANSI::gray << "{send} =>" << ANSI::cmd << ret << ANSI::r << endl; 
			for (map<int, pair<char, vector<string> > >::iterator it = cl_in_chan.begin(); it != cl_in_chan.end(); it++){
				if (send((*it).first, ret.c_str(), ret.length(), 0) == -1)
					cerr << ANSI::red << "Erreur lors de l'envoi des données au client " << it->first << endl;

			}
			chan_temp.removeClient((*target).first);
		}
	}
}