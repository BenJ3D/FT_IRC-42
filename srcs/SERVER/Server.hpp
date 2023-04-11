/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abucia <abucia@student.42lyon.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/29 00:12:30 by bducrocq          #+#    #+#             */
/*   Updated: 2023/04/10 20:36:08 by abucia           ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVER_HPP
# define SERVER_HPP

# include <stdlib.h>
# include <cstdlib>
# include <cstdio>
# include <ctime>
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
# define SERVER_NAME "minitel_rose"
# define SERVER_VERSION "1.0.0"
# define SERVER_DATE "2021-03-29"

class Channel;
class Server
{
	private:
		Server(Server const &src);
		Server(void);

		int		openSocket(int port);
		void	init_parsing_map();

		string					_server_name;
		string					_pass_word;
		fd_set					_read_fds;
		int						_max_fd;
		vector<int>				_client_fds;
		map<int,Client>			_client;
		map<string,Channel>		_channel;

		std::map<std::string, std::pair<long unsigned int, CmdFunc> > commands;
		void notice(int const &fd, string msg);

		/** COMMAND **/
		void					nick(vector<string> args, int cl);
		void					ping(vector<string> args, int cl);
		void					user(vector<string> args, int cl);
		void					privmsg(vector<string> args, int cl);
		void					mode(vector<string> args, int fd_client);
		void					join_channel(vector<string> args, int fd_client);
		void					list(vector<string> args, int fd_client);
		void					topic(vector<string> args, int fd_client);


	public:
		Server(std::string port, std::string address);//adress doit devenir password
		~Server();

		Server &operator=(Server const &rhs);

		/* --- PARSING --- */
		void parser(string command, int client_fd);

};

void confirm_to_client(const int &fd, string msg, map<int, Client>);
void confirm_to_all_channel_client(int const &fd, string msg, map<int, Client> _client, Channel chan);
std::ostream &operator<<(std::ostream &o, Server const &i);



#endif /* ********************************************************* SERVER_HPP */

