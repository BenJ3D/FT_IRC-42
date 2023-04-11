/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Bot.hpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amiguez <amiguez@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/09 06:32:18 by amiguez           #+#    #+#             */
/*   Updated: 2023/04/11 05:28:37 by amiguez          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef BOT_HPP
# define BOT_HPP

# include <iostream>
# include <string>
# include <sys/types.h>
# include <sys/socket.h>
# include <netinet/in.h>
# include <arpa/inet.h>
# include <unistd.h>
# include <stdlib.h>

# include "ANSI.hpp"

# define USAGE "./Bob <password> [<ip_address> [port]]"

using namespace std;
using namespace ANSI;

class Bot{
	private :
		Bot(const Bot& src);
		Bot operator=(const Bot& rhs);
		
		int _server_fd;
		string _real_name;
		string _nick_name;


		bool _auth;

	public :
		void init(string ip = "127.0.0.1", string port = "6667") throw(exception);
		Bot();
		~Bot();

		void auth(string pasw) throw(exception);

		bool is_auth();
};

#endif