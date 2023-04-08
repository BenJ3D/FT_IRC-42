/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Parser.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abucia <abucia@student.42lyon.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/06 20:39:56 by abucia            #+#    #+#             */
/*   Updated: 2023/04/08 06:25:44 by abucia           ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "./Server.hpp"

void	Server::init_parsing_map()
{
	cout << ANSI::yellow << "init PARSING OK" << endl;
	this->commands["NICK"] = make_pair(2, &Server::nick);
	this->commands["PING"] = make_pair(2, &Server::ping);
	this->commands["USER"] = make_pair(5, &Server::user);
	// this->commands["JOIN"] = make_pair(2, &nick);
	// this->commands["PRIVMSG"] = make_pair(3, &nick);
}

vector<string> split_cmd(const string command, char separator)
{
	stringstream stream(command);
	vector<string> args;
	string buffer;
	while (getline(stream, buffer, separator))
	{
		if (buffer[buffer.length() - 1] == '\r')
		{
			cout << ANSI::red << "REMOVE \\r ⏬" << endl;
			buffer.substr(0, buffer.length() - 1);
		}
		else if (separator == '\n')
		{
			cerr << ANSI::red << "ERROR: Command not ended with \\r\\n" << endl;
			return vector<string>();
		}
		args.push_back(buffer);
		cout << ANSI::purple << "ADD ARG : " << ANSI::red << buffer << endl; // DEBUG
	}
	cout << endl;
	return args;
}

void	Server::parser(string command, int client_fd) {
	vector<string> cmds = split_cmd(command, '\n');
	if (cmds.size() == 0)
		return;
	for (vector<string>::iterator it = cmds.begin(); it != cmds.end(); it++)
	{
		vector<string> args = split_cmd(*it, ' ');
		cout << args[0].length() << " " << args[0].c_str() << endl;
		if (commands.find(args[0]) != commands.end())
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
			
		}
		
	}
}
