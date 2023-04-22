/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Bot.hpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amiguez <amiguez@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/09 06:32:18 by amiguez           #+#    #+#             */
/*   Updated: 2023/04/20 15:59:31 by amiguez          ###   ########.fr       */
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
# include <limits>

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
		string _ip;
		bool _run;

		string _channel;

		void init_cmds();
			void help();
			void ping();
			void rps(string input);
			void bj_bob();

			void join(string chan = "");
			void part();
			void quit();

		int send_serv(string input);
		string recv_serv();
	public :
		void init(string ip = "127.0.0.1", string port = "6667") throw(exception);
		Bot();
		~Bot();

		void auth(string pasw) throw(exception);
		void run();

};

	vector<string> split_cmd(const string command, char separator);

#endif