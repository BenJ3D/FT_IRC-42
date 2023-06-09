/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bducrocq <bducrocq@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/29 00:12:30 by bducrocq          #+#    #+#             */
/*   Updated: 2023/04/27 00:09:25 by bducrocq         ###   ########.fr       */
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
# include <sstream>
# include <iostream>
# include <sys/types.h>
# include <sys/select.h>
# include <sys/socket.h>
# include <netinet/in.h>
# include <arpa/inet.h>
# include <unistd.h>
# include <errno.h>
# include <fstream>
# include <vector>
# include <map>
# include <set>
# include <cctype>

# include "ANSI.hpp"
# include "Channel.hpp"
# include "Client.hpp"
# include "NumericReplies.hpp"
# include "tools.hpp"

using namespace std;
typedef void (Server::*CmdFunc)(std::vector<std::string>, int);
# define SERVER_NAME "minitel_rose"
# define SERVER_VERSION "1.0.0"
# define SERVER_DATE "2021-03-29"

class Channel;
class Client;
class Server
{
	private:
		Server(Server const &src);
		Server();

		int		openSocket(int port);
		void	init_parsing_map();

		const string			_pass_word;
		string					_oper_passw;
		string					_oper_user;
		string					_server_name;
		string					_motd;
		map<string,Channel>		_channel;
		map<int,Client>			_client;
		fd_set					_read_fds;
		int						_max_fd;
		bool					_delete_client;

		std::map<std::string, std::pair<long unsigned int, CmdFunc> > commands;

		void		config();
		void		notice(int const &fd, string msg);

		/* --- COMMAND --- */
		void					pass(vector<string> args, int cl);
		void					nick(vector<string> args, int cl);
		void					ping(vector<string> args, int cl);
		void					user(vector<string> args, int cl);
		void					kick(vector<string> args, int cl);
		void					privmsg(vector<string> args, int cl);
		void					motd(vector<string> args, int cl);
		void					motd_auth(int cl);
		void					cmd_notice(vector<string> args, int client_fd);
		void					mode(vector<string> args, int fd_client);
		void					mode_channel(vector<string> args, int fd_client);
		void					mode_client(vector<string> args, int fd_client);
		void					join(vector<string> args, int fd_client);
		void					list(vector<string> args, int fd_client);
		void					topic(vector<string> args, int fd_client);
		void					part(vector<string> args, int fd_client);
		void					quit(vector<string> args, int fd_client);
		void					oper(vector<string> args, int fd_client);
		void					names(vector<string> args, int fd_client);
		void					invite(vector<string> args, int fd_client);

	public:

		Server(std::string port, std::string password);
		~Server();
		Server &operator=(Server const &rhs);

		/* --- GETTER --- */
		map<int, Client>		&get_client();
		map<string, Channel>	&get_channel();
		string					recupAllModChan(string channelName, bool displayKey);

		void					send_error(int fd);

		/* --- PARSING --- */
		void					parser(string command, int client_fd);
		vector<string>			split_to_point(string str);
		std::string				trim(std::string str);
		vector<string>			super_split(string cmd, int nb_arg);

		/* --- SERVER --- */
		bool					isExistChannelName(string const &channelName);
		int						findClientFdWithNick(string const &nick);
};

	std::ostream &operator<<(std::ostream &o, Server const &i);

#endif /* ********************************************************* SERVER_HPP */

