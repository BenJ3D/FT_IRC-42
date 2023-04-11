/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Bot.hpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amiguez <amiguez@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/09 06:32:18 by amiguez           #+#    #+#             */
/*   Updated: 2023/04/11 14:39:40 by amiguez          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef BOT_HPP
# define BOT_HPP

# include <iostream>
# include <string>
# include <sstream>
# include <vector>
# include <map>
# include <sys/types.h>
# include <sys/socket.h>
# include <netinet/in.h>
# include <arpa/inet.h>
# include <unistd.h>
# include <stdlib.h>

# include "ANSI.hpp"

# define USAGE "./Bob <password> [<ip_address> [port]]"
class Bot;

using namespace std;
typedef void (Bot::*CmdFunc)(string);
using namespace ANSI;

class Bot{
	private :
		Bot(const Bot& src);
		Bot operator=(const Bot& rhs);
		
		int _server_fd;
		string _real_name;
		string _nick_name;

		string _channel;
		map<string, CmdFunc > m_cmd;

		
		void init_cmds();
			void ping(string input);
			void rps(string input);
			void help(string input);
			void quit(string input);

			void join(string input);
			void leave(string input);
			void join();

		int send_serv(string input);
		string recv_serv();
	public :
		void init(string ip = "127.0.0.1", string port = "6667") throw(exception);
		Bot();
		~Bot();

		void auth(string pasw) throw(exception);
		void run();

};

#endif