/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bducrocq <bducrocq@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/06 20:39:56 by abucia            #+#    #+#             */
/*   Updated: 2023/04/17 01:17:31 by bducrocq         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./Server.hpp"

void	Server::init_parsing_map()
{
	this->commands["NICK"] = make_pair(0, &Server::nick);
	this->commands["PING"] = make_pair(0, &Server::ping);
	this->commands["USER"] = make_pair(0, &Server::user);
	this->commands["PRIVMSG"] = make_pair(1, &Server::privmsg);
	this->commands["PASS"] = make_pair(1, &Server::pass);
	this->commands["NOTICE"] = make_pair(1, &Server::cmd_notice);
	this->commands["MODE"] = make_pair(0, &Server::mode);
	this->commands["KICK"] = make_pair(1, &Server::kick);
	this->commands["JOIN"] = make_pair(0, &Server::join);
	this->commands["LIST"] = make_pair(0, &Server::list);
	this->commands["TOPIC"] = make_pair(1, &Server::topic);
	this->commands["PART"] = make_pair(1, &Server::part);
	this->commands["QUIT"] = make_pair(1, &Server::quit);
	this->commands["OPER"] = make_pair(0, &Server::oper);
	this->commands["MOTD"] = make_pair(0, &Server::motd);
	this->commands["NAMES"] = make_pair(0, &Server::names);
	this->commands["INVITE"] = make_pair(0, &Server::invite);
	cout << ANSI::yellow << "init PARSING OK" << endl;
}

std::string Server::trim(std::string str)
{
	for (size_t i = 0; i < str.length(); i++)
	{
		if (str[i] == ' ')
			str.erase(i, 1);
		else
			break;
	}
	for (size_t i = str.length() - 1; i > 0; i--)
	{
		if (str[i] == ' ')
			str.erase(i, 1);
		else
			break;
	}
	return str;
}

vector<string> Server::split_to_point(string str)
{
	vector<string> args;
	for (size_t i = 1; i < str.length(); i++)
	{
		if (str[i] == ':' && str[i - 1] == ' ')
		{
			args.push_back(str.substr(0, i));
			args.push_back(str.substr(i + 1, str.length() - i));
			return args;
		}
	}
	return args;
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
	string command = trim(get_cmd(cmd));
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
		cout << "DEBUG : " << _client[client_fd].get_pass_confirm() << " " << _client[client_fd].get_is_auth() << " " << args[0] << endl;
		if (!args.empty() && !(_client[client_fd].get_pass_confirm() || args[0] == "PASS"))
			continue;
		else if (_client[client_fd].get_pass_confirm() && !_client[client_fd].get_is_auth() && !(args[0] == "NICK" || args[0] == "USER"))
		{
			cout << "NOPEEEEEEEEEEEEEEEEEEEEEEEEEE" << endl;
			continue;
		}
		if (commands.find(args[0]) != commands.end())
		{
			cout << ANSI::back_cyan << ANSI::red << "pars" << args[0] << ANSI::r << endl;
				if (commands[args[0]].first == 1)
					args.push_back(split_cmd(cmd, '\r')[distance(cmds.begin(), it)]);
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
