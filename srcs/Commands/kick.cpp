/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   kick.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amiguez <amiguez@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/06 20:39:56 by abucia            #+#    #+#             */
/*   Updated: 2023/04/25 19:01:20 by amiguez          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"

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
					send_error((*it).first);
			}
			chan_temp.removeClient((*target).first);
		}
		if (_channel[chan[i]].getList().empty()){
			cout << ANSI::blue << "chan erase" << endl;
			_channel.erase(chan[i]);
		}
	}
}