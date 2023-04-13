/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Parser.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bducrocq <bducrocq@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/06 20:39:56 by abucia            #+#    #+#             */
/*   Updated: 2023/04/13 15:42:23 by bducrocq         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "./Server.hpp"

void	Server::init_parsing_map()
{
	this->commands["NICK"] = make_pair(1, &Server::nick);
	this->commands["PING"] = make_pair(1, &Server::ping);
	this->commands["USER"] = make_pair(1, &Server::user);
	this->commands["PRIVMSG"] = make_pair(1, &Server::privmsg);
	this->commands["JOIN"] = make_pair(2, &Server::join);
	this->commands["MODE"] = make_pair(2, &Server::mode);
	this->commands["LIST"] = make_pair(1, &Server::list);
	this->commands["TOPIC"] = make_pair(1, &Server::topic);
	cout << ANSI::yellow << "init PARSING OK" << endl;
}

vector<string> split_cmd(const string command, char separator)
{
	stringstream stream(command);
	vector<string> args;
	string buffer;
	while (getline(stream, buffer, separator))
	{
		if (buffer.length() != 0)
			args.push_back(buffer);
		if (args.size() > 0 && args[0] == "PRIVMSG" && buffer[0] == ':')
		{
			args.push_back(buffer.substr(1));

			break;
		}
		cout << ANSI::purple << "ADD ARG : " << ANSI::red << buffer << endl; // DEBUG
	}
	cout << endl; // DEBUG
	return args;
}

string get_cmd(string cmd)
{
	string command = "";
	for (size_t i = 0; i < cmd.length(); i++)
		if (cmd[i] != '\n')
			command += cmd[i];
	return command;
}

void	Server::parser(string cmd, int client_fd) {
	string command = get_cmd(cmd);
	vector<string> cmds = split_cmd(command, '\r');
	if (cmds.size() == 0)
	{
		if (_client.find(client_fd) == _client.end())
			return Rep().E421(client_fd ,"*", command);
		return Rep().E421(client_fd ,_client[client_fd].get_nick(), command);
	}
	for (vector<string>::iterator it = cmds.begin(); it != cmds.end(); it++)
	{
		vector<string> args = split_cmd(*it, ' ');
		if (args.size() != 0 && commands.find(args[0]) != commands.end())
		{
			long unsigned int expected_args = commands[args[0]].first;
			if (args.size() < expected_args) {
				cout << "Not enough arguments for command " << args[0] << endl;
				continue;
			}
			(this->*commands[args[0]].second)(args, client_fd);
		}
		else
		{
			if (_client.find(client_fd) == _client.end())
				return Rep().E421(client_fd ,"*", command);
			if (args.size() == 0)
				return Rep().E421(client_fd ,_client[client_fd].get_nick(), command);
			Rep().E421(client_fd ,_client[client_fd].get_nick(), args[0]);
			cout << ANSI::red << "Command not found for " << args[0] << endl;
		}
	}
}
