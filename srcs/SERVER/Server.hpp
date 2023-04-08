/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amiguez <amiguez@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/29 00:12:30 by bducrocq          #+#    #+#             */
/*   Updated: 2023/04/08 06:40:42 by amiguez          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVER_HPP
# define SERVER_HPP

# include <cstdlib>
# include <cstdio>
# include <stdlib.h>
# include <string>
# include <cstring>
# include <iostream>
# include <sstream>
# include <sys/types.h>
# include <sys/select.h>
# include <sys/socket.h>
# include <netinet/in.h>
# include <arpa/inet.h>
# include <unistd.h>
# include <vector>
# include <map>

# include "../UTILS/ANSI.hpp"
# include "./Channel.hpp"
# include "../CLIENT/Client.hpp"
# include "../UTILS/NumericReplies.hpp"

using namespace std;
typedef void (Server::*CmdFunc)(std::vector<std::string>, int);
# define SERVER_NAME "127.0.0.1"

class Server
{
	private:
		Server(Server const &src);
		Server(void);

		int		openSocket(int port);
		void	init_parsing_map();

		const string _pass_word;
		fd_set _read_fds;
		int _max_fd;
		vector<int> _client_fds;
		map<int,Client> _client;
		map<int,Channel> _channel;

		std::map<std::string, std::pair<long unsigned int, CmdFunc> > commands;


		/** COMMAND **/
		void nick(vector<string> args, int cl);
		void ping(vector<string> args, int cl);
		void user(vector<string> args, int cl);
		void pass(vector<string> args, int cl);
		// void join(vector<string> args, Client& cl);
		// void privmsg(vector<string> args, Client& cl);

	public:
		Server(std::string port, std::string password);
		~Server();

		Server &operator=(Server const &rhs);

		/* --- PARSING --- */
		void parser(string command, int client_fd);
};

std::ostream &operator<<(std::ostream &o, Server const &i);

#endif /* ********************************************************* SERVER_HPP */

