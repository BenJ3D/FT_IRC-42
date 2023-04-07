/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bducrocq <bducrocq@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/29 00:12:30 by bducrocq          #+#    #+#             */
/*   Updated: 2023/04/05 10:30:00 by bducrocq         ###   ########lyon.fr   */
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

class Server
{
	private:
		Server(Server const &src);
		Server(void);

		int		openSocket(int port);
		void	init_parsing_map();

		string _server_name;
		string _pass_word;
		fd_set _read_fds;
		int _max_fd;
		vector<int> _client_fds;
		map<int,Client> _client;
		map<int,Channel> _channel;

		std::map<std::string, std::pair<int, CmdFunc> > commands;


		/** COMMAND **/
		void nick(vector<string> args, int cl);
		// void join(vector<string> args, Client& cl);
		// void privmsg(vector<string> args, Client& cl);

	public:
		Server(std::string port, std::string address);//adress doit devenir password
		~Server();

		Server &operator=(Server const &rhs);

		/* --- PARSING --- */
		void parser(string command, int client_fd);
};

std::ostream &operator<<(std::ostream &o, Server const &i);

#endif /* ********************************************************* SERVER_HPP */

