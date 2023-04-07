/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Parser.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abucia <abucia@student.42lyon.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/06 20:39:56 by abucia            #+#    #+#             */
/*   Updated: 2023/04/07 03:23:45 by abucia           ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "./Server.hpp"

vector<string>	split_cmd(const string command, char separator) {
	stringstream stream(command);
	vector<string> args;
	string buffer;
	while (getline(stream, buffer, separator))
	{
		if (separator == '\n')
			buffer.substr(0, strlen(buffer.c_str()) - 2);
		args.push_back(buffer);
		
		cout << ANSI::purple << "ADD ARG : " << ANSI::red << buffer << endl; // DEBUG
	}
	return args;
}

void	Server::init_parsing_map()
{
	this->commands["NICK"] = make_pair(2, &Server::nick);
	// this->commands["JOIN"] = make_pair(2, &nick);
	// this->commands["PRIVMSG"] = make_pair(3, &nick);
}

void	Server::parser(string command, int client_fd) {
	vector<string> cmds = split_cmd(command, '\n');
	if (cmds.size() == 0)
		return;
	for (vector<string>::iterator it = cmds.begin(); it != cmds.end(); it++)
	{
		vector<string> args = split_cmd(*it, ' ');
		if (commands.find(args[0]) != commands.end())
		{
			int expected_args = commands[args[0]].first;
			if (args.size() < expected_args) {
				cout << "Not enough arguments for command " << args[0] << endl;
				continue;
			}
			(this->*commands[args[0]].second)(args, client_fd);
		}
		else
		{
			
		}
		
	}
}

void Server::nick(vector<string> args, int cl) {
	string msg(args[0]);
	msg += " " + args[1];
	send(cl, msg.c_str(), strlen(msg.c_str()), 0);
	//Rep(*this).R001(cl.get_id(), args[1])
}